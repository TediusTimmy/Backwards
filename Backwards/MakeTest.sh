#!/bin/sh

g++ -I../../include -I../../../SlowFloat -g --coverage -O0 -c -Wall -Wextra -Wpedantic *.cpp
