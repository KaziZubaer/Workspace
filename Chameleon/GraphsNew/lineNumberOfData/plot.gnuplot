set xrange [3:14]
set yrange [0:100]
set xtics 3,1,14
set ytics 0,20,100
set xlabel "Number of training data"
set ylabel "Percentage of error"
set grid
unset key
set terminal pngcairo dashed font "Times roman, 16" size 600,384
set output "lineNumberOfData.png"
plot "result.txt" using 1:2 with lines lt 14 lw 2
