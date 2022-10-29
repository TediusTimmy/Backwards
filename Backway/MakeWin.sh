#!/bin/sh -x

cd ./src
x86_64-w64-mingw32-g++.exe -I../include -I../../Backwards/include -I../../SlowFloat -O3 -std=c++17  -c -Wall -Wextra -Wpedantic *.cpp
mv ./*.o ../obj

cd ../lib
ar -rcs Backway.a ../obj/*.o
