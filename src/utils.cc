#include "utils.h"
#include "tree.h"
#include "exceptions.h"

#include <cctype>

namespace SimpleParser {

int8_t getPriority(char tmp) {
	if ( std::isalpha(tmp) ) {
		return 100;
	} else {
		switch ( tmp ) {
			case '-':
				return 10;
			case '+':
				return 10;
			case '/':
				return 20;
			case '*':
				return 20;
			case '^':
				return 30;
			case '(':
				return 90;
			case ')':
				return 90;
			case ',':
				return -1;
			default:
				return -1;
		}
	}
}

std::vector<std::string> lexer(std::string term) {
	std::string tmp;
	std::string tmpNumber;
	std::string tmpIdentifier;
	std::vector<std::string> output;

	int8_t   priority     = 0;
	int8_t   lastPriority = 0;
	uint32_t level        = 0;

	for ( auto termIter = term.begin();
	      termIter     != term.end();
	      termIter++ ) {
		priority = getPriority(*termIter);

		if ( priority == -1 || priority == 100 || ( termIter == term.begin() &&
		                                            priority == 10 ) ) {
			if ( level > 0 ) {
				tmp += *termIter;
			} else {
				if ( priority == -1 ) {
					tmpNumber += *termIter;
				} else if ( priority == 100 ) {
					tmpIdentifier += *termIter;
				}
			}
		} else {
			if ( level == 0 ) {
				if ( lastPriority == -1 ) {
					output.push_back(tmpNumber);
					tmpNumber.clear();
				} else if ( lastPriority == 100 ) {
					output.push_back(tmpIdentifier);
					tmpIdentifier.clear();
				}
			}

			switch ( *termIter ) {
				case '(': {
					if ( level > 0 ) {
						tmp += *termIter;
					}

					level++;

					break;
				}
				case ')': {
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

		lastPriority = priority;
	}

	if ( lastPriority == -1 ) {
		output.push_back(tmpNumber);
	} else if ( lastPriority == 100 ) {
		output.push_back(tmpIdentifier);
	} else if ( lastPriority != 90 ) {
		throw operator_exception();
	}

	if ( level != 0 ) {
		throw parenthese_exception();
	}

	if ( lastPriority == 90 && output.size() == 1 ) {
		output = lexer(output[0]);
	}

	return output;
}

}
