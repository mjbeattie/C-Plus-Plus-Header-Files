/*	AbstractBinarySearchTree.h
*	This header file declares the classes and methods necessary to implement
*	binary search trees by extending AbstractBinaryTreeAccess.  It is based on code from Radhadrishnan.
*	Author:  Matthew J. Beattie
*	Date:	 August 1, 2017
*/

#ifndef _ABSTRACTBINARYSEARCHTREE_H
#define _ABSTRACTBINARYSEARCHTREE_H

#include "AbstractBinaryTreeAccess.h"
#include <stack>
#include <list>

template <class DataType>
class AbstractBinarySearchTree
{

public:
	virtual ~AbstractBinarySearchTree();						//destructor
	virtual DataType& rootData() = NULL;						//returns data from root
	virtual AbstractBinarySearchTree<DataType>* left() = NULL;	//returns pointer to left subtree
	virtual AbstractBinarySearchTree<DataType>* right() = NULL;	//returns pointer to right subtree
	virtual bool contains(const DataType& q) = NULL;			//returns true if tree contains a node with q
	virtual DataType find(const DataType& q) = NULL;			//returns a node that matches q or throws exception
	virtual void insert(const DataType& data) = NULL;			//inserts data while maintaining binary search properties
	virtual void remove(const DataType& data) = NULL;			//removes the node matching data if present
	Enumeration<DataType>* preOrderEnumerator();		//returns a pre-order enumerator
	virtual bool isEmpty() = NULL;								//flag to determine if tree is empty
	void preOrderDisplay();
	void inOrderDisplay();
	void postOrderDisplay();
//	virtual void rangeSearch(DataType& low, DataType& high) = NULL;
																//outputs all node values in range low to high inclusive
};


//Destructor
template <class DataType>
AbstractBinarySearchTree<DataType>::~AbstractBinarySearchTree() { }

/*  PREORDER METHODS
*	These methods walk through the trees using preorder progression.  Preorder 
*	means that the tree returns the root, then the left side before the right
*/
template <class DataType>
void AbstractBinarySearchTree<DataType>::preOrderDisplay()
{
	if (isEmpty())
		return;
	cout << rootData() << " ";
	left()->preOrderDisplay();
	right()->preOrderDisplay();
}

/*preorder enumerator of binary search tree
* enumerators by nature require returning individual locations of nodes, so this method
* is iterative rather than recursive.  This class uses an STD stack with a list container
* to store the nodes during iteration.  It differs from the book solution, which uses 
* a user-defined list structure.
*/
template <class DataType>
class preOrderEnumerator : public Enumeration<DataType>
{
protected:
	AbstractBinarySearchTree<DataType>* _tree;
	stack<AbstractBinarySearchTree<DataType>*, list<AbstractBinarySearchTree<DataType>*>>* _stack;

public:
	preOrderEnumerator(AbstractBinarySearchTree<DataType>* t);
	bool hasMoreElements();
	DataType& nextElement();

};

template <class DataType>
preOrderEnumerator<DataType>::preOrderEnumerator(AbstractBinarySearchTree<DataType>* t)
{
	_tree = t;
	list<AbstractBinarySearchTree<DataType>*> tpTree;
	_stack = new stack<AbstractBinarySearchTree<DataType>*, list<AbstractBinarySearchTree<DataType>*>>(tpTree);
	if ((_tree != NULL) && (!_tree->isEmpty()))
	{
		_stack->push(_tree);
	}
}

//returns true if enumerator has more elements in the tree
template <class DataType>
bool preOrderEnumerator<DataType>::hasMoreElements()
{
	return (!_stack->empty());
}

//returns the next element in the tree
template <class DataType>
DataType& preOrderEnumerator<DataType>::nextElement()
{
	if (_stack->empty()) throw BinaryTreeEmptyTree();

	AbstractBinarySearchTree<DataType>* tree = _stack->top();	//set _tree to next of stack location
	_stack->pop();
	if (!tree->right()->isEmpty())								//move to next stack element
	{
		_stack->push(tree->right());
	}
	if (!tree->left()->isEmpty())
	{
		_stack->push(tree->left());
	}
	return tree->rootData();
}

//creates a preOrderEnumerator
template <class DataType>
Enumeration<DataType>* AbstractBinarySearchTree<DataType>::preOrderEnumerator()
{
	return new preOrderEnumerator::preOrderEnumerator<DataType>(this);
}


/*	INORDER METHODS
*	These methods interact with the bst using inorder progression.
*	Inorder progression progresses left first, then root, then right.
*/
template <class DataType>
void AbstractBinarySearchTree<DataType>::inOrderDisplay()
{
	if (isEmpty())
		return;
	left()->inOrderDisplay();
	cout << rootData() << " ";
	right()->inOrderDisplay();
}

/*	POSTORDER METHODS
*	These methods interact with the bst using postorder progression.
*	Inorder progression progresses left first, then right, then root.
*/
template <class DataType>
void AbstractBinarySearchTree<DataType>::postOrderDisplay()
{
	if (isEmpty())
		return;
	left()->postOrderDisplay();
	right()->postOrderDisplay();
	cout << rootData() << " ";
}


#endif	//_ABSTRACTBINARYSEARCHTREE_H

