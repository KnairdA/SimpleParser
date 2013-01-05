#ifndef PARSER_SRC_NODE_H_
#define PARSER_SRC_NODE_H_

#include <vector>
#include <string>
#include <memory>

#include "nodes.h"

namespace SimpleParser {

class Tree {
	public:
		void setRoot(Node*);
		double solve();

		Node* addOperand(Node**, double);
		Node* addOperator(Node**, char);

		std::string print(std::string);

	private:
		Node* root_node_;
		std::vector<std::unique_ptr<Node>> node_collection_;
};

}

#endif  // PARSER_SRC_NODE_H_
