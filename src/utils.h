#ifndef PARSER_SRC_UTILS_H_
#define PARSER_SRC_UTILS_H_

#include <string>
#include <vector>

#include "nodes.h"

namespace SimpleParser {

enum class PrecedenceLevel : uint8_t {
	FIRST  = 1,
	SECOND = 2,
	THIRD  = 3,
	FOURTH = 4,
	FIFTH  = 5,
};

enum class TokenType {
	OPERATOR_PLUS,
	OPERATOR_MINUS,
	OPERATOR_DIVIDE,
	OPERATOR_MULTIPLY,
	OPERATOR_POWER,
	PARENTHESES_OPEN,
	PARENTHESES_CLOSE,
	VALUE_NUMBER,
	VALUE_IDENTIFIER,
};

TokenType determineToken(const char);
TokenType determineToken(const std::string&);
PrecedenceLevel precedence(TokenType);
std::vector<std::string> lexer(const std::string&);

double stringToDouble(const std::string&);

}

#endif  // PARSER_SRC_UTILS_H_
