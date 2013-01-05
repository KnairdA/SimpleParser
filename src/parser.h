#ifndef PARSER_SRC_PARSER_H_
#define PARSER_SRC_PARSER_H_

#include <string>
#include <vector>

#include "tree.h"

namespace SimpleParser {

double calculate(std::string);
std::string exportTree(std::string);

namespace {
	int8_t getPriority(char);
	std::vector<std::string> lexer(std::string);
	Node* buildTree(Tree*, std::string);
}

}

#endif  // PARSER_SRC_PARSER_H_
