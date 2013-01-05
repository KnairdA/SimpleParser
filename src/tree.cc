#include "tree.h"
#include "exceptions.h"

#include <sstream>
#include <limits>

namespace SimpleParser {

void Tree::setRoot(Node* root) {
	this->root_node_ = root;
}

double Tree::solve() {
	return this->root_node_->solve();
}

Node* Tree::addOperand(Node** place, double value) {
	this->node_collection_.emplace_back(new OperandNode(value));

	if ( place != nullptr ) {
		*place = this->node_collection_.back().get();
	}

	return this->node_collection_.back().get();
}

Node* Tree::addOperator(Node** place, char oper) {
	this->node_collection_.emplace_back(new OperatorNode(oper));

	if ( place != nullptr ) {
		*place = this->node_collection_.back().get();
	}

	return this->node_collection_.back().get();
}

std::string Tree::print(std::string term) {
	std::stringstream out;
	out.precision(std::numeric_limits<double>::digits10);

	out << "digraph \"" << term << "\"" << std::endl
		<< "{"  << std::endl
		<< "node [shape = box];" << std::endl;

	int i = 0;

	for ( auto it = this->node_collection_.begin(); it != this->node_collection_.end(); ++it ) {
		out << "node" << i << " [ label = \"" << (*it)->print() << "\"];" << std::endl;

		if ( (*it)->getType() == OPERATOR_NODE ) {
			for ( auto iter = this->node_collection_.begin(); iter != this->node_collection_.end(); ++iter ) {
				if ( (*iter).get() == (*it)->leftChild ) {
					out << "\"node" << i << "\" -> \"node" << (iter - this->node_collection_.begin()) << "\";" << std::endl;
				}
				if ( (*iter).get() == (*it)->rightChild ) {
					out << "\"node" << i << "\" -> \"node" << (iter - this->node_collection_.begin()) << "\";" << std::endl;
				}
			}
		}

		i++;
	}
	
	out << "}" << std::endl;
	
	return out.str();
}

}
