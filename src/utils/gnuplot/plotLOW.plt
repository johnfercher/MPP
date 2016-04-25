set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1.5
set terminal png size 675,675 enhanced font "Helvetica,20"
set output 'pathLOW.png'
plot 'path' with linespoints ls 1