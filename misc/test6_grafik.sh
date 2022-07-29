#! /bin/bash
cd ../build
# rm -rf ../build/*
cmake .. 
cmake --build . 
./nloptwrtest/nloptwr_test06 
../misc/nloptwr_test6.plt 
xpdf nloptwr_test6.pdf 
cd ../misc

