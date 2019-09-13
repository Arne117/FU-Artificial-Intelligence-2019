#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <future>
#include <chrono>
#include <mutex>

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
        while (knf_file.is_open() && !knf_file.eof()) {
            std::string line;
            std::getline(knf_file, line);
            if (line[0] == 'p') {
                std::sscanf(line.c_str(), "p cnf %d %d", &variables, &nr_clauses);
                clauses.reserve(nr_clauses);
                continue;
            }
            if (line.size() > 0 && line[0] != 'c' && line[0] != '%' && line[0] != '0') {
                clauses.push_back(std::set<int>());
                int temp;
                std::stringstream lineStream(line);
                // solange es in eine Zahl konvertiert werden kann
                while (lineStream >> temp) {
                    // nur bis 0
                    if (temp == 0) break;
                    clauses.back().insert(temp);
                }
            }
        }
        if (clauses.size() == 0) {
            std::cout << "Datei konnte nicht gelesen werden." << std::endl;
        }
    }

    void print() {
        std::cout << "[";
        for (unsigned int i = 0; i < clauses.size(); ++i) {
            std::cout << "[";
            for (auto it = clauses[i].begin(); it != clauses[i].end(); ++it) {
                std::cout << *it;
                if (it != clauses[i].end()--) {
                    std::cout << ",";
                }
            }
            std::cout << "]";
            if (i < clauses.size() -1 ) {
                std::cout << std::endl;
            }
        }
        std::cout << "]" << std::endl;
    }

    void solveForVar(int var) {
        for (int i = clauses.size()- 1; i >= 0; i--) {
            if (clauses[i].find(var) != clauses[i].end()) {
                clauses.erase(clauses.begin() + i);
            } else if (clauses[i].find(-var) != clauses[i].end()) {
                clauses[i].erase(-var);
            }
        }
    }

    bool isInvalid() {
        for (unsigned int i = 0; i < clauses.size(); ++i) {
            if(clauses[i].empty()) {
                if (doPrint) {
                    std::cout << "invalid branch" << std::endl;
                }
                return true;
            }
        }
        return false;
    }

    bool solveUnitProp() {
        for (unsigned int i = 0; i < clauses.size(); ++i) {
            if (clauses[i].size() == 1) {
                int var = *clauses[i].begin();
                solveForVar(var);
                solution.push_back(var);
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

        for (unsigned int i = 0; i < clauses.size(); ++i) {
            for (auto it = clauses[i].begin(); it != clauses[i].end(); ++it) {
                int key = abs(*it);
                int sign = (*it > 0)? 1 : -1;
                found[key]++;
                //std::cout << key << "/" << found.size() << std::endl;
                if (found[key] == 1) {
                    signs[key] = sign;
                    pureliterals.insert(*it);
                } else if (signs[key] != 0 && signs[key] != sign) {
                    pureliterals.erase(signs[key]*key);
                    signs[key] = 0;
                }
                if (maxv == 0 || found[key] > found[maxv]) {
                    maxv = key;
                }
            }
        }

        for (auto it = pureliterals.begin(); it != pureliterals.end(); ++it) {
            int var = *it;
            solveForVar(var);
            solution.push_back(var);
            if (doPrint) {
                std::cout << "Pure Literal " << var << std::endl;
            }
            return true;
        }
        return false;
    }

    bool empty() {
        return clauses.empty();
    }

    void overwriteWith(Formel formel) {
        this->clauses = formel.clauses;
        this->solution = formel.solution;
        this->variables = formel.variables;
        this->nr_clauses = formel.nr_clauses;
    }

    int maxv = 0;
    int variables;
    int nr_clauses;
    std::vector<std::set<int>> clauses;
    std::vector<int> solution;
};

void printResult(bool result, Formel& formel) {
    result_mutex.lock();
    if (!result_printed) {
        result_printed = true;
        if (result) { 
            std::cout << "% SZS status Satisfiable" << std::endl;
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
    std::terminate();
    return 0;
}
