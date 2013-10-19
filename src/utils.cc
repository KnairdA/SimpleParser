#include "utils.h"
#include "tree.h"
#include "exceptions.h"

#include <cctype>

namespace SimpleParser {

TokenType getTokenType(char tmp) {
	if ( std::isalpha(tmp) ) {
		return TokenType::VALUE_IDENTIFIER;
	} else {
		switch ( tmp ) {
			case '-':
				return TokenType::OPERATOR_MINUS;
			case '+':
				return TokenType::OPERATOR_PLUS;
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
}

PrecedenceLevel getPrecedence(TokenType token) {
	switch ( token ) {
		case TokenType::VALUE_NUMBER:
		case TokenType::VALUE_IDENTIFIER: {
			return PrecedenceLevel::FIRST;
		}
		case TokenType::OPERATOR_MINUS:
		case TokenType::OPERATOR_PLUS: {
			return PrecedenceLevel::SECOND;
		}
		case TokenType::OPERATOR_DIVIDE:
		case TokenType::OPERATOR_MULTIPLY: {
			return PrecedenceLevel::THIRD;
		}
		case TokenType::OPERATOR_POWER: {
			return PrecedenceLevel::FOURTH;
		}
		case TokenType::PARENTHESES_OPEN:
		case TokenType::PARENTHESES_CLOSE: {
			return PrecedenceLevel::FIFTH;
		}
		default: {
			return PrecedenceLevel::FIRST;
		}
	}
}

std::vector<std::string> lexer(std::string term) {
	std::string tmp;
	std::string tmpNumber;
	std::string tmpIdentifier;
	std::vector<std::string> output;
	TokenType token;
	TokenType lastToken;

	uint32_t level = 0;

	for ( auto termIter = term.begin();
	      termIter     != term.end();
	      termIter++ ) {
		token = getTokenType(*termIter);

		if ( token    == TokenType::VALUE_NUMBER     ||
		     token    == TokenType::VALUE_IDENTIFIER ||
		   ( token    == TokenType::OPERATOR_MINUS   &&
		     termIter == term.begin() ) ) {
			if ( level > 0 ) {
				tmp += *termIter;
			} else {
				if ( token == TokenType::VALUE_NUMBER ||
				     token == TokenType::OPERATOR_MINUS ) {
					tmpNumber += *termIter;
				} else if ( token == TokenType::VALUE_IDENTIFIER ) {
					tmpIdentifier += *termIter;
				}
			}
		} else {
			if ( level == 0 ) {
				if ( lastToken == TokenType::VALUE_NUMBER ) {
					output.push_back(tmpNumber);
					tmpNumber.clear();
				} else if ( lastToken == TokenType::VALUE_IDENTIFIER ) {
					output.push_back(tmpIdentifier);
					tmpIdentifier.clear();
				}
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
		output.push_back(tmpNumber);
	} else if ( lastToken == TokenType::VALUE_IDENTIFIER ) {
		output.push_back(tmpIdentifier);
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
