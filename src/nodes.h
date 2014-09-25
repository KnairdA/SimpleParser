#ifndef PARSER_SRC_NODES_H_
#define PARSER_SRC_NODES_H_

#include <map>
#include <string>

namespace SimpleParser {

enum class TokenType;
typedef std::map<std::string, double> ConstantMap;

class Node {
	public:
		virtual ~Node() {};

		virtual double solve()      const = 0;
		virtual std::string print() const = 0;

		Node* leftChild;
		Node* rightChild;

};

class OperatorNode: public Node {
	public:
		explicit OperatorNode(TokenType);

		TokenType token() const;

		virtual double solve()      const;
		virtual std::string print() const;


	private:
		const TokenType operator_;

};

class OperandNode: public Node {
	public:
		explicit OperandNode(double);

		virtual double solve()      const;
		virtual std::string print() const;

	private:
		const double value_;

};

class ConstantNode: public Node {
	public:
		ConstantNode(const std::string&, const ConstantMap*);

		virtual double solve()      const;
		virtual std::string print() const;

	private:
		const std::string identifier_;
		const ConstantMap* const constants_;

};

}

#endif  // PARSER_SRC_NODES_H_
