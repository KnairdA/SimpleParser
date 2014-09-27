#include "utils.h"
#include "tree.h"
#include "exceptions.h"

#include <cctype>
#include <sstream>

namespace SimpleParser {

TokenType determineToken(const char tmp) {
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

TokenType determineToken(const std::string& tmp) {
	return determineToken(tmp.front());
}

PrecedenceLevel precedence(const TokenType token) {
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

std::vector<std::string> lexer(const std::string& term) {
	std::vector<std::string> resultBuffer;

	std::string levelBuffer;
	std::string numberBuffer;
	std::string identifierBuffer;

	TokenType   previousToken;
	uint32_t    level{0};

	for ( auto&& termIter = term.begin();
	      termIter        < term.end();
	      ++termIter ) {
		const TokenType token{ determineToken(*termIter) };

		if ( token    == TokenType::VALUE_NUMBER     ||
		     token    == TokenType::VALUE_IDENTIFIER ||
		   ( token    == TokenType::OPERATOR_MINUS   &&
		     termIter == term.begin() ) ) {
			if ( level > 0 ) {
				levelBuffer += *termIter;
			} else {
				switch ( token ) {
					case TokenType::VALUE_NUMBER:
					case TokenType::OPERATOR_MINUS: {
						numberBuffer += *termIter;

						break;
					}
					case TokenType::VALUE_IDENTIFIER: {
						identifierBuffer += *termIter;

						break;
					}
					default: {
						break;
					}
				}
			}
		} else {
			if ( level == 0 ) {
				switch ( previousToken ) {
					case TokenType::VALUE_NUMBER: {
						resultBuffer.push_back(numberBuffer);
						numberBuffer.clear();

						break;
					}
					case TokenType::VALUE_IDENTIFIER: {
						resultBuffer.push_back(identifierBuffer);
						identifierBuffer.clear();

						break;
					}
					default: {
						break;
					}
				}
			}

			switch ( token ) {
				case TokenType::PARENTHESES_OPEN: {
					if ( level > 0 ) {
						levelBuffer += *termIter;
					}

					++level;

					break;
				}
				case TokenType::PARENTHESES_CLOSE: {
					--level;

					if ( level == 0 ) {
						resultBuffer.push_back(levelBuffer);
						levelBuffer.clear();
					} else {
						levelBuffer += *termIter;
					}

					break;
				}
				default: {
					if ( level == 0 ) {
						const std::string helper{ *termIter };

						resultBuffer.push_back(helper);
					} else {
						levelBuffer += *termIter;
					}

					break;
				}
			}
		}

		previousToken = token;
	}

	switch ( previousToken ) {
		case TokenType::VALUE_NUMBER: {
			resultBuffer.push_back(numberBuffer);

			break;
		}
		case TokenType::VALUE_IDENTIFIER: {
			resultBuffer.push_back(identifierBuffer);

			break;
		}
		case TokenType::PARENTHESES_CLOSE: {
			break;
		}
		default: {
			throw operator_exception();
		}
	}

	if ( level != 0 ) {
		throw parenthese_exception();
	}

	if ( previousToken == TokenType::PARENTHESES_CLOSE &&
	     resultBuffer.size() == 1 ) {
		resultBuffer = lexer(resultBuffer.front());
	}

	return resultBuffer;
}

double stringToDouble(const std::string& str) {
	double value;
	std::istringstream convertStream(str);
	convertStream >> value;

	return value;
}

}
