set title "Probabilidad de Lluvia en Bogotá"
set xlabel "Hora del día (t)"
set ylabel "Probabilidad"
set grid
set style fill transparent solid 0.3

plot "probabilidad_continua.txt" using 1:2 with lines title "Probabilidad continua" lc "red", \
     "probabilidad_discretizada_integral.txt" using 1:2 with steps title "Discretizada usando integral" lc "green", \
     "probabilidad_discretizada_valor_medio.txt" using 1:2 with steps title "Discretizada usando valor medio" lc "blue"