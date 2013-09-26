#include "utils.h"
#include "exceptions.h"

#include "tree.h"

namespace SimpleParser {

int8_t getPriority(char tmp) {
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

std::vector<std::string> lexer(std::string term) {
	std::string tmp;
	std::string tmpNum;
	std::vector<std::string> output;

	int8_t   priority     = 0;
	int8_t   lastPriority = 0;
	uint32_t level        = 0;

	for ( auto termIter = term.begin();
	      termIter     != term.end();
	      termIter++ ) {
		priority = getPriority(*termIter);

		if ( priority == -1 || ( termIter == term.begin() &&
		                         priority == 10 ) ) {
			if ( level > 0 ) {
				tmp += *termIter;
			} else {
				tmpNum += *termIter;
			}
		} else {
			if ( lastPriority == -1 && level == 0 ) {
				output.push_back(tmpNum);
				tmpNum.clear();
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
		output.push_back(tmpNum);
	} else if ( lastPriority != 90 ) {
		throw operator_exception();
	}

	if (level != 0) {
		throw parenthese_exception();
	}

	if ( lastPriority == 90 && output.size() == 1 ) {
		output = lexer(output[0]);
	}

	return output;
}

}
