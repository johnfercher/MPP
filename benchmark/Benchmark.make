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


CCX = g++ -std=c++11 -w



INCLUDES = -I -Iresults



#LIBRARIES = $(LIBRARIES_OMPL) $(LIBRARIES_BOOST) $(LIBRARIES_PROTO) $(LIBRARIES_GLUT) $(LIBRARIES_QT) -pthread -lm -w
LIBRARIES = -pthread -lm -w


SRC := $(shell find -name '*.cpp')
FILE_NAMES_SRC = $(SRC:.cpp=.o)

PROTOS := $(shell find -name '*.cc')
FILE_NAMES_PROTOS = $(PROTOS:.cc=.o)

FILE_NAMES = $(FILE_NAMES_SRC) $(FILE_NAMES_PROTOS) 



EXEC = ../Benchmark

.cpp.o:
	@$(CCX) $(INCLUDES) $(LIBRARIES) -Wall -Wformat -c -o $@ $< -w

.cc.o:
	@$(CCX) $(INCLUDES) $(LIBRARIES) -Wall -Wformat -c -o $@ $< -w

all: message_compiling $(EXEC)
	@echo Done ...
	
message_compiling:
	@echo Compiling Bechmark ...

message_cleaning:
	@echo Cleaning Becnhmark ...


$(EXEC): $(FILE_NAMES)
	@$(CCX) -o $(EXEC) $(FILE_NAMES) $(LIBRARIES) $(INCLUDES)

clean: message_cleaning
	@rm $(EXEC) $(FILE_NAMES)

proto:
	cd utils/protos && make -f protos.make

teste: 
	$(SRC)

