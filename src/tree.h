#ifndef PARSER_SRC_NODE_H_
#define PARSER_SRC_NODE_H_

#include <memory>
#include <string>
#include <vector>

#include "nodes.h"

namespace SimpleParser {

class Tree {
	public:
		Tree(const std::string&, const ConstantMap*);
		explicit Tree(const std::string&);

		double solve()      const;
		std::string print() const;

	private:
		template <class NodeType, typename... Args>
		Node* addNode(Args&&... args);

		Node* buildTree(const std::string&);

		const std::string  term_;
		const ConstantMap* constants_;

		std::vector<std::unique_ptr<Node>> node_collection_;
		Node* const root_node_;
};

}

#endif  // PARSER_SRC_NODE_H_
