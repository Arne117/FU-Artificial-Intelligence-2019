% da wir shall/should nicht abbilden können, nehmen wir an das es immer folgt
% wir haben weiterhin einiges vereinfacht, da selbst diese kurze Version das Problem zeigt

fof(a1, axiom, (
  ! [X]: (
    personalData(X) => processLawfully(X)
  )
)).

fof(a2, axiom, (
  ! [X]: (
    processLawfully(X) => keepData(X)
  )
)).

fof(a3, axiom, (
  ! [X]: (
    (~processLawfully(X) & personalData(X)) => eraseData(X)
  )
)).

fof(a4, axiom, (
  ? [X]: (
    ~processLawfully(X) & personalData(X)
  )
)).

% SystemOnTPTP sagt Proof found, Unsatisfiable
% da A4 wiederspricht A1, denn persöhnliche Daten sollten gesetzkonform verarbeitet werden, 
% wir haben aber in A4 gesagt, das es Daten gibt wo das nicht der Fall ist