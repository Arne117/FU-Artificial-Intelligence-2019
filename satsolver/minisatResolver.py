#!/usr/bin/python3

import sys
import os
from SATParser import parse


clauses, numVariables, numClauses = parse(sys.argv[1])
