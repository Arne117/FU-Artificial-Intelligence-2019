#!/bin/bash

{ time ./satsolver uf150-01.cnf; } |& grep real
{ time ./satsolver uf175-01.cnf; } |& grep real
#{ time ./satsolver uf200-01.cnf; } |& grep real
{ time ./satsolver uuf125-01.cnf; } |& grep real
{ time ./satsolver uuf150-01.cnf; } |& grep real
#{ time ./satsolver uuf175-01.cnf; } |& grep real