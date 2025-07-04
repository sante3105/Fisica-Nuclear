# plot_states.gnuplot

# ————————————————————————————————————————
# 1) Set up terminal and labels
a = 5.0
set terminal pdfcairo enhanced font 'Arial,10' size 5in,3.75in
set output 'eigenstates.pdf'

set xrange [0:a]
set xlabel 'x (Å)'
set ylabel 'ψ(x) + E_j'
set title  'Potential & First Four Eigenstates'
set grid
set key outside

# ————————————————————————————————————————
# 2) Read energies from energies.dat
# energies.dat format: 
#   j   Energy(eV)
#   0   E0
#   1   E1
#   2   E2
#   3   E3

stats 'energies.dat' using ($1==0 ? $2 : 1/0) nooutput
E0 = STATS_mean
stats 'energies.dat' using ($1==1 ? $2 : 1/0) nooutput
E1 = STATS_mean
stats 'energies.dat' using ($1==2 ? $2 : 1/0) nooutput
E2 = STATS_mean
stats 'energies.dat' using ($1==3 ? $2 : 1/0) nooutput
E3 = STATS_mean

# ————————————————————————————————————————
# 3) Plot potential, shifted eigenfunctions, and horizontal energy lines

plot \
  'potential.dat' using 1:2                             with lines lw 2 title 'V(x)', \
  'state0.dat'    using 1:($2+E0)                      with lines title sprintf('ψ₀+E₀ (%.3f)', E0), \
  'state1.dat'    using 1:($2+E1)                      with lines title sprintf('ψ₁+E₁ (%.3f)', E1), \
  'state2.dat'    using 1:($2+E2)                      with lines title sprintf('ψ₂+E₂ (%.3f)', E2), \
  'state3.dat'    using 1:($2+E3)                      with lines title sprintf('ψ₃+E₃ (%.3f)', E3), \
  E0                                                   with lines dt 2 lw 1 title sprintf('E₀ = %.3f eV', E0), \
  E1                                                   with lines dt 2 lw 1 title sprintf('E₁ = %.3f eV', E1), \
  E2                                                   with lines dt 2 lw 1 title sprintf('E₂ = %.3f eV', E2), \
  E3                                                   with lines dt 2 lw 1 title sprintf('E₃ = %.3f eV', E3)

unset output  # Finalize the PDF file