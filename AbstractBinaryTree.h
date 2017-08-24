/*	AbstractBinaryTree.h
*	This header file declares the classes and methods necessary to implement
*	binary trees by extending AbstractBinaryTreeAccess.  It is based on code from Radhadrishnan.
*	Author:  Matthew J. Beattie
*	Date:	 August 1, 2017
*/

#ifndef _ABSTRACTBINARYTREE_H
#define _ABSTRACTBINARYTREE_H

#include "AbstractBinaryTreeAccess.h"

template <class DataType>
class AbstractBinaryTree : public AbstractBinaryTreeAccess<DataType>
{
public:
	virtual ~AbstractBinaryTree();								//destructor
	virtual AbstractBinaryTree<DataType>* btleft() = NULL;		//returns pointer to left subtree
	virtual AbstractBinaryTree<DataType>* btright() = NULL;		//returns pointer to right subtree
	virtual void setrootData(DataType& data) = NULL;			//set data at the root node
	virtual AbstractBinaryTree<DataType>* setleft
		(AbstractBinaryTree<DataType>* newleft) = NULL;			//sets left tree to newleft and returns former left
	virtual AbstractBinaryTree<DataType>* btright
		(AbstractBinaryTree<DataType>* newright) = NULL;		//sets right tree to newright and returns former right
};

#endif	//_ABSTRACTBINARYTREE_H
