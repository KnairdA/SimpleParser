#include <iostream>
#include "parser.h"

int main(int argc, char *argv[])
{
	string inputTerm;
	
	std::cin >> inputTerm; // Example: 2.5*(2+3-(3/2+1))
	
	Parser *parser = new Parser();
	
	try {
		std::cout << parser->calculate(inputTerm, true).result << std::endl;
	}
	catch ( exception &e )
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}

	return 0;
}
