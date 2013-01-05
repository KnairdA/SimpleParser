#include <iostream>
#include <limits>
#include "parser.h"

int main(int argc, char *argv[])
{
	std::string inputTerm;
	
	std::cin >> inputTerm; // Example: 2.5*(2+3-(3/2+1))
	
	SimpleParser::Parser parser;
	
	try {
		typedef std::numeric_limits<double> dbl;
		std::cout.precision(dbl::digits10);

		std::cout << parser.calculate(inputTerm) << std::endl;
	}
	catch ( std::exception &e )
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
