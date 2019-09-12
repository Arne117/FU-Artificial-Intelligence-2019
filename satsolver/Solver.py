from copy import deepcopy


def print2(solutions, *msg):
  return
  #print(solutions, *msg)
  #print(len(solutions), " ", *msg)

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

def solve(clauses, solutions):
  while True:
    # print2(solutions, "iteration", clauses)
    # done
    if len(clauses) == 0:
      print2(solutions, "Solved")
      return True, solutions

    goToNext = False

    # fail / backtrack
    for c in clauses:
      # fail / backtrack
      if len(c) == 0:
        print2(solutions, "invalid branch")
        return False, None

      # Unit propagation
      elif len(c) == 1:
        goToNext = True
        val = c[0]
        print2(solutions, "Unit propagation", val)
        solutions.append(val)
        solveForVar(clauses, val)
        break

    if goToNext:
      continue

    # Pure Literal
    # anzahl treffer
    found = {}
    maxv = 0
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

        # das v mit maximalem Vorkommen
        # oder das key mit max Vorkommen, da wir auch -v testen?
        # bei unsat ist nach key besser, bei sat nach v (Unterschied max. 10-20%)
        # im vergleich zu v für Split zufällig wählen:
        # bei unsat ist v konkret wählen gut (Faktor 1/2) (Unsat2)
        # bei sat ist es schlecht? (Faktor 5) (Sat4)
        if maxv == 0 or found[key] > found[maxv]:
          maxv = key

    for num, isPositive in sign.items():
      val = int(num) * isPositive
      print2(solutions, "found pure Literal", val)
      solutions.append(val)
      solveForVar(clauses, val)
      goToNext = True
      break
    if goToNext:
      continue

    # Split
    v = maxv # oder: clauses[0][0]
    copyOne = deepcopy(clauses)
    print2(solutions,"split", v)
    solveForVar(copyOne, v)
    isSolved, subSolutions = solve(copyOne, solutions+[v])
    if isSolved:
      solutions.append(v)
      return True, subSolutions
    v = -v
    copyTwo = deepcopy(clauses)
    print2(solutions,"split", v)
    solveForVar(copyTwo, v)
    isSolved, subSolutions = solve(copyTwo, solutions+[v])
    if isSolved:
      solutions.append(v)
      return True, subSolutions

    # falls wir v und -v getestet haben, dann müssen wir keine weiteren v's prüfen
    # da diese auch irgentwann v oder -v setzen oder wenn sie ohne v/-v auskommen,
    # müssten sie auch mit v/-v funktionieren
    # => ein durchgang reicht!
    print2(solutions, "split v/-v didn't work => invalid branch")
    return False, None