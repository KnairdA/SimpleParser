#include <iostream>
#include <limits>

#include "src/parser.h"

typedef std::numeric_limits<double> dbl;

int main() {
	std::string inputTerm;
	std::cout.precision(dbl::digits10);
	
	while ( std::cin >> inputTerm ) {
		try {
			std::cout << SimpleParser::calculate(inputTerm) << std::endl;
		}
		catch ( std::exception &e ) {
			std::cerr << e.what() << std::endl;
		}
	}

	return 0;
}
