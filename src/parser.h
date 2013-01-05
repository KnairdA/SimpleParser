#ifndef PARSER_SRC_PARSER_H_
#define PARSER_SRC_PARSER_H_

#include <vector>
#include <stack>

#include "tree.h"

namespace SimpleParser {

class Parser {
	public:
		double calculate(std::string);
	
	private:
		int8_t getPriority(char);
		std::vector<std::string> lexer(std::string);
		Node* buildTree(Tree*, std::string);
};

}

#endif  // PARSER_SRC_PARSER_H_
