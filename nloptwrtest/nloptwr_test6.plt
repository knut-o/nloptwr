#! /usr/bin/env gnuplot


# labels
set xlabel 'x [m]'
set ylabel 'h [m]'

# grid
set grid

# ranges
set autoscale                          # let gnuplot determine ranges (default)
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically

set mytics 4
set mxtics 5

set style line 1 linecolor rgb '#0060ad' linetype 1 linewidth 1 pointtype 7 pointsize 0.3
set style line 2 linecolor rgb '#ff000f' linetype 1 linewidth 1 pointtype 7 pointsize 0.3

set key center top

show xlabel
show ylabel

set terminal pdf colour
set output 'nloptwr_test6.pdf'

set title 'Chain with weights'
plot "nloptwr_test6_0.dat" using 2:3 with linespoints linestyle 1 title 'chain'

set title 'Chain with weights on ground (linear)'
plot "nloptwr_test6_1.dat" using 2:3 with linespoints linestyle 1 title 'chain', "nloptwr_test6_1.dat" using 2:4 with lines linestyle 2 title 'ground'

set title 'Chain with weights on ground (cos)'
plot "nloptwr_test6_2.dat" using 2:3 with linespoints linestyle 1 title 'chain', "nloptwr_test6_2.dat" using 2:4 with lines linestyle 2 title 'ground'

# pause -1
