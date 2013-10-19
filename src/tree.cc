#include "tree.h"
#include "exceptions.h"

#include <sstream>
#include <limits>
#include <stack>

#include "utils.h"

namespace SimpleParser {

Node* topNodeFrom(const std::stack<Node*>& stack) {
	if ( !stack.empty() ) {
		return stack.top();
	} else {
		throw operator_exception();
	}
}

Tree::Tree(std::string term):
	term_(term) {
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
		<< std::endl;
	out << "{" << std::endl;
	out << "node [shape = box];" << std::endl;

	int i = 0;

	for ( auto it = this->node_collection_.begin();
	      it     != this->node_collection_.end();
	      ++it ) {
		out << "node"
			<< i
			<< " [ label = \""
			<< (*it)->print()
			<< "\"];"
			<< std::endl;

		if ( (*it)->getType() == NodeType::OPERATOR ) {
			for ( auto iter = this->node_collection_.begin();
			      iter     != this->node_collection_.end();
			      ++iter ) {
				if ( (*iter).get() == (*it)->leftChild ) {
					out << "\"node"
						<< i
						<< "\" -> \"node"
						<< (iter - this->node_collection_.begin())
						<< "\";"
						<< std::endl;
				}
				if ( (*iter).get() == (*it)->rightChild ) {
					out << "\"node"
						<< i
						<< "\" -> \"node"
						<< (iter - this->node_collection_.begin())
						<< "\";"
						<< std::endl;
				}
			}
		}

		i++;
	}
	
	out << "}" << std::endl;
	
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
	std::stack<Node*> operandStack;
	std::stack<Node*> operatorStack;

	std::vector<std::string> tmpLexer;
	std::vector<std::string> lexerOutput = lexer(term);

	for ( auto termIter = lexerOutput.begin();
	      termIter     != lexerOutput.end();
	      termIter++ ) {
		const std::string& currTerm = (*termIter);
		const TokenType token       = getTokenType(currTerm[0]);

		if ( token              != TokenType::VALUE_NUMBER     && 
		     token              != TokenType::VALUE_IDENTIFIER &&
		     (*termIter).size() == 1 ) {
			if ( !operatorStack.empty() ) {
				OperatorNode* lastNode(
					static_cast<OperatorNode*>(topNodeFrom(operatorStack))
				);

				if ( token > lastNode->getToken() ) {
					operatorStack.push( 
						this->addNode<OperatorNode>(nullptr, token)
					);
				} else {
					Node* currOperator = topNodeFrom(operatorStack);
					operatorStack.pop();

					currOperator->rightChild = topNodeFrom(operandStack);
					operandStack.pop();

					currOperator->leftChild  = topNodeFrom(operandStack);
					operandStack.pop();

					operandStack.push(currOperator);

					termIter--;
				}
			} else {
				operatorStack.push(
					this->addNode<OperatorNode>(nullptr, token)
				);
			}
		} else {
			tmpLexer = lexer(*termIter);

			if ( tmpLexer.size() == 1 ) {
				switch ( getTokenType(tmpLexer[0][0]) ) {
					case TokenType::VALUE_NUMBER: {
						double value;
						std::istringstream convertStream(tmpLexer[0]);
						convertStream >> value;

						operandStack.push(
							this->addNode<OperandNode>(nullptr, value)
						);

						break;
					}
					case TokenType::VALUE_IDENTIFIER: {
						operandStack.push(
							this->addNode<ConstantNode>(nullptr, tmpLexer[0])
						);

						break;
					}
					default: {
						throw operator_exception();
					}
				}
			} else {
				operandStack.push(
					buildTree(*termIter)
				);
			}
		}
	}

	while ( !operatorStack.empty() ) {
		OperatorNode *currOperator(
			static_cast<OperatorNode*>(topNodeFrom(operatorStack))
		);
		operatorStack.pop();

		currOperator->rightChild = topNodeFrom(operandStack);
		operandStack.pop();

		currOperator->leftChild  = topNodeFrom(operandStack);
		operandStack.pop();

		operandStack.push(currOperator);
	}

	return topNodeFrom(operandStack);
}

}
