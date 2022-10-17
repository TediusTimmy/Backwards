#!/bin/bash -x

x86_64-w64-mingw32-g++.exe -I../Backwards/include -I../SlowFloat -s -O3 -std=c++17 -o Program -Wall -Wextra -Wpedantic main.cpp *.a -luser32 -lgdi32 -lgdiplus -lopengl32 -lSHlwapi -ldwmapi -lstdc++fs -lwinmm -Wl,-subsystem,windows