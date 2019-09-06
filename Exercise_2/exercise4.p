fof(a1, axiom, (
  loves(peter,mary)
)).

fof(a2, axiom, (
  loves(mary,ben)
)).

fof(a3, axiom, (
  knows(john,mary,ben)
)).

fof(a4, axiom, (
  ~ knows(john,peter,mary)
)).

% die beiden aussagen loves und knows haben eine Beziehung, 
% die wir nicht formal darstellen können, daher haben wir sie nur umschrieben

% wenn X weiß, das Y Z liebt, dann ist es auch so, dass Y Z liebt
fof(a5, axiom, (
  ! [X,Y,Z] : (
    knows(X,Y,Z) => loves(Y,Z)
  )
)).

% wenn Y Z nicht liebt, dann kann es auch kein X geben, der weiß das Y Z liebt
fof(a5, axiom, (
  ! [X,Y,Z] : (
    ~ loves(Y,Z)=> ~ knows(X,Y,Z)
  )
)).

% SystemOnTPTP sagt Satisfiable