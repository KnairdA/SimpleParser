#include "utils.h"
#include "exceptions.h"

#include "tree.h"

namespace SimpleParser {

TokenType getTokenType(char tmp) {
	switch ( tmp ) {
		case '-':
			return TokenType::OPERATOR_PLUS;
		case '+':
			return TokenType::OPERATOR_MINUS;
		case '/':
			return TokenType::OPERATOR_DIVIDE;
		case '*':
			return TokenType::OPERATOR_MULTIPLY;
		case '^':
			return TokenType::OPERATOR_POWER;
		case '(':
			return TokenType::PARENTHESES_OPEN;
		case ')':
			return TokenType::PARENTHESES_CLOSE;
		case ',':
			return TokenType::VALUE_NUMBER;
		default:
			return TokenType::VALUE_NUMBER;
	}
}

std::vector<std::string> lexer(std::string term) {
	std::string tmp;
	std::string tmpNum;
	std::vector<std::string> output;
	TokenType token;
	TokenType lastToken;

	uint32_t level = 0;

	for ( auto termIter = term.begin();
	      termIter     != term.end();
	      termIter++ ) {
		token = getTokenType(*termIter);

		if ( token    == TokenType::VALUE_NUMBER   ||
		   ( token    == TokenType::OPERATOR_MINUS &&
		     termIter == term.begin() ) ) {
			if ( level > 0 ) {
				tmp += *termIter;
			} else {
				tmpNum += *termIter;
			}
		} else {
			if ( lastToken == TokenType::VALUE_NUMBER &&
			     level     == 0 ) {
				output.push_back(tmpNum);
				tmpNum.clear();
			}

			switch ( token ) {
				case TokenType::PARENTHESES_OPEN: {
					if ( level > 0 ) {
						tmp += *termIter;
					}

					level++;

					break;
				}
				case TokenType::PARENTHESES_CLOSE: {
					level--;

					if ( level == 0 ) {
						output.push_back(tmp);
						tmp.clear();
					} else {
						tmp += *termIter;
					}

					break;
				}
				default: {
					if ( level == 0 ) {
						std::string helper;
						helper = *termIter;

						output.push_back(helper);
					} else {
						tmp += *termIter;
					}

					break;
				}
			}
		}

		lastToken = token;
	}

	if ( lastToken == TokenType::VALUE_NUMBER ) {
		output.push_back(tmpNum);
	} else if ( lastToken != TokenType::PARENTHESES_CLOSE ) {
		throw operator_exception();
	}

	if ( level != 0 ) {
		throw parenthese_exception();
	}

	if ( lastToken     == TokenType::PARENTHESES_CLOSE &&
	     output.size() == 1 ) {
		output = lexer(output[0]);
	}

	return output;
}

}
