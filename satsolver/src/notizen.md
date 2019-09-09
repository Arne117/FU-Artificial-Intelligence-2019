### Schleife
Formel zum anfang "sauber" sein
` [[1,-5,4],[-1,5,3,4],[-3,-4]]`


Prüfen ob solved:
`[] == formel => solved`

Prüfen ob failed;
`[] in formel => Backtrack oder Fail`

Prüfen ob Unitpropagation (z.b. [x] in formel)

Prüfen ob Pure-Literal (z.b. x kommt nur in einer teilformel vor und -x gar nicht bzw. andersrum)

Falls nicht, dann split -> ist rekursiver funktionsaufruf
hier müsste mal alle möglichen splits aufrufen bis einer stimmt, falls keiner passt, dann backtrack oder fail

```python
if pureLiteralPossible:
  # pureLiteral stuff...
  continue
```

deep copy using
```python
import copy
b = copy.deepcopy(a)
```

Optimierungsideen:
  - am anfang zählen wie häufig welche varialben in +/- vorkommen
 (die summe evtl. zu laufzeit aktualisieren)
  - falls gleichviel +/- erstmal die großen formeln erfüllen
  - sortieren  nach Länge der einzelenen Arrays
