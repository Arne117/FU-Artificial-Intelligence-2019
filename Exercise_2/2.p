fof(c1, axiom, (
  ! [X] :
  ? [Y] : (
    isStudent(X) => (
      isCourse(Y) & enrolled(X, Y)
    )
  )
)).

fof(c2, axiom, (
  ! [Z] :
  ? [Y] : (
    isProfessor(Z) => (
      isCourse(Y) & teaches(Z, Y)
    )
  )
)).

fof(c3, axiom, (
  ! [Y] :
  ? [X] : (
    isCourse(Y) => (
      isStudent(X) & enrolled(X, Y)
    )
  )
)).

fof(c4, axiom, (
  ! [Y] :
  ? [Z] : (
    isCourse(Y) => (
      isProfessor(Z) & teaches(Z, Y)
    )
  )
)).


fof(c5, axiom, (
  ! [C] :
  %for all courses
  ! [Y] : (
    (
      coordinates(C, Y) & isCourse(Y)
    )  => (
      teaches(C, Y)
    )
  )
)).


fof(c6, axiom, (
  ! [X, Y, Z] : (
    (
      isStudent(X) & isCourse(Y) & isProfessor(Z) & enrolled(X, Y) & teaches(Z, Y)
    ) => (
      teaches(Z, X)
    )
  )
)).

fof(c61, axiom, (
  ! [Z, Y]: (
    teaches(Z, Y) => isProfessor(Z)
  )
)).

fof(c7, axiom, (
  isCourse(course) & enrolled(micheal, course) & coordinates(victor, course) & isStudent(micheal)
)).


fof(c8, conjecture, (
  teaches(victor, micheal)
)).
