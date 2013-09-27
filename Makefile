CXX      = g++
CXXFLAGS = -std=c++11 -W -Wall -Wextra -pedantic -fpic

SRC_DIR  = src
BIN_DIR  = bin

LIB_SRC  = $(SRC_DIR)/nodes.cc \
           $(SRC_DIR)/tree.cc \
           $(SRC_DIR)/utils.cc \
           $(SRC_DIR)/parser.cc
LIB_OBJ  = $(subst .cc,.o,$(LIB_SRC))

TEST_SRC = test.cc
CLC_SRC  = clc.cc

all: lib test clc

lib:  $(BIN_DIR)/libSimpleParser.so;
test: $(BIN_DIR)/test;
clc:  $(BIN_DIR)/clc;

$(BIN_DIR)/libSimpleParser.so: $(LIB_OBJ)
	$(CXX) -shared -o $(BIN_DIR)/libSimpleParser.so $(LIB_OBJ) $(CXXFLAGS)

$(BIN_DIR)/test: $(BIN_DIR)/libSimpleParser.so
	$(CXX) -o $(BIN_DIR)/test $(TEST_SRC) -lgtest -L$(BIN_DIR)/ -lSimpleParser
	./$(BIN_DIR)/test

$(BIN_DIR)/clc: $(BIN_DIR)/libSimpleParser.so
	$(CXX) -o $(BIN_DIR)/clc $(CLC_SRC) -L$(BIN_DIR)/ -lSimpleParser

install: $(BIN_DIR)/libSimpleParser.so $(BIN_DIR)/clc 
	install -m 0755 $(BIN_DIR)/libSimpleParser.so /usr/lib
	install -m 0755 $(BIN_DIR)/clc /usr/bin

.PHONY: clean;
clean:
	rm -f $(LIB_OBJ)
	rm -f $(BIN_DIR)/*

depend: .depend

.depend: $(LIB_SRC)
	$(CXX) -M $(CXXFLAGS) $< > $@

include .depend
