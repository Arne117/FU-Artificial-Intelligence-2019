#!/bin/bash

#g++ --std=c++11 -Wall -march=native -o3 -pthread -o satsolver_gcc satsolver.cpp

clang++ --std=c++11 -Wall -march=native -o3 -pthread -o satsolver satsolver.cpp
clang++ --std=c++11 -Wall -march=native -o3 -pthread -g -o satsolver2 satsolver2.cpp
#-g für debug
