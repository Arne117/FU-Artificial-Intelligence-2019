#!/usr/bin/python3

# c Here is a comment.
# p cnf 5 3
# 1 -5 4 0
# -1 5 3 4 0
# -3 -4 0

import sys
import os

# switch cwd to /satsolver
os.chdir("satsolver")
# print(os.getcwd())
# print(os.listdir())

class minisatResolver():
  # def __init__(self):
  #   self.variablesNum = 0
  #   self.clausesNum = 0

  satFileName = sys.argv[1]
  minisatFile = open(satFileName + ".cnf", "r")
  foundHeader = False

  for line in minisatFile:
    lineContent = line.split()
    # if line begins with a comment
    if lineContent[0] == "c":
      # print("Comment: " + line)
      continue
    # parse header
    if lineContent[0] == "p" and lineContent[1] == "cnf":
      print("--- Header ---")
      variablesNum = lineContent[2]
      clausesNum = lineContent[3]
      print("Variables: " + variablesNum, "| Clauses: " + clausesNum)
      print("--- END Header ---")
      foundHeader = True
      continue
    if foundHeader == True:
      print(line)
