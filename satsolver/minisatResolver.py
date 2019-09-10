#!/usr/bin/python3

import sys
import os
from SATParser import parse
from Solver import solve


clauses, numVariables, numClauses = parse(sys.argv[1])

isSolved, solutions = solve(clauses, [])

print("is solved", isSolved, "solutions", solutions)