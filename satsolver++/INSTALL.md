# LinuSAT

von Abdullah Barhoum, Arne Rolf, Linus Helfmann, Linus Ververs

## Abhängigkeiten & bauen
benötigt clang++ mit c++11 unterstützung (oder alternativ g++ mit c+11 unterstützung) sowie pthread

bauen mit:
```
clang++ --std=c++11 -Wall -march=native -o3 -pthread -o satsolver satsolver.cpp
```
(oder clang++ durch g++ ersetzen)

## Ausführen
executable ausführen und pfad als erstes argument
```
./satsolver test.cnf
```