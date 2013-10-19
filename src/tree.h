#ifndef PARSER_SRC_NODE_H_
#define PARSER_SRC_NODE_H_

#include <vector>
#include <string>
#include <memory>

#include "nodes.h"

namespace SimpleParser {

class Tree {
	public:
		Tree(std::string);

		double solve();
		std::string print();

	private:
		Node* addOperand(Node**, double);
		Node* addOperand(Node**, std::string);
		Node* addOperator(Node**, char);
		Node* buildTree(std::string);

		std::vector<std::unique_ptr<Node>> node_collection_;
		Node* root_node_;
		std::string term_;
};

}

#endif  // PARSER_SRC_NODE_H_
