#include "parser.h"

int Parser::getPriority(char tmp)
{
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

vector<string>* Parser::lexer(string term)
{
	int priority = 0;
	int last_priority = 0;
	int level = 0;
	string tmp, tmp_num;
	
	vector<string> *output = new vector<string>();
	
	string::iterator termIter;
	
	for ( termIter = term.begin(); termIter != term.end(); termIter++ ) {
		priority = this->getPriority( *termIter );
		
		if ( priority == -1 ) {
			if ( level > 0 ) {
				tmp += *termIter;
			}
			else {
				tmp_num += *termIter;
			}
		}
		else {
			if ( last_priority == -1 && level == 0 ) {
				output->push_back( tmp_num );
				tmp_num = "";
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
						output->push_back( tmp );
						tmp = "";
					}
					else {
						tmp += *termIter;
					}
					break;
				}
				default: {				
					if ( level == 0 ) {
						output->push_back( boost::lexical_cast<string>(*termIter) );
					}
					else {
						tmp += *termIter;
					}
					break;
				}
			}
		}
		
		last_priority = priority;	
	}
	
	if ( last_priority == -1 ) {
		output->push_back( tmp_num );
	}
	else if ( last_priority != 90 ) {
		throw operator_exception();
	}
	
	if (level != 0) {
		throw parenthese_exception();
	}
	
	return output;
}

Node* Parser::buildTree(Tree **tree, string term)
{
	stack<Node*> *operandStack = new stack<Node*>();
	stack<Node*> *operatorStack = new stack<Node*>();
	
	int priority;
	Node *newNode;
	
	vector<string> *tmpLexer;
	
	vector<string> *lexerOutput = lexer(term);
	
	for ( vector<string>::iterator termIter = lexerOutput->begin(); termIter != lexerOutput->end(); termIter++ ) {
		priority = this->getPriority( (*termIter)[0] );
		
		if ( priority != -1 ) {
			if ( !operatorStack->empty() ) {
				OperatorNode *lastNode = static_cast<OperatorNode*>( operatorStack->top() );
			
				if ( this->getPriority( lastNode->function ) < priority ) {
					newNode = (*tree)->addOperator( NULL, (*termIter)[0] );
					operatorStack->push( newNode );
				}
				else {
					Node *currOperator = operatorStack->top();
					operatorStack->pop();
					
					currOperator->rightChild = operandStack->top();
					operandStack->pop();
					
					currOperator->leftChild = operandStack->top();
					operandStack->pop();
					
					operandStack->push( currOperator );
					
					termIter--;
				}
			}
			else {
				newNode = (*tree)->addOperator( NULL, (*termIter)[0] );
				operatorStack->push( newNode );
			}
		}
		else {
			tmpLexer = lexer( *termIter );
			
			if ( tmpLexer->size() == 1 ) {
				newNode = (*tree)->addOperand( NULL, boost::lexical_cast<double>(*termIter) );
				operandStack->push( newNode );
			}
			else if ( tmpLexer->size() > 1 ) {
				newNode = buildTree(tree, *termIter);
				operandStack->push( newNode );
			}
		}
	}
	
	while ( !operatorStack->empty() ) {
		OperatorNode *currOperator = static_cast<OperatorNode*>( operatorStack->top() );
		operatorStack->pop();
					
		currOperator->rightChild = operandStack->top();
		operandStack->pop();
					
		currOperator->leftChild = operandStack->top();
		operandStack->pop();
				
		operandStack->push( currOperator );
	}
	
	return operandStack->top();
}

ParserResult Parser::calculate(string term, bool getTreeAsDot)
{
	Tree *termTree = new Tree();
	termTree->root = buildTree(&termTree, term);
	
	ParserResult returnVal;
	
	returnVal.result = termTree->root->solve();
	
	if ( getTreeAsDot ) {
		returnVal.tree = termTree->print(term);
	}
	
	return returnVal;
}
