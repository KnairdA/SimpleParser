#include <vector>
#include <string>
#include <sstream>
#include <math.h>

using namespace std;

enum NodeType 
{
	OPERAND_NODE,
	OPERATOR_NODE,
};

class Node
{
	public:
		Node();
		double solve();
		
		Node *leftChild;
		Node *rightChild;

		NodeType type;		
};

class OperatorNode: public Node 
{
	public:
		OperatorNode();
		double solve();
		char function;
};


class OperandNode: public Node
{
	public:
		OperandNode();
		double solve();
		double value;
};

class Tree
{
	public:
		Tree();
		
		Node *root;
		
		Node* addOperand(Node**, double);
		Node* addOperator(Node**, char);
		
		string print(string);
		
	private:
		vector<Node*> *nodeCollection;
};

class divide_exception: public exception
{
	virtual const char* what() const throw()
	{
		return "A divison through zero had to be prevented by the parser - check your input term.";
	}
};