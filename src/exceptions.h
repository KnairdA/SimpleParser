#ifndef PARSER_SRC_EXCEPTIONS_H_
#define PARSER_SRC_EXCEPTIONS_H_

#include <exception>

namespace SimpleParser {

class parenthese_exception: public std::exception {
	virtual const char* what() const throw() {
		return "Invalid parenthesized expression - check your input term.";
	}
};

class operator_exception: public std::exception {
	virtual const char* what() const throw() {
		return "Unexpected operator placement - check your input term.";
	}
};

class divide_exception: public std::exception {
	virtual const char* what() const throw()
	{
		return "A divison through zero had to be prevented by the parser - check your input term.";
	}
};

class identifier_exception: public std::exception {
	virtual const char* what() const throw()
	{
		return "Identifier could not be correctly resolved.";
	}
};

}

#endif  // PARSER_SRC_EXCEPTIONS_H_
