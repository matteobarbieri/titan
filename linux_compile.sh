#!/bin/bash

#g++ src/main.cpp -o titan -Isrc -L. -ltcod -g -Wl,-rpath=.
g++ \
    src/*.cpp \
    src/components/*.cpp \
    src/map/*.cpp \
    src/map/generators/*.cpp \
    -o titan \
    -Isrc -L. -ltcod -g -Wl,-rpath=.
