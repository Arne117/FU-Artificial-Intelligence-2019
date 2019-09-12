#!/usr/bin/python3

import sys
import os
from SATParser import parse
from Solver import solve


clauses, numVariables, numClauses = parse(sys.argv[1], False)

isSolved, solutions = solve(clauses, [])

if isSolved:
	print("% SZS status Satisfiable")
	#print("is solved", isSolved, "solutions", solutions)
else:
	print("% SZS status Unsatisfiable")