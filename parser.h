#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <vector>
#include <stack>
#include <exception>

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

class parenthese_exception: public std::exception {
	virtual const char* what() const throw() {
		return "Invalid parenthesized expression - check your input term.";
	}
};

class operator_exception: public std::exception {
	virtual const char* what() const throw() {
		return "Unexpected operator placement - check your input term.";
	}
};

}

#endif  // PARSER_PARSER_H_
