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


CCX = g++ -w



INCLUDES = -Isrc -Isrc/control -Isrc/utils -Isrc/utils/protos -Isrc/utils/includes -I/usr/include/qt4



LIBRARIES_OMPL = `pkg-config --cflags --libs ompl` 
LIBRARIES_BOOST = -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread
LIBRARIES_QT = -lQtNetwork -lQtCore
LIBRARIES_GLUT = -lGLU -lglut -lGL
LIBRARIES_PROTO = `pkg-config --cflags --libs protobuf`

LIBRARIES = $(LIBRARIES_OMPL) $(LIBRARIES_BOOST) $(LIBRARIES_PROTO) $(LIBRARIES_GLUT) $(LIBRARIES_QT) -pthread -lm -w



SRC := $(shell find -name '*.cpp')
FILE_NAMES_SRC = $(SRC:.cpp=.o)

PROTOS := $(shell find -name '*.cc')
FILE_NAMES_PROTOS = $(PROTOS:.cc=.o)

FILE_NAMES = $(FILE_NAMES_SRC) $(FILE_NAMES_PROTOS) 



EXEC = MPP

.cpp.o:
	@$(CCX) $(INCLUDES) $(LIBRARIES) -Wall -Wformat -c -o $@ $< -w

.cc.o:
	@$(CCX) $(INCLUDES) $(LIBRARIES) -Wall -Wformat -c -o $@ $< -w

all: message_compiling $(EXEC)
	@echo Done ...
	
message_compiling:
	@echo Compiling MPP ...

message_cleaning:
	@echo Cleaning MPP ...

run:
	./MPP

$(EXEC): $(FILE_NAMES)
	@$(CCX) -o $(EXEC) $(FILE_NAMES) $(LIBRARIES) $(INCLUDES)

clean: message_cleaning
	@rm $(EXEC) $(FILE_NAMES)

proto:
	cd utils/protos && make -f protos.make

teste: 
	$(SRC)

