#ifndef PARSER_SRC_NODES_H_
#define PARSER_SRC_NODES_H_

#include <string>

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

}

#endif  // PARSER_SRC_NODES_H_
