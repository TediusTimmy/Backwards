#!/bin/bash -x

x86_64-w64-mingw32-g++.exe -s -O2 -std=c++17 -o BackRooms -I../SlowFloat -I../Backwards/include -I../Backway/include -Wall -Wextra -Wpedantic main.cpp MazeGen8.cpp ContextBuilder.cpp ./lib/Backway.a ./lib/Backwards.a ./lib/SlowFloat.a -luser32 -lgdi32 -lgdiplus -lopengl32 -lSHlwapi -ldwmapi -lstdc++fs -lwinmm -Wl,-subsystem,windows
#x86_64-w64-mingw32-g++.exe -s -O2 -std=c++17 -o BackRooms -I../SlowFloat -I../Backwards/include -I../Backway/include -Wall -Wextra -Wpedantic main.cpp MazeGen8.cpp ContextBuilder.cpp ./lib/Backway.a ./lib/Backwards.a ./lib/SlowFloat.a -luser32 -lgdi32 -lgdiplus -lopengl32 -lSHlwapi -ldwmapi -lstdc++fs -lwinmm
#x86_64-w64-mingw32-g++.exe -g -Og -std=c++17 -o BackRooms -I../SlowFloat -I../Backwards/include -I../Backway/include -Wall -Wextra -Wpedantic main.cpp MazeGen8.cpp ContextBuilder.cpp ./lib/Backway.a ./lib/Backwards.a ./lib/SlowFloat.a -luser32 -lgdi32 -lgdiplus -lopengl32 -lSHlwapi -ldwmapi -lstdc++fs -lwinmm -Wl,-subsystem,windows
#x86_64-w64-mingw32-g++.exe -g -Og -std=c++17 -o BackRooms -I../SlowFloat -I../Backwards/include -I../Backway/include -Wall -Wextra -Wpedantic main.cpp MazeGen8.cpp ContextBuilder.cpp ./lib/Backway.a ./lib/Backwards.a ./lib/SlowFloat.a -luser32 -lgdi32 -lgdiplus -lopengl32 -lSHlwapi -ldwmapi -lstdc++fs -lwinmm