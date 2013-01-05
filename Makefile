LIB_FILES  = tree.cpp parser.cpp
PROG_FILES = main.cpp
TEST_FILES = test.cpp

all: parser test

parser:	$(PROG_FILES) $(LIB_FILES)
	g++ -std=c++11 -g -o bin/parser $(PROG_FILES) $(LIB_FILES) 

test: $(LIB_FILES) $(TEST_FILES)
	g++ -std=c++11 -O3 -o bin/test -lgtest $(LIB_FILES) $(TEST_FILES)
	./bin/test

