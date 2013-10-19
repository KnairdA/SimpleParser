#ifndef PARSER_SRC_NODES_H_
#define PARSER_SRC_NODES_H_

#include <map>
#include <string>

namespace SimpleParser {

enum class TokenType;
typedef std::map<std::string, double> ConstantMap;

enum class NodeType {
	OPERAND,
	OPERATOR,
	CONSTANT,
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

class ConstantNode: public Node {
	public:
		explicit ConstantNode(std::string, const ConstantMap*);

		virtual double solve();
		virtual NodeType getType();
		virtual std::string print();

	private:
		std::string identifier_;
		const ConstantMap* constants_;
};

}

#endif  // PARSER_SRC_NODES_H_
