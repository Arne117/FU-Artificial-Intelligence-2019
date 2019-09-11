from copy import deepcopy

false_solutions = []

# optimization: only one loop if possible
def solveForVar(clauses, var):
  for c in clauses[:]:
    if var in c:
      # mark resolved clauses for deletion
      clauses.remove(c)
      continue

    # er hat ein böses bsp [17,17,40], einmal remove entfernt nur das eine 17
    while (-var in c):
      # remove unresolvable literals
      c.remove(-var)

def isAlreadyKnownAsFalse(solutions):
  for not_solution in false_solutions:
    if not_solution.issubset(set(solutions)):
      # auf dieser basis kann es keine lösung geben
      return True
  return False

def addFalseSolution(solutions):
  solset = set(solutions)
  combined = False
  for v in solutions:
    solset.remove(v)
    solset.add(-v)
    if solset in false_solutions:
      # falls ein mit uns kombinierbare lösung exisitert
      # diese löschen
      false_solutions.remove(solset)
      # und die kombinierte hinzufügen
      solset.remove(-v)
      addFalseSolution(solset)
      combined=True
    else:
      # ursprüngliches set wiederherstellen
      solset.remove(-v)
    # ursprüngliches set wiederherstellen 
    solset.add(v)
  if (not combined) and (not (solset in false_solutions)):
    false_solutions.append(solset)

def solve(clauses, solutions):
  if isAlreadyKnownAsFalse(solutions):
    print(solutions, "tested as false before => fail & backtrack", len(false_solutions))
    return False, None

  while True:
    # print("iteration", clauses)
    # done
    if len(clauses) == 0:
      print("Solved", solutions)
      return True, solutions

    # fail / backtrack
    for c in clauses:
      if len(c) == 0:
        print(solutions, "invalid branch")
        addFalseSolution(solutions)
        return False, None
        # return...

    # Unit propagation
    goToNext = False
    for c in clauses:
      if len(c) == 1:
        goToNext = True
        val = c[0]
        print(solutions,"Unit propagation", val)
        solutions.append(val)
        solveForVar(clauses, val)
        break

    if goToNext:
      continue

    # Pure Literal
    # anzahl treffer
    found = {}
    # vorzeichen -1 => -, 1 = +, 0 => beides => nicht gültig
    sign = {}
    for c in clauses:
      for v in c:
        key = abs(v)
        isPositive = v > 0
        if key in found:
          found[key] += 1
          if key in sign and sign[key] != -1 + isPositive * 2:
            # anderes vorzeichen als bereits gesehen -> nicht pure literal
            del sign[key]
        else:
          found[key] = 1
          sign[key] = -1 + isPositive * 2

    for num, isPositive in sign.items():
      val = int(num) * isPositive
      print(solutions, "found pure Literal", val)
      solutions.append(val)
      solveForVar(clauses, val)
      goToNext = True
      break
    if goToNext:
      continue

    # Split
    tested = {}
    for c in clauses:
      for v in c:
        key = abs(v)
        if key in tested:
          continue
        tested[key] = 1
        copyOne = deepcopy(clauses)
        print(solutions,"split", v)
        solveForVar(copyOne, v)
        isSolved, subSolutions = solve(copyOne, solutions+[v])
        if isSolved:
          solutions.append(v)
          return True, subSolutions
        
        v = -v
        copyTwo = deepcopy(clauses)
        print(solutions,"split", v)
        solveForVar(copyTwo, v)
        isSolved, subSolutions = solve(copyTwo, solutions+[v])
        if isSolved:
          solutions.append(v)
          return True, subSolutions

    # alles probiert, nichts geht => fail
    print(solutions, "no split worked => invalid branch")
    addFalseSolution(solutions)
    return False, None