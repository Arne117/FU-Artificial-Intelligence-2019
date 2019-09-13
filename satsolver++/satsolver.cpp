
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <sstream>

using namespace std;

bool doPrint = false;

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

bool solve(Formel& formel) {
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
        if (doPrint) {
            std::cout << "split try " << v << std::endl;
        }

        Formel tempFormel;
        tempFormel.overwriteWith(formel);
        tempFormel.solveForVar(v);
        if(solve(tempFormel)) {
            formel.overwriteWith(tempFormel);
            return true;
        }

        v = -v;
        if (doPrint) {
            std::cout << "split try " << v << std::endl;
        }
        tempFormel.overwriteWith(formel);
        tempFormel.solveForVar(v);
        if(solve(tempFormel)) {
            formel.overwriteWith(tempFormel);
            return true;
        }

        return false;
    }
}

int main(int argc, char **argv) {
    std::cout << "SATSolver gestartet..." << std::endl;
    if (argc >= 2) {
        Formel formel = Formel(argv[1]);
        if (doPrint) formel.print();
        if(solve(formel)) {
            std::cout << "% SZS status Satisfiable" << std::endl;
        } else {
            std::cout << "% SZS status Unsatisfiable" << std::endl;
        }
    } else {
        std::cout << "kein Input" << std::endl;
    }

    return 0;
}