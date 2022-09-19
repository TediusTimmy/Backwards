#!/bin/sh -x

cd ./src/Types
x86_64-w64-mingw32-g++.exe -I../../include -I../../../SlowFloat -O3 -std=c++17 -c -Wall -Wextra -Wpedantic *.cpp
mv ./*.o ../../obj

cd ../Input
x86_64-w64-mingw32-g++.exe -I../../include -I../../../SlowFloat -O3 -std=c++17 -c -Wall -Wextra -Wpedantic *.cpp
mv ./*.o ../../obj

cd ../Engine
x86_64-w64-mingw32-g++.exe -DM_PI=3.141592653589793238462643383279502884L -I../../include -I../../../SlowFloat -O3 -std=c++17 -c -Wall -Wextra -Wpedantic *.cpp
mv ./*.o ../../obj

cd ../Parser
x86_64-w64-mingw32-g++.exe -I../../include -I../../../SlowFloat -O3 -std=c++17 -c -Wall -Wextra -Wpedantic *.cpp
mv ./*.o ../../obj

cd ../../bin
ar -rcs Backwards.a ../obj/*.o
