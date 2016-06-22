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


all: mpp bench
	

mpp:
	cd src && make -f MPP.make

#bench:
#	cd benchmark && make -f Benchmark.make

new_bench:
	cd new_benchmark && make -f Benchmark.make

run:
	./Benchmark
	

