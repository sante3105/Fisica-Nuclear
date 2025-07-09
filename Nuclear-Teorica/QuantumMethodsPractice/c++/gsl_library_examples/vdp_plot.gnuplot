# vdp_plot.gnuplot
# Gnuplot script to plot Van der Pol oscillator results

set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'vdp_time_series.png'

set title 'Van der Pol Oscillator: Time Series'
set xlabel 't'
set ylabel 'u(t), v(t)'
set grid
plot 'vdp.dat' using 1:2 with lines title 'u(t)', \
     ''       using 1:3 with lines title 'v(t)'

set output 'vdp_phase_space.png'
set title 'Phase Space'
set xlabel 'u(t)'
set ylabel 'v(t)'
set grid
plot 'vdp.dat' using 2:3 with lines title 'Phase Space'