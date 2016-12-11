set xrange [0:100]
set yrange [0:100]
set xtics 0,20,100
set ytics 0,20,100
set xlabel "Standard deviation weight"
set ylabel "Percentage of error"
set grid
set key box opaque
set terminal pngcairo dashed font "Times roman, 22" size 640,384
set output "Varying_only_AVG_SD0_FAR_FRR.png"
plot "result.txt" using 1:4 with lines lt 14 lw 3 title " FAR",\
"result.txt" using 1:5 with lines lt 16 lw 3 title " FRR"
