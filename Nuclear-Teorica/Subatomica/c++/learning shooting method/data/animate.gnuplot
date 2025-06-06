set terminal gif animate delay 100
set output 'shooting.gif'
set title "Shooting Method Iteration"
set xlabel "x"
set ylabel "y(x)"
set xrange [0:4]
set yrange [-2:2]

do for [i=1:20] {
    plot sprintf("shoot%d.dat", i) with lines title sprintf("Iter %d", i)
}