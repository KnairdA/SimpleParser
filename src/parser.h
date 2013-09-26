#ifndef PARSER_SRC_PARSER_H_
#define PARSER_SRC_PARSER_H_

#include <string>
#include <vector>

#include "tree.h"

namespace SimpleParser {

double calculate(std::string term) {
	return Tree(term).solve();
}

std::string exportTree(std::string term) {
	return Tree(term).print();
}

}

#endif  // PARSER_SRC_PARSER_H_
