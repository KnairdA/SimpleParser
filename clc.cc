#include <iostream>
#include <limits>

#include "src/parser.h"

int main()
{
	std::string inputTerm;
	
	std::cin >> inputTerm;
	
	try {
		typedef std::numeric_limits<double> dbl;
		std::cout.precision(dbl::digits10);

		std::cout << SimpleParser::calculate(inputTerm) << std::endl;
	}
	catch ( std::exception &e )
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
