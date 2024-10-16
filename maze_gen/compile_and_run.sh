#!/bin/bash


cd "$(dirname "$0")"


g++ main.cpp -o maze_gen && ./maze_gen 70 10 ../input/output_gen.txt 5