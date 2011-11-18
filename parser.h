#include <stack>
#include <string>
#include <sstream>
#include <exception>
#include <boost/lexical_cast.hpp>

#include "tree.h"

struct ParserResult 
{
	double result;
	string tree;	
};

class Parser
{
	public:
		ParserResult calculate(string, bool);
	
	private:
	 	int getPriority(char);
	 	vector<string>* lexer(string);
	 	Node* buildTree(Tree**, string);
};

class parenthese_exception: public exception
{
	virtual const char* what() const throw()
	{
		return "Invalid parenthesized expression - check your input term.";
	}
};

class operator_exception: public exception
{
	virtual const char* what() const throw()
	{
		return "Unexpected operator placement - check your input term.";
	}
};
