
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <map>

using namespace std;

bool doPrint = false;

class Formel {
public:
    Formel(std::string name) {
        readFile(name);
    }

    void readFile(std::string name) {
        std::ifstream knf_file(name);
        while (knf_file.is_open() && !knf_file.eof()) {
            std::string line;
            std::getline(knf_file, line);
            int variables;
            int nr_clauses;
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
        for (unsigned int i = 0; i < clauses.size(); i++) {
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
        for (unsigned int i = 0; i < clauses.size(); i++) {
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
        for (unsigned int i = 0; i < clauses.size(); i++) {
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
        std::map<int,int> found;
        maxv = 0;
        std::map<int,bool> signs; // vorzeichen -1 => -, 1 = +, 0 => beides => nicht gültig

        for (unsigned int i = 0; i < clauses.size(); i++) {
            for (auto it = clauses[i].begin(); it != clauses[i].end(); ++it) {
                int key = abs(*it);
                bool isPos = *it > 0;
                if (found.find(key) != found.end()) {
                    found[key]++;
                    if (signs.find(key) != signs.end() && signs[key] != isPos) {
                        signs.erase(key);
                    }
                } else {
                    found.emplace(key,1);
                    signs.emplace(key,isPos);
                }

                if (maxv == 0 || found[key] > found[maxv]) {
                    maxv = key;
                }
            }
        }

        for (auto it = signs.begin(); it != signs.end(); ++it) {
            int var = (it->second)? it->first : -it->first;
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

    Formel getCopy() {
        return Formel(this->clauses, this->solution);
    }

    void overwriteWith(Formel formel) {
        this->clauses = formel.clauses;
        this->solution = formel.solution;
    }

    Formel(std::vector<std::set<int>> clauses, std::vector<int> solution) {
       this->clauses = clauses;
       this->solution = solution;
    }

    int maxv = 0;
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

        Formel tempFormel = formel.getCopy();
        tempFormel.solveForVar(v);
        if(solve(tempFormel)) {
            formel.overwriteWith(tempFormel);
            return true;
        }

        v = -v;
        if (doPrint) {
            std::cout << "split try " << v << std::endl;
        }
        tempFormel = formel.getCopy();
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