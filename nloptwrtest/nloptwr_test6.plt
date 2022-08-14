#! /usr/bin/env gnuplot

set title 'Chain with weights'
set xlabel 'x [m]'
set ylabel 'h [m]'

# grid
set grid

# ranges
set autoscale                          # let gnuplot determine ranges (default)
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically

#set ytics 400
#set ytics (0,200,400,600,800,1000,1200)
#set y2tics (-100,0,100)
set mytics 4
set mxtics 5

# labels
# set label "boiling point" at 10, 212

# key/legend
# set key left bottom
set nokey     # no key

set terminal pdf colour 
set output 'nloptwr_test6.pdf'
# plot "nloptwr_test6.dat" using 2:3 with linespoints
plot "nloptwr_test6.dat" using 2:3 with lines 
# pause -1

