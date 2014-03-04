#include "tree.h"
#include "exceptions.h"

#include <sstream>
#include <limits>
#include <stack>

#include "utils.h"

namespace SimpleParser {

inline Node* topNode(const std::stack<Node*>& stack) {
	if ( !stack.empty() ) {
		return stack.top();
	} else {
		throw operator_exception();
	}
}

inline Node* popNode(std::stack<Node*>& stack) {
	Node*const tmp = topNode(stack);
	stack.pop();

	return tmp;
}

Tree::Tree(std::string term):
	term_(term),
	constants_(nullptr) {
	this->root_node_ = this->buildTree(term);
}

Tree::Tree(std::string term, const ConstantMap* constants):
	term_(term),
	constants_(constants) {
	this->root_node_ = this->buildTree(term);
}

double Tree::solve() {
	return this->root_node_->solve();
}

std::string Tree::print() {
	std::stringstream out;
	out.precision(std::numeric_limits<double>::digits10);

	out << "digraph \""
	    << this->term_
	    << "\""
	    << std::endl
	    << "{" << std::endl
	    << "node [shape = box];" 
	    << std::endl;

	size_t i{};

	for ( auto&& node : this->node_collection_ ) {
		out << "node"
		    << i
		    << " [ label = \""
		    << node->print()
		    << "\"];"
		    << std::endl;

		if ( node->type() == NodeType::OPERATOR ) {
			size_t j{};

			for ( auto&& child : this->node_collection_ ) {
				if ( child.get() == node->leftChild ) {
					out << "\"node"
					    << i
					    << "\" -> \"node"
					    << j
					    << "\";"
					    << std::endl;
				}

				if ( child.get() == node->rightChild ) {
					out << "\"node"
					    << i
					    << "\" -> \"node"
					    << j
					    << "\";"
					    << std::endl;
				}

				++j;
			}
		}

		++i;
	}

	out << "}"
	    << std::endl;

	return out.str();
}

template <typename NType, typename... Args>
Node* Tree::addNode(Node** place, Args&&... args) {
	this->node_collection_.emplace_back(
		new NType(std::forward<Args>(args)...)
	);

	if ( place != nullptr ) {
		*place = this->node_collection_.back().get();
	}

	return this->node_collection_.back().get();
}

Node* Tree::buildTree(std::string term) {
	std::stack<Node*> operands;
	std::stack<Node*> operators;

	std::vector<std::string> topElements(lexer(term));

	for ( auto elementIterator = topElements.begin();
	      elementIterator     != topElements.end();
	      ++elementIterator ) {
		const std::string& element   = *elementIterator;
		const TokenType elementToken = determineToken(element[0]);

		if ( elementToken      != TokenType::VALUE_NUMBER     &&
		     elementToken      != TokenType::VALUE_IDENTIFIER &&
		     element.size()    == 1 ) {
			if ( operators.empty() ) {
				operators.push(
					this->addNode<OperatorNode>(nullptr, elementToken)
				);
			} else {
				OperatorNode*const lastNode(
					static_cast<OperatorNode*>(topNode(operators))
				);

				if ( precedence(elementToken) > precedence(lastNode->token()) ) {
					operators.push( 
						this->addNode<OperatorNode>(nullptr, elementToken)
					);
				} else {
					Node*const currOperator  = popNode(operators);
					currOperator->rightChild = popNode(operands);
					currOperator->leftChild  = popNode(operands);

					operands.push(currOperator);

					--elementIterator;
				}
			}
		} else {
			std::vector<std::string> subElements(lexer(element));

			if ( subElements.size() == 1 ) {
				switch ( determineToken(subElements[0][0]) ) {
					case TokenType::VALUE_NUMBER:
					case TokenType::OPERATOR_MINUS: {
						operands.push(this->addNode<OperandNode>(
							nullptr,
							doubleToString(subElements[0])
						));

						break;
					}
					case TokenType::VALUE_IDENTIFIER: {
						operands.push(this->addNode<ConstantNode>(
							nullptr,
							subElements[0],
							this->constants_
						));

						break;
					}
					default: {
						throw operator_exception();
					}
				}
			} else {
				operands.push(
					buildTree(element)
				);
			}
		}
	}

	while ( !operators.empty() ) {
		OperatorNode*const currOperator(
			static_cast<OperatorNode*const>(popNode(operators))
		);

		currOperator->rightChild = popNode(operands);
		currOperator->leftChild  = popNode(operands);

		operands.push(currOperator);
	}

	return popNode(operands);
}

}
