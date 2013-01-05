#ifndef PARSER_NODE_H_
#define PARSER_NODE_H_

#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <cmath>

namespace SimpleParser {

enum NodeType {
	OPERAND_NODE,
	OPERATOR_NODE,
};

class Node {
	public:
		virtual ~Node() {};

		virtual double solve()      = 0;
		virtual NodeType getType()  = 0;
		virtual std::string print() = 0;
		
		Node* leftChild;
		Node* rightChild;
};

class OperatorNode: public Node {
	public:
		explicit OperatorNode(char);

		virtual double solve();
		virtual NodeType getType();
		virtual std::string print();

		char getFunction();

	private:
		char function_;
};


class OperandNode: public Node {
	public:
		explicit OperandNode(double);

		virtual double solve();
		virtual NodeType getType();
		virtual std::string print();

	private:
		double value_;
};

class Tree {
	public:
		Node* root;

		Node* addOperand(Node**, double);
		Node* addOperator(Node**, char);

		std::string print(std::string);

	private:
		std::vector<std::unique_ptr<Node>> node_collection_;
};

class divide_exception: public std::exception {
	virtual const char* what() const throw()
	{
		return "A divison through zero had to be prevented by the parser - check your input term.";
	}
};

}

#endif  // PARSER_NODE_H_
