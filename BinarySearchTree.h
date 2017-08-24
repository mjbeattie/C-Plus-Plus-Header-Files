/*	BinarySearchTree.h
*	This file defines the classes and methods associated with the BinarySearchTree. 
*	It is based on code from Radhadrishnan.
*	Author:  Matthew J. Beattie
*	Date:	 August 1, 2017
*/

#include <iostream>
#include <algorithm>
#include "Exception.h"
#include "AbstractBinarySearchTree.h"

using namespace std;

class BinarySearchTreeChangedSubtree : public BinaryTreeException { };
class BinarySearchTreeNotFound : public BinaryTreeException { };


template <class DataType>
class BinarySearchTree : virtual public AbstractBinarySearchTree<DataType>
{
protected:
	DataType* _rootData;										//pointer to data at root
	BinarySearchTree<DataType>* _left;							//pointer to left subtree
	BinarySearchTree<DataType>* _right;							//pointer to right subtree
	bool _subtree;												//true if tree contains a subtree
	void copyTree(BinarySearchTree<DataType>* bat);				//copies one tree to another
	void _makeNull();											//sets all pointers to NULL

public:
	BinarySearchTree();											//empty constructor
	BinarySearchTree(const DataType& data);							//constructor with data input
	virtual ~BinarySearchTree();								//destructor
	BinarySearchTree<DataType>* makeSubtree();					//creates an empty subtree
	bool subtree();												//returns value of _subtree
	void makeEmpty();											//deletes the structure of the tree
	BinarySearchTree<DataType>* _find(const DataType& data);	//protected find method to return pointer to node

	//from AbstractBinaryTreeAccess.h ************************************
	bool isEmpty();								//true if tree is empty, false otherwise
	int Height();								//returns height of tree
	int Size();									//returns number of nodes in tree
	DataType& rootData();						//returns data from root
	BinarySearchTree<DataType>* left();	//returns pointer to left subtree
	BinarySearchTree<DataType>* right();	//returns pointer to right subtree
//	Enumeration<DataType>* inOrderEnumerator();			//returns an inorder enumerator of all data
//	virtual Enumeration<DataType>* postOrderEnumerator();		//returns a postorder enumerator
//	Enumeration<DataType>* preOrderEnumerator();		//returns a preorder enumerator
//	virtual void display(ostream& os);							//displays tree into an ostream
//	virtual void printtree(ostream& os, int level);				//display tree structure

	//from AbstractBinarySearchTree.h ***************************************
	bool contains(const DataType& q);					//returns true if tree contains a node with q
	DataType find(const DataType& q);					//returns a node that matches q or throws exception
	void insert(const DataType& data);					//inserts data while maintaining binary search properties
	void remove(const DataType& data);					//removes the node matching data if present
//	virtual void rangeSearch(DataType& low, DataType& high) = NULL;


};


//Empty constructor
template <class DataType>
BinarySearchTree<DataType>::BinarySearchTree()
{
	_rootData = NULL;
	_left = NULL;
	_right = NULL;
	_subtree = false;
}


//Constructor using data to set root
template <class DataType>
BinarySearchTree<DataType>::BinarySearchTree(const DataType& data)
{
	_subtree = false;
	_rootData = new DataType(data);
	if (_rootData == NULL) throw BinaryTreeMemory();
	_left = makeSubtree();
	_right = makeSubtree();
}


//Destructor
template <class DataType>
BinarySearchTree<DataType>::~BinarySearchTree()
{
	if (_rootData != NULL)
		delete _rootData;
	_rootData = NULL;
	if (_left != NULL)
		delete _left;
	_left = NULL;
	if (_right != NULL)
		delete _right;
	_right = NULL;
}


//makeSubtree:  creates pointer to a subtree under root
template <class DataType>
BinarySearchTree<DataType>* BinarySearchTree<DataType>::makeSubtree()
{
	BinarySearchTree<DataType>* bat = new BinarySearchTree<DataType>();	//dynamically create new subtree
	bat->_subtree = true;
	return bat;
}


//checks to see whether or not the binary search tree is empty
template <class DataType>
bool BinarySearchTree<DataType>::isEmpty()
{
	return (_rootData == NULL);
}


//returns pointer to root data object
template <class DataType>
DataType& BinarySearchTree<DataType>::rootData()
{
	if (isEmpty()) throw BinaryTreeEmptyTree();
	return *_rootData;
}


//returns pointer to top of left subtree
template <class DataType>
BinarySearchTree<DataType>* BinarySearchTree<DataType>::left()
{
	return _left;
}


//returns pointer to top of right subtree
template <class DataType>
BinarySearchTree<DataType>* BinarySearchTree<DataType>::right()
{
	return _right;
}


//returns tree if a subtree exists, false otherwise
template <class DataType>
bool BinarySearchTree<DataType>::subtree()
{
	return _subtree;
}


//returns the height of the tree by checking both sides via recursion
template <class DataType>
int BinarySearchTree<DataType>::Height()
{
	if (isEmpty()) return 0;
	return (1 + std::max(_left->Height(), _right->Height()));
}


//returns the number of all the nodes in the tree
template <class DataType>
int BinarySearchTree<DataType>::Size()
{
	if (isEmpty()) 
		return 0;
	return (1 + _left->Size() + _right->Size());
}


//deletes the binary search tree structure
template <class DataType>
void BinarySearchTree<DataType>::makeEmpty()
{
	if (_subtree) throw BinarySearchTreeChangedSubtree();
	if (_rootData != NULL)
		delete _rootData;
	_rootData = NULL;
	if (_left != NULL)
		delete _left;
	_left = NULL;
	if (_right != NULL)
		delete _right;
	_right = NULL;
}


//copyTree copies one binary search tree onto another
template <class DataType>
void BinarySearchTree<DataType>::copyTree(BinarySearchTree<DataType>* bst)
{
	_rootData = bst->_rootData;
//	if (_left != NULL) _left->makeEmpty();					/***** FUCKS IT UP *******/
	_left = bst->_left;
//	if (_right != NULL) _right->makeEmpty();
	_right = bst->_right;
}


//sets all the pointers in a node to NULL
template <class DataType>
void BinarySearchTree<DataType>::_makeNull()
{
	_rootData = NULL;
	_left = NULL;
	_right = NULL;
}


//returns pointer to a node where a data element should be found
template <class DataType>
BinarySearchTree<DataType>* BinarySearchTree<DataType>::_find(const DataType& data)
{
	BinarySearchTree<DataType>* bst = this;
	while (true)
	{
		if (bst->isEmpty())							//returns pointer to node if empty
			return bst;
		if (*(bst->_rootData) < data)				//moves pointer to right subtree if root < data
			bst = bst->_right;
		else if (*(bst->_rootData) > data)			//moves pointer to left subtree if root > data
			bst = bst->_left;
		else
			return bst;								//returns pointer to node if equal to data
	}
}


//returns contents of node if data is found, exception otherwise
template <class DataType>
DataType BinarySearchTree<DataType>::find(const DataType& q)
{
	BinarySearchTree<DataType>* bst = _find(q);
	if (bst->isEmpty()) throw BinarySearchTreeNotFound();
	return bst->rootData();
}


//returns true if data is found in the tree, false otherwise
template <class DataType>
bool BinarySearchTree<DataType>::contains(const DataType& q)
{
	BinarySearchTree<DataType>* bst = _find(q);
	return !bst->isEmpty();
}


//inserts data into the appropriate node of the tree, creating one if necessary
template <class DataType>
void BinarySearchTree<DataType>::insert(const DataType& data)
{
	if (_subtree) throw BinarySearchTreeChangedSubtree();
	BinarySearchTree<DataType>* bst = _find(data);
	if (bst->isEmpty())
	{
		bst->_rootData = new DataType(data);
		bst->_left = makeSubtree();
		bst->_right = makeSubtree();
	}
	else
	{
		delete bst->_rootData;
		bst->_rootData = new DataType(data);
	}
}


//removes a node that matches data and restructures the tree appropriately
template <class DataType>
void BinarySearchTree<DataType>::remove(const DataType& data)
{
	if (this->_subtree) throw BinarySearchTreeChangedSubtree();
	BinarySearchTree<DataType>* bst;
	BinarySearchTree<DataType>* bst2;
	BinarySearchTree<DataType>* bst3;

	bst = _find(data);
	if (bst->isEmpty()) throw BinarySearchTreeNotFound();

	//dispose of existing data and overwrite pointer
	delete bst->_rootData;

	//left subtree exists, but pointer will be overwritten, so dispose of it
	if (bst->_left->isEmpty())
	{
		delete bst->_left;						//remove pointer to left subtree
		bst2 = bst->_right;						//save the pointer to the right subtree
		bst->copyTree(bst2);					//copy the right subtree, if right is empty, bst is empty
		bst2->_makeNull();						//prepare right subtree for deletion
		delete bst2;							//delete right subtree to prevent garbage
	}
	
	//right subtree exists, but pointer will be overwritten, so dispose of it
	else if (bst->_right->isEmpty())
	{
		delete bst->_right;						//remove the pointer to right subtree
		bst2 = bst->_left;						//save the pointer to the left subtree
		bst->copyTree(bst2);					//copy the left subtree
		bst2->_makeNull();						//prepare left subtree for deletion
		delete bst2;							//delete left subtree to prevent garbage
	}

	//both subtrees are non-empty
	else
	{
		bst2 = bst->_right;						//move to the right
		while (!bst2->_left->isEmpty())			//move down as far left as possible
			bst2 = bst2->_left;
		bst->_rootData = bst2->_rootData;		//overwrite the data pointer
		delete bst2->_left;						//bst2's left subtree is known to be empty, so overwrite pointer
		if (bst2->_right->isEmpty())			//bst2's right child is copied into it
		{
			delete bst2->_right;				
			bst2->_makeNull();
		}
		else
		{
			bst3 = bst2->_right;
			bst2->copyTree(bst2->_right);
			bst3->_makeNull();
			delete bst3;
		}
	}
}


