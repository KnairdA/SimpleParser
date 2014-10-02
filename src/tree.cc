#include "tree.h"
#include "exceptions.h"

#include <boost/optional.hpp>

#include <sstream>
#include <limits>
#include <stack>

#include "utils.h"

namespace {
	using SimpleParser::Node;

	inline boost::optional<Node*> top(const std::stack<Node*> stack) {
		if ( !stack.empty() ) {
			return boost::make_optional<Node*>(
				stack.top()
			);
		} else {
			return boost::optional<Node*>();
		}
	}

	inline boost::optional<Node*> pop(std::stack<Node*>& stack) {
		if ( boost::optional<Node*> node{ top(stack) } ) {
			stack.pop();

			return node;
		} else {
			return node;
		}
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

		if ( node->hasChildren() ) {
			std::size_t childIndex{};

			for ( auto&& child : this->node_collection_ ) {
				if ( node->isParentOf(child.get()) ) {
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

template <
	typename    Type,
	typename... Args
>
typename std::add_pointer<Type>::type Tree::addNode(Args&&... args) {
	this->node_collection_.emplace_back(
		std::make_unique<Type>(
			std::forward<Args>(args)...
		)
	);

	return static_cast<typename std::add_pointer<Type>::type>(
		this->node_collection_.back().get()
	);
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
				operators.emplace(
					this->addNode<OperatorNode>(elementToken)
				);
			} else {
				if ( boost::optional<Node*> lastNode{ top(operators) } ) {
					OperatorNode*const lastOperator{
						static_cast<OperatorNode*const>(*lastNode)
					};
  
					if (   precedence(elementToken)
					     > precedence(lastOperator->token()) ) {
						operators.emplace( 
							this->addNode<OperatorNode>(elementToken)
						);
					} else {
						boost::optional<Node*> currentOperator{ pop(operators) };
						boost::optional<Node*> rightChild     { pop(operands)  };
						boost::optional<Node*> leftChild      { pop(operands)  };

						if ( currentOperator &&
						     rightChild      &&
						     leftChild ) {
							static_cast<OperatorNode*const>(
								*currentOperator
							)->setChildren(
								*rightChild,
								*leftChild
							);

							operands.emplace(*currentOperator);

							--elementIterator;
						} else {
							throw operator_exception();
						}
					}
				} else {
					throw operator_exception();
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
						operands.emplace(
							this->addNode<OperandNode>(
								stringToDouble(subElements.front())
							)
						);

						break;
					}
					case TokenType::VALUE_IDENTIFIER: {
						operands.emplace(
							this->addNode<ConstantNode>(
								subElements.front(),
								this->constants_
							)
						);

						break;
					}
					default: {
						throw operator_exception();
					}
				}
			} else {
				operands.emplace(
					buildTree(element)
				);
			}
		}
	}

	while ( !operators.empty() ) {
		boost::optional<Node*> currentOperator{ pop(operators) };
		boost::optional<Node*> rightChild     { pop(operands)  };
		boost::optional<Node*> leftChild      { pop(operands)  };

		if ( currentOperator &&
			 rightChild      &&
			 leftChild ) {
			static_cast<OperatorNode*const>(
				*currentOperator
			)->setChildren(
				*rightChild,
				*leftChild
			);

			operands.emplace(*currentOperator);
		} else {
			throw operator_exception();
		}
	}

	if ( boost::optional<Node*> rootNode{ pop(operands) } ) {
		return *rootNode;
	} else {
		throw operator_exception();
	}
}

}
