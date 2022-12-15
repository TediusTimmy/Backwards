#!/bin/sh -x

./Clean.sh

cd ../src
g++ -Wall -Wextra -Wpedantic -s -O3 -c -I../../Backwards/include -I../../SlowFloat -I../include *.cpp

cd ../bin
g++ -o BackwayTest -Wall -Wextra -Wpedantic -s -O3 -I../../Backwards/External/googletest/include -I../../Backwards/include -I../../SlowFloat -I../include ../Tests/StdLibTest.cpp ../src/*.o ../../Backwards/External/googletest/lib/libgtest.a ../../Backwards/External/googletest/lib/libgtest_main.a ../lib/*.a
./BackwayTest.exe
