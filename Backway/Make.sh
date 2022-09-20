#!/bin/sh -x

cd ./src
g++ -I../include -I../../Backwards/include -I../../SlowFloat -O3 -c -Wall -Wextra -Wpedantic *.cpp
mv ./*.o ../obj

cd ../lib
ar -rcs Backway.a ../obj/*.o
