#include "parser.h"

namespace SimpleParser {

double calculate(std::string term) {
	return Tree(term).solve();
}

double calculate(std::string term, const ConstantMap* constants) {
	return Tree(term, constants).solve();
}

std::string print(std::string term) {
	return Tree(term).print();
}

std::string print(std::string term, const ConstantMap* constants) {
	return Tree(term, constants).print();
}

}
