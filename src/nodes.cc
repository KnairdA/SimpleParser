#include "nodes.h"
#include "exceptions.h"

#include <cmath>
#include <sstream>
#include <limits>

namespace SimpleParser {

OperandNode::OperandNode(double val):
	value_(val) { }

double OperandNode::solve() {
	return this->value_;
}

NodeType OperandNode::getType() {
	return NodeType::OPERAND_NODE;
}

std::string OperandNode::print() {
	std::stringstream convertStream;
	convertStream.precision(std::numeric_limits<double>::digits10);

	convertStream << this->value_;

	return convertStream.str();
}

OperatorNode::OperatorNode(char op):
	function_(op) { }

double OperatorNode::solve() {
	switch ( this->function_ ) {
		case '*': {
			return this->leftChild->solve() * this->rightChild->solve();
		}
		case '/': {
			double rightChild = this->rightChild->solve();
			
			if ( rightChild != 0 ) {
				return this->leftChild->solve() / rightChild;
			}
			else {
				throw divide_exception();
			}
		}
		case '+': {
			return this->leftChild->solve() + this->rightChild->solve();
		}
		case '-': {
			return this->leftChild->solve() - this->rightChild->solve();
		}
		case '^': {
			return std::pow(
				this->leftChild->solve(), this->rightChild->solve()
			);
		}
		default: {
			throw operator_exception();
		}
	}
}

NodeType OperatorNode::getType() {
	return NodeType::OPERATOR_NODE;
}

std::string OperatorNode::print() {
	return std::string(1, this->function_);
}

char OperatorNode::getFunction() {
	return this->function_;
}

}
