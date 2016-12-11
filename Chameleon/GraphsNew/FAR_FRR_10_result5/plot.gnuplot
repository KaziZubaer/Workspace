set xrange [0:1000]
set yrange [0:130]
set xtics 0,200,1000
set ytics 0,20,120
set xlabel "Standard deviation weight"
set ylabel "Percentage of error"
set grid
set key box opaque
set terminal pngcairo dashed font "Times roman, 22" size 640,384
set output "FAR_FRR_10_result5.png"
plot "result_0_010.txt" using 2:4 with lines lt 14 lw 3 title " FAR",\
"result_0_010.txt" using 2:5 with lines lt 16 lw 3 title " FRR"
