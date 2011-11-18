#include "tree.h"
#include <math.h>

#include <boost/lexical_cast.hpp>

Node::Node()
{

}

double Node::solve()
{
	switch (this->type) {
		case OPERAND_NODE: {
			OperandNode *tmp = static_cast<OperandNode*>( this );
			return tmp->solve();
		}
		case OPERATOR_NODE: {
			OperatorNode *tmp = static_cast<OperatorNode*>( this );
			return tmp->solve();
		}
	}
}

OperandNode::OperandNode()
{
	this->type = OPERAND_NODE;
}

double OperandNode::solve()
{
	return this->value;
}

OperatorNode::OperatorNode()
{
	this->type = OPERATOR_NODE;
}

double OperatorNode::solve()
{
	switch (this->function) {
		case '*':
			return this->leftChild->solve() * this->rightChild->solve();
		case '/': {
			double rightChild = this->rightChild->solve();
			
			if ( rightChild != 0 ) {
				return this->leftChild->solve() / rightChild;
			}
			else {
				throw divide_exception();
			}
		}
		case '+':
			return this->leftChild->solve() + this->rightChild->solve();
		case '-':
			return this->leftChild->solve() - this->rightChild->solve();
		case '^':
			return pow( this->leftChild->solve(), this->rightChild->solve() );
	}
}


Tree::Tree()
{
	this->nodeCollection = new vector<Node*>();
}

Node* Tree::addOperand(Node **place, double value)
{
	OperandNode *newNode = new OperandNode();
	
	newNode = new OperandNode();
	newNode->value = value;
	
	this->nodeCollection->push_back( newNode ); 
	
	if (place != NULL) {
		*place = this->nodeCollection->back();
	}
	
	return newNode;
}

Node* Tree::addOperator(Node **place, char oper)
{	
	OperatorNode *newNode = new OperatorNode();
	
	newNode = new OperatorNode();
	newNode->function = oper;
	
	this->nodeCollection->push_back( newNode ); 
	
	if (place != NULL) {
		*place = this->nodeCollection->back();
	}
	
	return newNode;
}

string Tree::print(string term)
{
	std::stringstream out;
	
	out << "digraph \"" << term << "\"" << endl << "{" << endl << "node [shape = box];" << endl;
	
	int i = 0;
	
	for ( vector<Node*>::iterator it = this->nodeCollection->begin(); it != this->nodeCollection->end(); ++it ) {
    	switch ( (*it)->type ) {
			case OPERAND_NODE: {
				OperandNode *tmp = static_cast<OperandNode*>( *it );
				out << "node" << i << " [ label = \"" << boost::lexical_cast<string>(tmp->value) << "\"];" << endl;
				break;
			}
			case OPERATOR_NODE: {
				OperatorNode *tmp = static_cast<OperatorNode*>( *it );
				out << "node" << i << " [ label = \"" << boost::lexical_cast<string>(tmp->function) << "\"];" << endl;
				
				for ( vector<Node*>::iterator iter = this->nodeCollection->begin(); iter != this->nodeCollection->end(); ++iter ) {
					if ( *iter == (*it)->leftChild ) {
						out << "\"node" << i << "\" -> \"node" << (iter - this->nodeCollection->begin()) << "\";" << endl;
					}
					if ( *iter == (*it)->rightChild ) {
						out << "\"node" << i << "\" -> \"node" << (iter - this->nodeCollection->begin()) << "\";" << endl;
					}
				}
				
				break;
			}
		}
		
		i++;
	}
	
	out << "}" << endl;
	
	return out.str();
}