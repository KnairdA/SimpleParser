LIB_FILES = tree.cpp parser.cpp
PROG_FILES = main.cpp

all: parser

parser:	$(PROG_FILES) $(LIB_FILES)
	g++ -O2 -g -o parser $(PROG_FILES) $(LIB_FILES) 

