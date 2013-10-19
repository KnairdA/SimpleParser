#include "nodes.h"
#include "exceptions.h"

#include <cmath>
#include <sstream>
#include <limits>

namespace SimpleParser {

OperandNode::OperandNode(double value):
	value_(value) { }

double OperandNode::solve() {
	return this->value_;
}

NodeType OperandNode::getType() {
	return NodeType::OPERAND;
}

std::string OperandNode::print() {
	std::stringstream convertStream;
	convertStream.precision(std::numeric_limits<double>::digits10);

	convertStream << this->value_;

	return convertStream.str();
}

OperatorNode::OperatorNode(TokenType token):
	operator_(token) { }

double OperatorNode::solve() {
	switch ( this->operator_ ) {
		case TokenType::OPERATOR_MULTIPLY: {
			return this->leftChild->solve() * this->rightChild->solve();
		}
		case TokenType::OPERATOR_PLUS: {
			return this->leftChild->solve() + this->rightChild->solve();
		}
		case TokenType::OPERATOR_MINUS: {
			return this->leftChild->solve() - this->rightChild->solve();
		}
		case TokenType::OPERATOR_POWER: {
			return std::pow(
				this->leftChild->solve(), this->rightChild->solve()
			);
		}
		case TokenType::OPERATOR_DIVIDE: {
			double rightChild = this->rightChild->solve();
			
			if ( rightChild != 0 ) {
				return this->leftChild->solve() / rightChild;
			}
			else {
				throw divide_exception();
			}
		}
		default: {
			throw operator_exception();
		}
	}
}

NodeType OperatorNode::getType() {
	return NodeType::OPERATOR;
}

std::string OperatorNode::print() {
	switch ( this->operator_ ) {
		case TokenType::OPERATOR_PLUS: {
			return std::string(1, '+');
		}
		case TokenType::OPERATOR_MINUS: {
			return std::string(1, '-');
		}
		case TokenType::OPERATOR_MULTIPLY: {
			return std::string(1, '*');
		}
		case TokenType::OPERATOR_DIVIDE: {
			return std::string(1, '/');
		}
		case TokenType::OPERATOR_POWER: {
			return std::string(1, '^');
		}
		default: {
			throw operator_exception();
		}
	}
}

TokenType OperatorNode::getToken() {
	return this->operator_;
}

}
