#!/bin/bash

cd "$(dirname "$0")"

./maze_gen/compile_and_run.sh && ./maze_solver/compile_and_run.sh
