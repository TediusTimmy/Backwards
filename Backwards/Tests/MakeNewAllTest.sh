#!/bin/sh -x

./Clean.sh

cd ../src/Types
../../MakeTest.sh
mv ./*.o ../../obj

cd ../Input
../../MakeTest.sh
mv ./*.o ../../obj

cd ../Engine
../../MakeTest.sh
mv ./*.o ../../obj

cd ../../bin
g++ -o AllTest -Wall -Wextra -Wpedantic --coverage -O0 -I../External/googletest/include -I../include ../Tests/ExpressionTest.cpp ../Tests/LexerTest.cpp ../Tests/StatementTest.cpp ../Tests/StdLibTest.cpp ../Tests/TypesTest.cpp ../obj/*.o ../External/googletest/lib/libgtest.a ../External/googletest/lib/libgtest_main.a
../External/lcov2/bin/lcov --rc lcov_branch_coverage=1 --no-external --capture --initial --filter line --filter branch --directory ../src --directory ../include --output-file All_Base.info
./AllTest.exe
../External/lcov2/bin/lcov --rc lcov_branch_coverage=1 --no-external --capture --filter line --filter branch --directory ../src --directory ../include --directory . --output-file All_Run.info
../External/lcov2/bin/lcov --rc lcov_branch_coverage=1 --add-tracefile All_Base.info --add-tracefile All_Run.info --output-file All.info
mkdir cov
../External/lcov2/bin/genhtml --rc lcov_branch_coverage=1 --branch-coverage --filter line --filter branch --output-directory cov All.info
