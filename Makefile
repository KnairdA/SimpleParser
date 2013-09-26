LIB_FILES  = src/nodes.cc src/tree.cc src/utils.cc
PROG_FILES = main.cc
TEST_FILES = test.cc

FLAGS  = -std=c++11 -W -Wall -Wextra -pedantic
PARSER = -o bin/parser $(FLAGS) $(PROG_FILES) $(LIB_FILES)

all: dev test

dev: $(PROG_FILES) $(LIB_FILES)
	g++ -g $(PARSER)

release: $(PROG_FILES) $(LIB_FILES)
	g++ -O3 $(PARSER)

test: $(LIB_FILES) $(TEST_FILES)
	g++ -O3 -o bin/test -lgtest $(TEST_FILES) $(FLAGS) $(LIB_FILES)
	./bin/test

