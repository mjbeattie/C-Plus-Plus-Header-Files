/* VectorHashTable.h : Header file containing the definition of the VectorHashTable class.
*  VectorHashTable sets up a hash table and then implements positions in the hash table as
*  standard vectors.  This allows for dynamic expansion of each hash table location.
*  Author:  Matthew J. Beattie
*  Date:    July 18, 2017
*  Course:  CS 5005, Summer 2017
*/


#include <iostream>
#include <vector>
#include <array>
#include <list>
#include "Exception.h"
#include "Enumeration.h"

using namespace std;

class HashTableException : public Exception { };
class HashTableMemory : public HashTableException { };
class HashCalculationError : public HashTableException { };
class HashTableOutOfBounds : public HashTableException { };
class ItemNotFound : public HashTableException { };


/* class VectorHashTable
*  Description:  An extension of AbstractHashTable that uses a vector as the table.  Includes
*                constructors, accessors, etc. as well as split(), a method to balance the
*                table sizes.  The vector is capable of handling different types of data.
*				 The elements of the VectorHashTable are linked lists, the elements of which
*				 are determined by the data type in the template.
*/

template <class DataType>
class VectorHashTable
{
protected:
	vector<list<DataType>>* Table;					//Main structure of the hash table:  a vector
													//of linked lists.

public:
	VectorHashTable();
	VectorHashTable(int n);
	VectorHashTable(VectorHashTable<DataType>& HT);
	~VectorHashTable();
	bool find(const DataType& data);				//Boolean test to see if an element is in the hash table
	int foundAt(const DataType& data);				//Returns the hash location of a found element
	void insert(const DataType& data);				//Inserts data while maintaining hash function
	void remove(const DataType& data);				//Removes the matching data element
	bool collision(int pos);						//Returns true is there is no element
													//in the table at position pos, false otherwise
	bool isEmpty();									//Returns true if there are no table elements
	int size();										//Returns the number of elements stored in the table
	void displayLL(unsigned int n);					//Prints the linked list at hash table location n
	void displayLL(ostream&s, unsigned int n);		//Prints the linked list to an ostream
	void displayHT();								//Prints the entire hash table
	void displayHT(ostream& s);						//Prints hash table for overloaded operator
	list<DataType> operator[] (unsigned int k);   	//Returns the object that is stored in position k
	int hash(const char* data);						//Hashing function for tables with char* elements
	int hash(const int& data);						//Hashing function for tables with integer elements
	void split(unsigned int, unsigned int p);		//Limits a position in the hash table to p elements and moves
													//the remainder to positions below
	void copy(VectorHashTable<DataType>& HT);       //Creates a copy of an existing hash table
	void operator= (VectorHashTable<DataType>& HT); //Overloaded = operator to assign HT to another

													//Overloaded operator -- defined in class body because it didn't work otherwise!!
	friend ostream& operator<< (ostream& s, VectorHashTable<DataType>& HT)
	{
		HT.displayHT(s);
		return s;
	}

};

//Default constructor
template <class DataType>
VectorHashTable<DataType>::VectorHashTable()
{
	Table = new vector<list<DataType>>;
}

//Empty table constructor of size n
template <class DataType>
VectorHashTable<DataType>::VectorHashTable(int n)
{
	try
	{
		Table = new vector<list<DataType>>(n);
	}
	catch (out_of_range e)
	{
		cout << "VectorHashTable experienced an out_of_range error";
		throw HashTableMemory();
	}
}

//Destructor
template <class DataType>
VectorHashTable<DataType>::~VectorHashTable()
{
	cout << "Deleting Hash Table" << endl;
	delete Table;
}


//collision():  determine if there is a collision at element Pos
template <class DataType>
bool VectorHashTable<DataType>::collision(int Pos)
{
	try
	{
		if (Pos <= (Table->size()) - 1)
		{
			if ((*Table)[Pos].size() > 0)
				return true;
			else
				return false;
		}
	}
	catch (exception e)
	{
		throw HashTableOutOfBounds();
	}
}

//size():  returns the number of items stored in the hash table
template <class DataType>
int VectorHashTable<DataType>::size()
{
	return (*Table).size();
}

//isEmpty():  returns true if the number of items stored in the hash table is 0
template <class DataType>
bool VectorHashTable<DataType>::isEmpty()
{
	return ((*Table).size() == 0);
}


//find():  returns the item store in the linked list at location hash(key)
//		   If not found in the linked list at hash(key), find() searches the
//	       linked lists higher in the stack
template <class DataType>
bool VectorHashTable<DataType>::find(const DataType& key)
{
	unsigned int k = hash(key);
	if ((k >= (*Table).size()) || ((*Table)[k].size() == 0))
	{
		return false;
	}
	else
	{
		for (unsigned int i = k; i < (*Table).size(); ++i)
		{
			for (list<char*>::iterator iter = (*Table)[i].begin(); iter != (*Table)[i].end(); ++iter)
			{
				if (strcmp(*iter,key)==0) return true;
			}
		}
		return false;
	}
}

//foundAt():  returns the hash location of the list in which an element is found
template <class DataType>
int VectorHashTable<DataType>::foundAt(const DataType& key)
{
	try
	{
		if (find(key))
		{
			unsigned int k = hash(key);
			for (unsigned int i = k; i < (*Table).size(); ++i)
			{
				for (list<char*>::iterator iter = (*Table)[i].begin(); iter != (*Table)[i].end(); ++iter)
				{
					if (strcmp(*iter,key)==0) return i;
				}
			}

		}
		return -1;
	}
	catch (exception e)
	{
		throw ItemNotFound();
	}
}


//hash(char* data):  Hashing function for char*.  Generates the nth value for unicode?
int VectorHashTable<char*>::hash(const char* data)
{
	try
	{
		int h_x = data[0] - 'A';
		return h_x;
	}
	catch (exception e)
	{
		cout << "Could not calculate hash function";
		throw HashCalculationError();
	}
}

//hash(int& data):  Hashing function for VectorHashTable of integers
int VectorHashTable<int>::hash(const int& data)
{
	try
	{
		int h_x = data;
		return h_x;
	}
	catch (exception e)
	{
		cout << "Could not calculate hash function";
		throw HashCalculationError();
	}
}


//insert():  inserts a new object into the hash table in a linked list at a location determined by hash().
//           insert() will add elements to the hash table if necessary.
template <class DataType>
void VectorHashTable<DataType>::insert(const DataType& data)
{
	unsigned int k = hash(data);
	if (k < 0) throw HashTableOutOfBounds();
	if (k >= (*Table).size())	(*Table).resize(k + 1);
	(*Table)[k].push_back(data);
}

//displayLL():  displays a linked list at a location in the hash table given by integer n
template <class DataType>
void VectorHashTable<DataType>::displayLL(unsigned int n)
{
if (n <= ((*Table).size() - 1) && (n >= 0))
{
	if ((*Table)[n].size() > 0)
	{
		for (list<char*>::iterator iter = (*Table)[n].begin(); iter != (*Table)[n].end(); ++iter)
		{
			cout << *iter << ", ";
		}
		cout << endl;
	}
	else return;
}
else throw HashTableOutOfBounds();
}

//displayLL(ostream& s, n):  displays linked list to an ostream
template <class DataType>
void VectorHashTable<DataType>::displayLL(ostream& s, unsigned int n)
{
	if (n <= ((*Table).size() - 1) && (n >= 0))
	{
		if ((*Table)[n].size() > 0)
		{
			cout << n << "-> ";
			for (list<char*>::iterator iter = (*Table)[n].begin(); iter != (*Table)[n].end(); ++iter)
			{
				s << *iter << ", ";
			}
			s << endl;
		}
		else return;
	}
	else throw HashTableOutOfBounds();
}

//displayHT():  displays all the linked lists in the hash table
template <class DataType>
void VectorHashTable<DataType>::displayHT()
{
	for (unsigned int i = 0; i < (*Table).size(); ++i)
	{
		displayLL(i);
	}
}


//displayHT(ostream& s):  displays all the linked lists in the hash table into a stream for <<
template <class DataType>
void VectorHashTable<DataType>::displayHT(ostream& s)
{
	for (unsigned int i = 0; i < (*Table).size(); ++i)
	{
		displayLL(s, i);
	}
}

//remove():  removes an object from the hash table if found, otherwise throws an exception
template <class DataType>
void VectorHashTable<DataType>::remove(const DataType& data)
{
	try
	{
		int k = foundAt(data);
		if (k == -1)
		{
			cout << "The remove() method did not find <" << data << ">" << endl;
			return;
		}
		else
		{
			(*Table)[k].remove(data);
			return;
		}

	}
	catch (exception e)
	{
		throw ItemNotFound();
	}
}


//overloaded operator []:  used to return the linked list at hash table location k
template <class DataType>
list<DataType> VectorHashTable<DataType>::operator[] (unsigned int k)
{
	if ((k < 0) || (k >= (*Table).size())) throw HashTableOutOfBounds();
	return (*Table)[k];
}

//split(i,p):  Takes the ith position in the hash table and reduces its elements to
//			   only p.  Moves all elements in the list beyond p to subsequent positions.
//			   Uses recursion to reduce list one by one.
template <class DataType>
void VectorHashTable<DataType>::split(unsigned int i, unsigned int p)
{
	if (p == 0)								//Aborts is list lengths are set to 0
	{
		cout << "split() was called with a length of 0 -- aborting split" << endl;
	}
	if ((*Table)[i].size() > p)
	{
		bool placedIt = false;
		unsigned int j = i + 1;
		while ((j < (*Table).size()) && !placedIt)
		{
			if ((*Table)[j].size() < p)
			{
				(*Table)[j].push_back((*Table)[i].back());
				placedIt = true;
			}
			++j;
		}
		if (!placedIt)
		{
			int currSize = (*Table).size();
			(*Table).resize(currSize + 1);
			(*Table)[currSize].push_back((*Table)[i].back());
		}
		(*Table)[i].pop_back();
		split(i, p);						//Works back through the list with recursion
	}
	return;
}

//copy():  Copies an existing vector hash table onto an empty one
template <class DataType>
void VectorHashTable<DataType>::copy(VectorHashTable<DataType>& HT)
{
	unsigned int newTableSize = (HT).size();
	for (unsigned int i = 0; i < newTableSize; ++i)
		(*Table).push_back(HT[i]);
}

//VectorHashTable(VectorHashTable& HT):  creates a new VHT as a copy of an existing one
template <class DataType>
VectorHashTable<DataType>::VectorHashTable(VectorHashTable<DataType>& HT)
{
	if (&HT != this)						//Prevents self copy
	{
		Table = new vector<list<DataType>>;
		(*this).copy(HT);
	}
}

//overloaded = operator:  copies one hash table onto another using the = operator
template <class DataType>
void VectorHashTable<DataType>::operator= (VectorHashTable<DataType>& HT)
{
	if (&HT != this)
	{
		copy(HT);
	}
}

