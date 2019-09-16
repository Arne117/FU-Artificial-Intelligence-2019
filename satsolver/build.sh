#!/bin/bash

#g++ --std=c++11 -Wall -march=native -o3 -pthread -o satsolver_gcc satsolver.cpp

clang++ --std=c++11 -Wall -march=native -o3 -pthread -o satsolver satsolver.cpp
#-g für debug
