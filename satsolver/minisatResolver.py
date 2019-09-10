#!/usr/bin/python3

import sys
import os
from SATParser import parse
from Solver import solveForVar


clauses, numVariables, numClauses = parse(sys.argv[1])

solutions = []

