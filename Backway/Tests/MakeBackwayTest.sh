#!/bin/sh -x

./Clean.sh

cd ../src
g++ -Wall -Wextra -Wpedantic -g --coverage -O0 -c -I../../Backwards/include -I../../SlowFloat -I../include *.cpp

cd ../bin
g++ -o BackwayTest -Wall -Wextra -Wpedantic -g --coverage -O0 -I../../Backwards/External/googletest/include -I../../Backwards/include -I../../SlowFloat -I../include ../Tests/StdLibTest.cpp ../src/*.o ../../Backwards/External/googletest/lib/libgtest.a ../../Backwards/External/googletest/lib/libgtest_main.a ../lib/*.a
../../Backwards/External/lcov/bin/lcov --rc lcov_branch_coverage=1 --no-external --capture --initial --directory ../src --directory ../include --output-file Backway_Base.info
./BackwayTest.exe
../../Backwards/External/lcov/bin/lcov --rc lcov_branch_coverage=1 --no-external --capture --directory ../src --directory ../include --directory . --output-file Backway_Run.info
../../Backwards/External/lcov/bin/lcov --rc lcov_branch_coverage=1 --add-tracefile Backway_Base.info --add-tracefile Backway_Run.info --output-file Backway.info
mkdir cov
../../Backwards/External/lcov/bin/genhtml --rc lcov_branch_coverage=1 --branch-coverage --output-directory cov Backway.info
