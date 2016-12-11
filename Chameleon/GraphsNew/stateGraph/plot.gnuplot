set yrange [0:105]
set ytics 0,10,100
set xlabel "State of the user"
set ylabel "Percentage of error"
unset key
set boxwidth 0.5
set style fill solid
set terminal pngcairo dashed font "Times roman, 12" size 600,384
set output "stateGraph.png"
plot "result.txt" using 1:3:xtic(2) with boxes lc 3
