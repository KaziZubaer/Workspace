set xrange [0:13]
set yrange [0:130]
set xtics 0,2,12
set ytics 0,20,120
set xlabel "Standard deviation weight"
set ylabel "Percentage of error"
set grid
set key box opaque maxrows 10
set terminal pngcairo dashed font "Times roman, 14" size 640,384
set output "mismatch.png"
plot "result.txt" using 3:4 with lines lt 14 lw 3 title "  FAR",\
"result.txt" using 3:5 with lines lt 16 lw 3 title "  FRR"
