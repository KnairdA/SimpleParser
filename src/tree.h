#ifndef PARSER_SRC_NODE_H_
#define PARSER_SRC_NODE_H_

#include <memory>
#include <string>
#include <vector>

#include "nodes.h"

namespace SimpleParser {

class Tree {
	public:
		explicit Tree(std::string);
		Tree(std::string, const ConstantMap*);

		double solve();
		std::string print();

	private:
		template <class NType, typename... Args>
		Node* addNode(Node**, Args&&... args);
		Node* buildTree(std::string);

		std::string term_;
		Node* root_node_;
		std::vector<std::unique_ptr<Node>> node_collection_;
		const ConstantMap* constants_;
};

}

#endif  // PARSER_SRC_NODE_H_
