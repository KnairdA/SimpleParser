#include "parser.h"
#include "exceptions.h"

#include <stack>
#include <sstream>

namespace SimpleParser {

double calculate(std::string term) {
	Tree termTree;
	termTree.setRoot(
		buildTree(&termTree, term)
	);

	return termTree.solve();
}

std::string exportTree(std::string term) {
	Tree termTree;
	termTree.setRoot(
		buildTree(&termTree, term)
	);

	return termTree.print(term);
}

namespace {

int8_t getPriority(char tmp) {
	switch ( tmp ) {
		case '-':
			return 10;
		case '+':
			return 10;
		case '/':
			return 20;
		case '*':
			return 20;
		case '^':
			return 30;
		case '(':
			return 90;
		case ')':
			return 90;
		case ',':
			return -1;
		default:
			return -1;
	}
}

std::vector<std::string> lexer(std::string term) {
	std::string tmp;
	std::string tmpNum;
	std::vector<std::string> output;

	int8_t   priority     = 0;
	int8_t   lastPriority = 0;
	uint32_t level        = 0;

	for ( auto termIter = term.begin();
	      termIter     != term.end();
	      termIter++ ) {
		priority = getPriority(*termIter);

		if ( priority == -1 || ( termIter == term.begin() &&
		                         priority == 10 ) ) {
			if ( level > 0 ) {
				tmp += *termIter;
			} else {
				tmpNum += *termIter;
			}
		} else {
			if ( lastPriority == -1 && level == 0 ) {
				output.push_back(tmpNum);
				tmpNum.clear();
			}

			switch ( *termIter ) {
				case '(': {
					if ( level > 0 ) {
						tmp += *termIter;
					}

					level++;

					break;
				}
				case ')': {
					level--;

					if ( level == 0 ) {
						output.push_back(tmp);
						tmp.clear();
					} else {
						tmp += *termIter;
					}

					break;
				}
				default: {
					if ( level == 0 ) {
						std::string helper;
						helper = *termIter;

						output.push_back(helper);
					} else {
						tmp += *termIter;
					}

					break;
				}
			}
		}

		lastPriority = priority;
	}

	if ( lastPriority == -1 ) {
		output.push_back(tmpNum);
	} else if ( lastPriority != 90 ) {
		throw operator_exception();
	}

	if (level != 0) {
		throw parenthese_exception();
	}

	if ( lastPriority == 90 && output.size() == 1 ) {
		output = lexer(output[0]);
	}

	return output;
}

Node* buildTree(Tree *tree, std::string term) {
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
					static_cast<OperatorNode*>(operatorStack.top())
				);

				if ( getPriority(lastNode->getFunction()) < priority ) {
					operatorStack.push( 
						tree->addOperator(nullptr, currTerm[0])
					);
				} else {
					Node* currOperator = operatorStack.top();
					operatorStack.pop();

					currOperator->rightChild = operandStack.top();
					operandStack.pop();

					currOperator->leftChild  = operandStack.top();
					operandStack.pop();

					operandStack.push(currOperator);

					termIter--;
				}
			} else {
				operatorStack.push(
					tree->addOperator(nullptr, currTerm[0])
				);
			}
		} else {
			tmpLexer = lexer(*termIter);

			if ( tmpLexer.size() == 1 ) {
				double value;
				std::istringstream convertStream(tmpLexer[0]);
				convertStream >> value;

				operandStack.push(
					tree->addOperand(nullptr,value)
				);
			} else if ( tmpLexer.size() > 1 ) {
				operandStack.push(buildTree(tree, *termIter));
			}
		}
	}

	while ( !operatorStack.empty() ) {
		OperatorNode *currOperator(
			static_cast<OperatorNode*>(operatorStack.top())
		);
		operatorStack.pop();

		currOperator->rightChild = operandStack.top();
		operandStack.pop();

		currOperator->leftChild  = operandStack.top();
		operandStack.pop();

		operandStack.push(currOperator);
	}

	return operandStack.top();
}

}

}
