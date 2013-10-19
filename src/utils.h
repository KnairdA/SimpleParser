#ifndef PARSER_SRC_UTILS_H_
#define PARSER_SRC_UTILS_H_

#include <string>
#include <vector>

#include "nodes.h"

namespace SimpleParser {

enum class TokenType : int8_t {
	OPERATOR_PLUS     = 10,
	OPERATOR_MINUS    = 11,
	OPERATOR_DIVIDE   = 20,
	OPERATOR_MULTIPLY = 21,
	OPERATOR_POWER    = 30,
	PARENTHESES_OPEN  = 90,
	PARENTHESES_CLOSE = 91,
	VALUE_NUMBER      = -1,
};

TokenType getTokenType(char);
std::vector<std::string> lexer(std::string);

}

#endif  // PARSER_SRC_UTILS_H_
