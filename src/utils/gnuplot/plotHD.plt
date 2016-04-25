#The MIT License (MIT)
#
#Copyright (c) 2016 Johnathan Fercher
#
#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:
#
#The above copyright notice and this permission notice shall be included in all
#copies or substantial portions of the Software.
#

set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1.5
set terminal png size 900,900 enhanced font "Helvetica,20"
set output 'pathHD.png'
plot 'path' with linespoints ls 1