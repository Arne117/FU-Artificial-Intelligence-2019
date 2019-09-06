Dear all,

our SAT solver competition will take place end of the next week or beginning of the week after.

What is the input format/Where are test problems?: Please use the same format as supported also by MiniSAT: https://dwheeler.com/essays/minisat-user-guide.html

Test problems can be found here: https://www.cs.ubc.ca/~hoos/SATLIB/benchm.html

See also the problem attached to this email.

See also here: http://www.satcompetition.org/2004/format-solvers2004.html

As small input problem would thus be 

p cnf 5 4
 1 -5 4 0
 -1 5 3 4 0
 -3 -4 0
 -1 -2 3 -4 -5 0
Explanation: see https://dwheeler.com/essays/minisat-user-guide.html

Organisation of the Competition:

(1) Each group will briefly present its solver: 5 min presentation

(2) Then we will see how each prover tackles (about) 5 five problems, which I will bring at the day; these problems will be given in 5 different files. Your prover will be called then like "your-prover inputFile1" and it should return SAT, UNSAT, (timeout, gave-up, etc.)

Looking forward to it, will be fun.

Best wishes,

  Christoph Benzmüller