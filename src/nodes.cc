#include "nodes.h"
#include "utils.h"
#include "tree.h"
#include "exceptions.h"

#include <cmath>
#include <cassert>
#include <sstream>
#include <limits>

namespace SimpleParser {

bool Node::hasChildren() const {
	return false;
}

bool Node::isParentOf(Node*const) const {
	return false;
}

OperandNode::OperandNode(double value):
	value_{value} { }

double OperandNode::solve() const {
	return this->value_;
}

std::string OperandNode::print() const {
	std::stringstream convertStream;
	convertStream.precision(std::numeric_limits<double>::digits10);

	convertStream << this->value_;

	return convertStream.str();
}

OperatorNode::OperatorNode(TokenType token):
	operator_{token} { }

double OperatorNode::solve() const {
	switch ( this->operator_ ) {
		case TokenType::OPERATOR_PLUS: {
			return this->left_->solve() + this->right_->solve();
		}
		case TokenType::OPERATOR_MINUS: {
			return this->left_->solve() - this->right_->solve();
		}
		case TokenType::OPERATOR_MULTIPLY: {
			return this->left_->solve() * this->right_->solve();
		}
		case TokenType::OPERATOR_DIVIDE: {
			return this->left_->solve() / this->right_->solve();
		}
		case TokenType::OPERATOR_POWER: {
			return std::pow(
				this->left_->solve(), this->right_->solve()
			);
		}
		default: {
			throw operator_exception();
		}
	}
}

std::string OperatorNode::print() const {
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

bool OperatorNode::hasChildren() const {
	return this->right_ != nullptr &&
	       this->left_  != nullptr;
}

bool OperatorNode::isParentOf(Node*const node) const {
	return this->right_ == node ||
	       this->left_  == node;
}

TokenType OperatorNode::token() const {
	return this->operator_;
}

void OperatorNode::setChildren(Node*const right, Node*const left) {
	assert(right != nullptr && left != nullptr);

	this->right_ = right;
	this->left_  = left;
}

ConstantNode::ConstantNode(
	const std::string& identifier,
	const ConstantMap* constants
):
	identifier_(identifier),
	constants_(constants) { }

double ConstantNode::solve() const {
	if ( this->constants_ != nullptr ) {
		try {
			return this->constants_->at(this->identifier_);
		}
		catch ( std::out_of_range& ) {
			throw identifier_exception();
		}
	} else {
		throw identifier_exception();
	}
}

std::string ConstantNode::print() const {
	return this->identifier_;
}

}
