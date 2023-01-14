#! /usr/bin/env bash

# =========================================
# Call and visualization of nloptwr_test6
# =========================================

# test if gnupot is existent
/usr/bin/env gnuplot -e "quit" || (echo "The program 'gnuplot' is to be installed."; exit 1; )
[ $? -eq 0 ] && echo "OK: gnuplot found" || exit 1;

# create build folder
mkdir -p ../build

# go to build folder
cd ../build

# build the program from sources
# rm -rf ../build/*
cmake ..
cmake --build .

# call the program
./lib/tests/nloptwr_test6 119 0 1 L
./lib/tests/nloptwr_test6 119 1 1 L
./lib/tests/nloptwr_test6 119 2 1 L

# create a pdf file
../lib/tests/nloptwr_test6.plt

# display result
xdg-open nloptwr_test6.pdf
cd ../../lib/tests/

