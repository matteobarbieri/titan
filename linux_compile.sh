#!/bin/bash

#g++ src/main.cpp -o titan -Isrc -L. -ltcod -g -Wl,-rpath=.
g++ src/*.cpp -o titan -Isrc -L. -ltcod -g -Wl,-rpath=.
