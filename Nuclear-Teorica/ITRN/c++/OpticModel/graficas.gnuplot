#!/usr/bin/env gnuplot
# plot_phase.gp ─ generate four PDFs from delta_vs_Elab.dat

# ------------------------------------------------------------------
# 1.  Read l and a from command line:  gnuplot -e "l=0;a=10" ...
# ------------------------------------------------------------------
if (!exists("l")) l = 0
if (!exists("a")) a = 10

# ------------------------------------------------------------------
# 2.  Build input/output paths
# ------------------------------------------------------------------
data_dir   = sprintf("data/datfiles_l=%g_a=%g", l, a)
data_file  = data_dir . "/delta_vs_Elab.dat"

img_dir    = sprintf("data/imagenesl=%g_a=%g", l, a)
# create folder if it does not exist (POSIX; adjust for Windows if needed)
system(sprintf("mkdir -p %s", img_dir))

# ------------------------------------------------------------------
# 3.  Common plot style
# ------------------------------------------------------------------
set style line 1 lw 2 pt 7
set grid
set xlabel "E_{lab} [MeV]"

# ------------------------------------------------------------------
# 4.  Plot 1: Real part
# ------------------------------------------------------------------
set terminal pdfcairo size 12cm,8cm
set output sprintf("%s/parte_real.pdf", img_dir)
set title sprintf("Parte Real del Phase Shift (l=%g, a=%g fm)", l, a)
set ylabel "δ_{real} [°]"
plot data_file using 1:2 with linespoints ls 1 notitle

# ------------------------------------------------------------------
# 5.  Plot 2: Imaginary part
# ------------------------------------------------------------------
set output sprintf("%s/parte_imaginaria.pdf", img_dir)
set title sprintf("Parte Imaginaria del Phase Shift (l=%g, a=%g fm)", l, a)
set ylabel "δ_{imag} [°]"
plot data_file using 1:3 with linespoints ls 1 notitle

# ------------------------------------------------------------------
# 6.  Plot 3: Cross-section
# ------------------------------------------------------------------
set output sprintf("%s/seccion_eficaz.pdf", img_dir)
set title sprintf("Sección Eficaz (l=%g, a=%g fm)", l, a)
set ylabel "σ [mb]"
set logscale y
plot data_file using 1:4 with linespoints ls 1 notitle
unset logscale y     # reset for next graph

# ------------------------------------------------------------------
# 7.  Plot 4: S-factor
# ------------------------------------------------------------------
set output sprintf("%s/factor_S.pdf", img_dir)
set title sprintf("Factor S (l=%g, a=%g fm)", l, a)
set ylabel "S(E) [MeV·mb]"
plot data_file using 1:5 with linespoints ls 1 notitle

unset output
print sprintf("Gráficas guardadas en  %s/", img_dir)

