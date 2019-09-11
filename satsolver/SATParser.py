from functools import reduce

COMMENT_CHARS = {"c", "%", "0"}

def parse(pathToFile, log=True):
  minisatFile = open(pathToFile, "r").read().split('\n')
  numVariables = 0
  numClauses = 0
  clauses = []

  for line in minisatFile:
    if len(line) == 0 or line[0] in COMMENT_CHARS:
      continue

    if line[0] == "p":
      lineContent = line.split()
      numVariables = int(lineContent[2])
      numClauses = int(lineContent[3])
      continue

    variables = list(set(map(int, line.split()[:-1])))
    clauses.append(variables)

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
