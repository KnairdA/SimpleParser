#include "tree.h"
#include "exceptions.h"

#include <sstream>
#include <limits>
#include <stack>

#include "utils.h"

namespace {
	using SimpleParser::Node;

	inline Node* topNode(const std::stack<Node*>& stack) {
		if ( !stack.empty() ) {
			return stack.top();
		} else {
			throw SimpleParser::operator_exception();
		}
	}

	inline Node* popNode(std::stack<Node*>& stack) {
		Node*const tmp = topNode(stack);
		stack.pop();

		return tmp;
	}
}

namespace SimpleParser {

Tree::Tree(
	const std::string& term,
	const ConstantMap* constants
):
	term_(term),
	constants_(constants),
	node_collection_(),
	root_node_{ this->buildTree(term) } { }

Tree::Tree(const std::string& term):
	Tree(term, nullptr) { }

double Tree::solve() const {
	return this->root_node_->solve();
}

std::string Tree::print() const {
	std::stringstream out;
	out.precision(std::numeric_limits<double>::digits10);

	out << "digraph \""
	    << this->term_
	    << "\" { node [shape = box]; ";

	std::size_t nodeIndex{};

	for ( auto&& node : this->node_collection_ ) {
		out << "node"
		    << nodeIndex 
		    << " [ label = \""
		    << node->print()
		    << "\"]; ";

		if ( node->rightChild != nullptr &&
		     node->leftChild  != nullptr ) {
			std::size_t childIndex{};

			for ( auto&& child : this->node_collection_ ) {
				if ( child.get() == node->leftChild  ||
				     child.get() == node->rightChild ) {
					out << "\"node"
					    << nodeIndex 
					    <<  "\" -> \"node"
					    << childIndex 
					    << "\"; ";
				}

				++childIndex;
			}
		}

		++nodeIndex;
	}

	out << " }";

	return out.str();
}

template <typename NodeType, typename... Args>
Node* Tree::addNode(Args&&... args) {
	this->node_collection_.emplace_back(
		std::make_unique<NodeType>(
			std::forward<Args>(args)...
		)
	);

	return this->node_collection_.back().get();
}

Node* Tree::buildTree(const std::string& term) {
	std::stack<Node*> operands;
	std::stack<Node*> operators;

	const std::vector<std::string> topElements{
		lexer(term)
	};

	for ( auto elementIterator = topElements.begin();
	      elementIterator     != topElements.end();
	      ++elementIterator ) {
		const std::string& element   = *elementIterator;
		const TokenType elementToken = determineToken(element.front());

		if ( elementToken   != TokenType::VALUE_NUMBER     &&
		     elementToken   != TokenType::VALUE_IDENTIFIER &&
		     element.size() == 1 ) {
			if ( operators.empty() ) {
				operators.push(
					this->addNode<OperatorNode>(elementToken)
				);
			} else {
				OperatorNode*const lastNode(
					static_cast<OperatorNode*>(topNode(operators))
				);

				if ( precedence(elementToken) > precedence(lastNode->token()) ) {
					operators.push( 
						this->addNode<OperatorNode>(elementToken)
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
			const std::vector<std::string> subElements{
				lexer(element)
			};

			if ( subElements.size() == 1 ) {
				switch ( determineToken(subElements.front()) ) {
					case TokenType::VALUE_NUMBER:
					case TokenType::OPERATOR_MINUS: {
						operands.push(this->addNode<OperandNode>(
							stringToDouble(subElements.front())
						));

						break;
					}
					case TokenType::VALUE_IDENTIFIER: {
						operands.push(this->addNode<ConstantNode>(
							subElements.front(),
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
