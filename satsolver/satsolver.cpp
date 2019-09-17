#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <future>
#include <chrono>
#include <mutex>
#include <csignal>

using namespace std;

bool doPrint = false;
bool result_printed = false;
std::mutex result_mutex;

std::vector<std::future<bool>> pending_futures;
std::mutex pending_futures_mutex;

class Formel {
public:
    Formel() {

    }

    Formel(std::string name) {
        readFile(name);
    }

    void readFile(std::string name) {
        std::ifstream knf_file(name);
        indizes.push_back(0);
        while (knf_file.is_open() && !knf_file.eof()) {
            std::string line;
            std::getline(knf_file, line);
            if (line[0] == 'p') {
                int nr_clauses;
                std::sscanf(line.c_str(), "p cnf %d %d", &variables, &nr_clauses);
                values.reserve(nr_clauses * 3); // Schätzung, die meisten haben min. 3
                continue;
            }
            if (line.size() > 0 && line[0] != 'c' && line[0] != '%' && line[0] != '0') {
                int temp;
                std::stringstream lineStream(line);
                // solange es in eine Zahl konvertiert werden kann
                while (lineStream >> temp) {
                    // nur bis 0
                    if (temp == 0) break;
                    values.push_back(temp);
                }
                indizes.push_back(values.size());
            }
        }
        if (values.size() == 0) {
            std::cout << "Datei konnte nicht gelesen werden." << std::endl;
        }
    }

    void print() {
        if (values.size() == 0) {
            std::cout << "[]" << std::endl;
        } else {
            std::cout << "cnf:[[";
            unsigned int j = 1;
            for (unsigned int i = 0; i < values.size(); ++i) {
                std::cout << values[i];
                if (i+1 != indizes[j]) {
                    std::cout << ",";
                } else if (i+1 != values.size()) {
                    std::cout << "]," << std::endl << "[";
                    j++;
                }
            }
            std::cout << "]]" << std::endl;
        }
        std::cout << "solution:[";
        for (unsigned int i = 0; i < solution.size(); ++i) {
            std::cout << solution[i];
            if (i+1 != solution.size()) {
                std::cout << ",";
            }
        }
        std::cout << "]" << std::endl;
    }

    void solveForVar(int var) {
        solution.push_back(var);
        int j = indizes.size()-2;
        for (int i = values.size()-1; i >= 0; --i) {
            // i = aktuelle pos
            // j = index an dem unsere clausel startet (j+1 => erstes element der nächsten Klausel)
            if (values[i] == var) {
                values.erase (values.begin()+indizes[j],values.begin()+indizes[j+1]);
                i = indizes[j];

                int s = indizes[j+1] - indizes[j];
                for(int k = j+1; k<indizes.size(); ++k) {
                    indizes[k] -= s;
                }
                indizes.erase (indizes.begin()+j);
            } else if (values[i] == -var) {
                values.erase(values.begin()+i);
                for(int k = j+1; k<indizes.size(); ++k) {
                    --indizes[k];
                }
            }
            while (i <= indizes[j] && j != 0) {
                j--;
            }
        }
    }

    bool isInvalid() {
        for (unsigned int i = 0; i < indizes.size()-1;++i) {
            if (indizes[i] == indizes[i+1]) {
                if (doPrint) {
                    std::cout << "invalid branch" << std::endl;
                }
                return true;
            }
        }
        return false;
    }

    bool solveUnitProp() {
        for (unsigned int j = 0; j < indizes.size()-1;++j) {
            if (indizes[j] + 1 == indizes[j+1]) {
                int var = values[indizes[j]];
                solveForVar(var);
                if (doPrint) {
                    std::cout << "Unit propagation " << var << std::endl;
                }
                return true;
            }
        }
        return false;
    }

    bool solvePureLit() {
        std::vector<int> found(variables+1, 0);
        std::vector<int> signs(variables+1, 0);
        std::set<int> pureliterals;
        maxv = 0;
        for (unsigned int i = 0; i < values.size(); ++i) {
            int key = abs(values[i]);
            int sign = (values[i] > 0)? 1 : -1;
            found[key]++;
            if (found[key] == 1) {
                signs[key] = sign;
                pureliterals.insert(values[i]);
            } else if (signs[key] != 0 && signs[key] != sign) {
                pureliterals.erase(signs[key]*key);
                signs[key] = 0;
            }
            if (maxv == 0 || found[key] > found[maxv]) {
                maxv = key;
            }
        }

        for (auto it = pureliterals.begin(); it != pureliterals.end(); ++it) {
            int var = *it;
            solveForVar(var);
            if (doPrint) {
                std::cout << "Pure Literal " << var << std::endl;
            }
            return true;
        }
        return false;
    }

    bool empty() {
        return values.empty() && indizes.size() == 1;
    }

    void overwriteWith(Formel formel) {
        this->values = formel.values;
        this->indizes = formel.indizes;
        this->solution = formel.solution;
        this->variables = formel.variables;
    }

    int maxv = 0;
    int variables = 0;

    std::vector<int> values;
    std::vector<int> indizes;
    std::vector<int> solution;
};

void printResult(bool result, Formel& formel) {
    result_mutex.lock();
    if (!result_printed) {
        result_printed = true;
        if (result) { 
            std::cout << "% SZS status Satisfiable" << std::endl;
            //formel.print();
        } else {
            std::cout << "% SZS status Unsatisfiable" << std::endl;
        }
    }
    result_mutex.unlock();
}

bool solve(Formel& formel, int depth) {
    while(true) {
        if (formel.empty()) {
            if (doPrint) {
                std::cout << "solved" << std::endl;
            }
            return true;
        }

        if (formel.isInvalid()) {
            return false;
        }

        if (formel.solveUnitProp()) {
            continue;
        }

        if (formel.solvePureLit()) {
            continue;
        }

        int v = formel.maxv;
        if (depth > 0) {
            Formel tempFormel1;
            tempFormel1.overwriteWith(formel);
            tempFormel1.solveForVar(v);
            std::future<bool> future1( std::async(solve,std::ref(tempFormel1),depth-1));

            v = -v;
            if (doPrint) {
                std::cout << "split try " << v << std::endl;
            }
            Formel tempFormel2;
            tempFormel2.overwriteWith(formel);
            tempFormel2.solveForVar(v);
            std::future<bool> future2(std::async(solve,std::ref(tempFormel2),depth-1));

            bool future1read = false;
            bool future2read = false;
            while (!(future1read && future2read)) {
                if (!future1read) {
                    if (std::future_status::ready == future1.wait_for(std::chrono::milliseconds(100))) {
                        future1read = true;
                        if (future1.get()) {
                            printResult(true, tempFormel1);
                            if (!future2read) {
                                // schiebe future nach global,
                                // da wir im destruktor von future auf abarbeitung warten
                                pending_futures_mutex.lock();
                                pending_futures.push_back(std::move(future2));
                                pending_futures_mutex.unlock();
                            }
                            return true;
                        }
                    }
                }
                
                if (!future2read) {
                    if (std::future_status::ready == future2.wait_for(std::chrono::milliseconds(100))) {
                        future2read = true;
                        if (future2.get()) {
                            printResult(true, tempFormel2);
                            if (!future1read) {
                                // schiebe future nach global,
                                // da wir im destruktor von future auf abarbeitung warten
                                pending_futures_mutex.lock();
                                pending_futures.push_back(std::move(future1));
                                pending_futures_mutex.unlock();
                            }
                            return true;
                        }
                    }
                }
            }
        } else {
            if (doPrint) {
                std::cout << "split try " << v << std::endl;
            }
            Formel tempFormel;
            tempFormel.overwriteWith(formel);
            tempFormel.solveForVar(v);
            if (solve(tempFormel,depth-1)) {
                printResult(true, tempFormel);
                return true;
            }

            v = -v;
            if (doPrint) {
                std::cout << "split try " << v << std::endl;
            }
            tempFormel.overwriteWith(formel);
            tempFormel.solveForVar(v);
            if (solve(tempFormel,depth-1)) {
                printResult(true, tempFormel);
                return true;
            }
        }
        return false;
    }
}

int main(int argc, char **argv) {
    std::cout << "SATSolver gestartet..." << std::endl;
    if (argc >= 2) {
        Formel formel = Formel(argv[1]);
        if (doPrint) formel.print();
         // 3 = 8Threads, 2 = 4, 1 = 2, 0 = 1...
        printResult(solve(formel, 3), formel);
    } else {
        std::cout << "kein Input" << std::endl;
    }
    // prozess killen, da er sonst wegen der futures weiterläuft
    std::raise(SIGTERM);
    return 0;
}
