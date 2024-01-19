#!/bin/bash
[[ -d ./bin ]] && rm -rf ./bin
#cmake -DCMAKE_C_COMPILER=`which clang` -DCMAKE_CXX_COMPILER=`which clang++` -DCMAKE_BUILD_TYPE=Debug -S . -B bin -G Ninja
#cmake -DCMAKE_C_COMPILER=`which clang` -DCMAKE_CXX_COMPILER=`which clang++` -DCMAKE_BUILD_TYPE=Release -S . -B bin -G Ninja
#cmake -DCMAKE_C_COMPILER=`which gcc` -DCMAKE_CXX_COMPILER=`which g++` -DCMAKE_BUILD_TYPE=Debug -S . -B bin -G Ninja
cmake -DCMAKE_C_COMPILER=`which gcc` -DCMAKE_CXX_COMPILER=`which g++` -DCMAKE_BUILD_TYPE=Release -S . -B bin -G Ninja
