
# optimization: only one loop if possible
def solveForVar(clauses, var):
  shouldDelete = [False] * len(clauses)
  for i, c in enumerate(clauses):
    if var in c:
      shouldDelete[i] = True

  shouldDelete.reverse()
  cLen = len(clauses) - 1
  for i, should in enumerate(shouldDelete):
    if should:
      clauses.remove(clauses[cLen - i])


def solve(clauses, solutions=[]):
  while True:
    # done
    if len(clauses) == 0:
      print("Solved", solutions)
      break

    # fail / backtrack
    for c in clauses:
      if len(c) == 0:
        print("invalid branch")
        raise "Not found"
        # return...

    # Unit propagation
    goToNext = False
    for c in clauses:
      if len(c) == 1:
        goToNext = True
        val = c[0]
        print("Unit propagation", val, clauses)
        solutions.append(val)
        solveForVar(clauses, val)
        break

    if goToNext:
      continue

    # Pure Literal
    found = {}
    for c in clauses:
      for v in c:
        key = abs(v)
        isPositive = v > 0
        if key in found:
          if found[key] != isPositive:
            del found[key]
        else:
          found[key] = isPositive
    for num, isPositive in found.items():
      val = int(num) * (1 if isPositive else -1)
      print("found pure Literal", val, clauses)
      solutions.append(val)
      solveForVar(clauses, val)
      goToNext = True
      break
    if goToNext:
      continue

    # Split
    raise "Not implemented"
