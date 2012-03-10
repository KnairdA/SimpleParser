#include "tree.h"
#include <limits>

Node::Node()
{

}

template <class T> 
double Node::castSolve (Node *node) {
	T *tmp = static_cast<T*>( node );
	return tmp->solve();
}

double Node::solve()
{
	switch (this->type) {
		case OPERAND_NODE: {
			return this->castSolve<OperandNode>( this );
		}
		case OPERATOR_NODE: {
			return this->castSolve<OperatorNode>( this );
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

	typedef std::numeric_limits<double> dbl;
	out.precision(dbl::digits10);
	
	out << "digraph \"" << term << "\"" << endl << "{" << endl << "node [shape = box];" << endl;
	
	int i = 0;
	
	for ( vector<Node*>::iterator it = this->nodeCollection->begin(); it != this->nodeCollection->end(); ++it ) {
    	switch ( (*it)->type ) {
			case OPERAND_NODE: {
				OperandNode *tmp = static_cast<OperandNode*>( *it );
				out << "node" << i << " [ label = \"" << tmp->value << "\"];" << endl;
				break;
			}
			case OPERATOR_NODE: {
				OperatorNode *tmp = static_cast<OperatorNode*>( *it );
				out << "node" << i << " [ label = \"" << tmp->function << "\"];" << endl;
				
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