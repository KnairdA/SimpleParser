#ifndef PARSER_SRC_NODES_H_
#define PARSER_SRC_NODES_H_

#include <string>

#include "utils.h"

namespace SimpleParser {

enum class NodeType {
	OPERAND,
	OPERATOR,
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
		explicit OperatorNode(TokenType);

		virtual double solve();
		virtual NodeType getType();
		virtual std::string print();

		TokenType getToken();

	private:
		TokenType operator_;
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
