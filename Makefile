LIB_FILES  = src/nodes.cc src/tree.cc src/parser.cc
PROG_FILES = main.cc
TEST_FILES = test.cc

FLAGS = -std=c++11 -W -Wall -Wextra -pedantic

all: parser test

parser:	$(PROG_FILES) $(LIB_FILES)
	g++ -g -o bin/parser $(FLAGS) $(PROG_FILES) $(LIB_FILES) 

test: $(LIB_FILES) $(TEST_FILES)
	g++ -O3 -o bin/test -lgtest $(FLAGS) $(LIB_FILES) $(TEST_FILES)
	./bin/test

