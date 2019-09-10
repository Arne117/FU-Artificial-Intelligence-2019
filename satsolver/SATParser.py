from functools import reduce

def parse(pathToFile, sort=True, log=True):
  minisatFile = open(pathToFile, "r").read().split('\n')
  numVariables = 0
  numClauses = 0
  clauses = []


  i = 0
  for line in minisatFile:
    if line[0] == "c": continue

    if line[0] == "p":
      lineContent = line.split()
      numVariables = int(lineContent[2])
      numClauses = int(lineContent[3])
      clauses = [[] for n in range(numClauses)]
      continue

    variables = map(int, line.split()[:-1])
    clauses[i].extend(variables)
    if sort:
      clauses[i].sort()
    i += 1

  assert numVariables > 0 and numClauses > 0 and reduce(lambda a, b: (a) + len(b), clauses, 0) > 0

  if log:
    print("--- Header ---")
    print("Variables: ", numVariables, "| Clauses: ", numClauses)
    print("--- END Header ---")
    print("--- Clauses ---")
    print(clauses)
    # for c in clauses: print(c)
    print("--- END Clauses ---")
  return (clauses, numVariables, numClauses)
