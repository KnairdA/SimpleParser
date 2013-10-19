#include "tree.h"
#include "exceptions.h"

#include <sstream>
#include <limits>
#include <stack>

#include "utils.h"

namespace SimpleParser {

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

		if ( (*it)->getType() == NodeType::OPERATOR_NODE ) {
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

Node* Tree::addOperand(Node** place, double value) {
	this->node_collection_.emplace_back(
		new OperandNode(value)
	);

	if ( place != nullptr ) {
		*place = this->node_collection_.back().get();
	}

	return this->node_collection_.back().get();
}

Node* Tree::addOperator(Node** place, char oper) {
	this->node_collection_.emplace_back(
		new OperatorNode(oper)
	);

	if ( place != nullptr ) {
		*place = this->node_collection_.back().get();
	}

	return this->node_collection_.back().get();
}

Node* topNodeFrom(const std::stack<Node*>& stack) {
	if ( !stack.empty() ) {
		return stack.top();
	} else {
		throw operator_exception();
	}
}

Node* Tree::buildTree(std::string term) {
	std::stack<Node*> operandStack;
	std::stack<Node*> operatorStack;

	std::vector<std::string> tmpLexer;
	std::vector<std::string> lexerOutput = lexer(term);

	int8_t priority;

	for ( auto termIter = lexerOutput.begin();
	      termIter     != lexerOutput.end();
	      termIter++ ) {
		std::string& currTerm = (*termIter);
		priority              = getPriority(currTerm[0]);

		if ( priority != -1 && (*termIter).size() == 1 ) {
			if ( !operatorStack.empty() ) {
				OperatorNode* lastNode(
					static_cast<OperatorNode*>(topNodeFrom(operatorStack))
				);

				if ( getPriority(lastNode->getFunction()) < priority ) {
					operatorStack.push( 
						this->addOperator(nullptr, currTerm[0])
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
					this->addOperator(nullptr, currTerm[0])
				);
			}
		} else {
			tmpLexer = lexer(*termIter);

			if ( tmpLexer.size() == 1 ) {
				double value;
				std::istringstream convertStream(tmpLexer[0]);
				convertStream >> value;

				operandStack.push(
					this->addOperand(nullptr,value)
				);
			} else if ( tmpLexer.size() > 1 ) {
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
