#include "tree.h"

#include <limits>

namespace SimpleParser {

OperandNode::OperandNode(double val) {
	this->value_ = val;
}

double OperandNode::solve() {
	return this->value_;
}

NodeType OperandNode::getType() {
	return OPERAND_NODE;
}

std::string OperandNode::print() {
	std::stringstream convertStream;
	convertStream.precision(std::numeric_limits<double>::digits10);

	convertStream << this->value_;

	return convertStream.str();
}

OperatorNode::OperatorNode(char op) {
	this->function_ = op;
}

double OperatorNode::solve() {
	switch ( this->function_ ) {
		case '*':
			return this->leftChild->solve() * this->rightChild->solve();
		case '/': {
			double rightChild = this->rightChild->solve();
			
			if ( rightChild != 0 ) {
				return this->leftChild->solve() / rightChild;
			}
			else {
				throw divide_exception();
			}
		}
		case '+':
			return this->leftChild->solve() + this->rightChild->solve();
		case '-':
			return this->leftChild->solve() - this->rightChild->solve();
		case '^':
			return std::pow( this->leftChild->solve(), this->rightChild->solve() );
	}
}

NodeType OperatorNode::getType() {
	return OPERATOR_NODE;
}

std::string OperatorNode::print() {
	return std::string(1, this->function_);
}

char OperatorNode::getFunction() {
	return this->function_;
}

Node* Tree::addOperand(Node **place, double value) {
	this->node_collection_.emplace_back(new OperandNode(value));

	if ( place != nullptr ) {
		*place = this->node_collection_.back().get();
	}

	return this->node_collection_.back().get();
}

Node* Tree::addOperator(Node **place, char oper) {
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
