/* HashTable.h
*  Date:  July 20, 2017
*  Author:  Matthew J. Beattie (copied from book)
*  Description:  This header file contains the abstract class definition for the HashTable
*                class.
*/

#include <iostream>
#include "Exception.h"
#include "Enumeration.h"
using namespace std;

class HashTableException : public Exception { };
class HashTableOutOfBounds : public HashTableException { };
class HashTableElementNotFound : public HashTableException { };
class HashTableMemory : public HashTableException { };
class HashCalculationError : public HashTableException { };

template <class DataType>
class AbstractHashTable {
	friend ostream& operator<< (ostream& s, const AbstractHashTable<DataType>& HT);

protected:
	virtual int hash(const DataType& data) = NULL;		//Determines hash value for data

public:
	virtual ~AbstractHashTable();
	virtual DataType find(const DataType& q) = NULL;	//Returns the element which matches q
														//or throws an exception on no match
	virtual void insert(const DataType& data) = NULL;	//Inserts data while maintaining hash function
	virtual void remove(const DataType& data) = NULL;	//Removes the matching data element
	virtual bool collision(int pos) = NULL;				//Returns true is there is no element
														//in the table at position pos, false otherwise
	virtual bool isEmpty() = NULL;						//Returns true if there are no table elements
	virtual int capacity() = NULL;						//Returns the capacity of the hash table
	virtual int size() = NULL;							//Returns the number of elements stored in the table
	virtual DataType& operator  [] (int k) = NULL;		//Returns the object that is stored in position k
	void display(ostream& s);					//Display the nodes of the linked list
	Enumeration<DataType>* enumerator();		//Returns an enumeration of the list
}; //end AbstractHashTable

/**************************************************/
/* HTEnumerator:  Enumerator class and methods for AbstractHashTable*/
//HTEnumerator class:  enumerator class for AbstractHashTable
template <class DataType>
class HTEnumerator : public Enumeration<DataType> {
	friend AbstractHashTable<DataType>;
protected:
	AbstractHashTable<DataType>* _HT;
	HTEnumerator(AbstractHashTable<DataType>* HT);
	int _currentIndex;						//Address in the table for the element to be returned next

public:
	virtual bool hasMoreElements();
	virtual DataType& nextElement();
};

/****************************************/
//HTEnumerator():  constructor for an AbstractHashTable enumerator
template <class DataType>
HTEnumerator<DataType>::HTEnumerator(AbstractHashTable<DataType>* HT)
{
	_HT = HT;
	_currentIndex = 0;
}

/****************************************/
//hasMoreElements():  logical test to see if the AbstractHashTable has more elements
template <class DataType>
bool HTEnumerator<DataType>::hasMoreElements()
{
	return((_HT != NULL) && (!_HT->isEmpty() && (_currentIndex < _HT->size())));
}

/*****************************************/
//HTEnumerator():  returns the next element of a hash table
template <class DataType>
DataType& HTEnumerator<DataType>::nextElement()
{
	int temp;
	if ((_HT == NULL) || (_HT->isEmpty()) || (_currentIndex == _HT->Capacity()))
		throw HashTableOutOfBounds();			//NOTE:  FIXED FROM BOOK?
	temp = _currentIndex;
	while ((*_HT[currentIndex] == NULL) && (_currentIndex < _HT->Capacity()))
		_currentIndex++;

	return *_HT[temp];
}

/*****************************************/
//display():  prints out AbstractHashTable
template <class DataType>
void AbstractHashTable<DataType>::display(ostream& os)
{
	bool first = true;
	Enumeration<DataType>* e = enumerator();
	os << "<";
	while (e->hasMoreElements())
	{
		if (!first) os << ", ";
		first = false;
		os << e->nextElement();
	}
	os << ">";
	delete e;
}

/*******************************************/
//enumerator():  creates a new HTEnumerator for AbstractHashTable and returns a pointer to it
template <class DataType>
Enumeration<DataType>* AbstractHashTable<DataType>::enumerator()
{
	return new HTEnumerator<DataType>(this);
}

/*******************************************/
//<<:  overloaded operator << to display AbstractHashTable
template<class DataType>
ostream& operator << (ostream& s, const AbstractHashTable<DataType>& HT)
{
	HT.display(s);
	return s;
}
