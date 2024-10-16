#!/bin/bash

cd "$(dirname "$0")"

g++ main.cpp -o maze_solver -Wall -Wextra && ./maze_solver ../input/output_gen.txt