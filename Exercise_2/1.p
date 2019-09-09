fof(c1, axiom, (
  ? [X] : (
    livesDread(X) & (murdererOf(agatha) = X)
  )
)).

fof(c2, axiom, (
  ! [X]: (
    livesDread(X) <=> (
      (agatha = X) | (charles = X) | (butler = X)
    )
  )
)).


fof(c3, axiom, (
  ! [X, Y]: (
    (murdererOf(X) = Y) => (
      hates(Y, X) & notRicherThan(Y, X)
    )
  )
)).

fof(c4, axiom, (
  ! [X]: (
    hates(agatha, X) => (
      ~ hates(charles, X)
    )
  )
)).

fof(c5, axiom, (
  ! [X]: (
    (X != butler) => hates(agatha, X)
  )
)).

fof(c6, axiom, (
  ! [X]: (
    notRicherThan(X, agatha) => hates(butler, X)
  )
)).

fof(c7, axiom, (
  ! [X]: (
    hates(agatha, X) => hates(butler, X)
  )
)).

fof(c8, axiom, (
  ! [X] :
  ? [Y] : (
    ~ hates(X, Y)
  )
)).

fof(c9, axiom, (
  agatha != butler
)).

fof(c10, conjecture, (
  murdererOf(agatha) = agatha
)).
