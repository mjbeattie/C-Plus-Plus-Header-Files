/*	AbstractBinaryTreeAccess.h
*	This header file declares the classes and methods necessary to implement
*	binary trees and binary search trees.  It is based on code from Radhadrishnan.
*	Author:  Matthew J. Beattie
*	Date:	 August 1, 2017
*/

#ifndef _ABSTRACTBINARYTREEACCESS_H
#define _ABSTRACTBINARYTREEACCESS_H

#include <iostream>
#include "Exception.h"
#include "Enumeration.h"

using namespace std;

class BinaryTreeException : public Exception { };
class BinaryTreeMemory : public BinaryTreeException { };
class BinaryTreeGaveSubtreeToEmptyTree : public BinaryTreeException { };
class BinaryTreeEmptyTree : public BinaryTreeException { };

template <class DataType>
class AbstractBinaryTreeAccess
{
public:
	virtual ~AbstractBinaryTreeAccess();							//destructor
	virtual bool isEmpty() = NULL;								//true if tree is empty, false otherwise
	virtual int Height() = NULL;								//returns height of tree
	virtual int Size() = NULL;									//returns number of nodes in tree
	virtual DataType& rootData() = NULL;						//returns data from root
	virtual AbstractBinaryTreeAccess<DataType>* left() = NULL;	//returns pointer to left subtree
	virtual AbstractBinaryTreeAccess<DataType>* right() = NULL;	//returns pointer to right subtree
//	virtual Enumeration<DataType>* inOrderEnumerator();			//returns an inorder enumerator of all data
//	virtual Enumeration<DataType>* postOrderEnumerator();		//returns a postorder enumerator
	virtual Enumeration<DataType>* preOrderEnumerator() = NULL;	//returns a preorder enumerator
//	virtual void display(ostream& os);							//displays tree into an ostream
//	virtual void printtree(ostream& os, int level);				//display tree structure
};


//Destructor
template <class DataType>
AbstractBinaryTreeAccess<DataType>::~AbstractBinaryTreeAccess() { }


#endif	//_ABSTRACTBINARYTREEACCESS_H