#! /bin/bash
cd ../build
# rm -rf ../build/*
cmake .. 
cmake --build . 
./nloptwrtest/nloptwr_test06 
../nloptwrtest/nloptwr_test6.plt 
xpdf nloptwr_test6.pdf 
cd ../nloptwrtest/

