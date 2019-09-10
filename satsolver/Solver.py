from copy import deepcopy

# optimization: only one loop if possible
def solveForVar(clauses, var):
  shouldDelete = [False] * len(clauses)
  for i, c in enumerate(clauses):
    if var in c:
      # mark resolved clauses for deletion
      shouldDelete[i] = True

    # er hat ein böses bsp [17,17,40], einmal remove entfernt nur das eine 17
    while -var in c:
      # remove unresolvable literals
      c.remove(-var)

  # reverse, because we delete from the end so that the indexes stay valid
  shouldDelete.reverse()
  cLen = len(clauses) - 1
  for i, should in enumerate(shouldDelete):
    # remove resolved clauses
    if should:
      clauses.remove(clauses[cLen - i])


def solve(clauses, solutions):
  while True:
    # print("iteration", clauses)
    # done
    if len(clauses) == 0:
      print("Solved", solutions)
      return True, solutions

    # fail / backtrack
    for c in clauses:
      if len(c) == 0:
        print("invalid branch")
        return False, None
        # return...

    # Unit propagation
    goToNext = False
    for c in clauses:
      if len(c) == 1:
        goToNext = True
        val = c[0]
        print("Unit propagation", val)
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
      print("found pure Literal", val)
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
        print("split", v)
        solveForVar(copyOne, v)
        isSolved, subSolutions = solve(copyOne, [])
        if isSolved:
          solutions.append(v)
          return True, solutions + subSolutions
        
        v = -v
        copyTwo = deepcopy(clauses)
        print("split", v)
        solveForVar(copyTwo, v)
        isSolved, subSolutions = solve(copyTwo, [])
        if isSolved:
          solutions.append(v)
          return True, solutions + subSolutions
        
