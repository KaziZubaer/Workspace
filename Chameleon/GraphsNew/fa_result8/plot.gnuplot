set xrange [0:1000]
set yrange [0:130]
set xtics 0,200,1000
set ytics 0,20,120
set xlabel "Standard deviation weight"
set ylabel "FAR"
set grid
set key box opaque center top maxrows 10
set terminal pngcairo dashed font "Times roman, 16" size 1000,690
set output "fa_result8.png"
plot "result_0_000.txt" using 2:4 with lines lt 1 lw 2 title "0% of average weight",\
"result_0_010.txt" using 2:4 with lines lt 2 lw 2 title "10% of average weight",\
"result_0_020.txt" using 2:4 with lines lt 3 lw 2 title "20% of average weight",\
"result_0_030.txt" using 2:4 with lines lt 4 lw 2 title "30% of average weight",\
"result_0_040.txt" using 2:4 with lines lt 5 lw 2 title "40% of average weight",\
"result_0_050.txt" using 2:4 with lines lt 6 lw 2 title "50% of average weight",\
"result_0_060.txt" using 2:4 with lines lt 7 lw 2 title "60% of average weight",\
"result_0_070.txt" using 2:4 with lines lt 8 lw 2 title "70% of average weight",\
"result_0_080.txt" using 2:4 with lines lt 9 lw 2 title "80% of average weight",\
"result_0_090.txt" using 2:4 with lines lt 10 lw 2 title "90% of average weight",\
"result_0_100.txt" using 2:4 with lines lt 11 lw 2 title "100% of average weight"

