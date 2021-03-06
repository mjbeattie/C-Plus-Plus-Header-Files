/*	ArrayClass.h
*	This header file declares the classes and methods necessary to implement
*	arrays by extending AbstractArray.  It is based on code from Radhadrishnan.
*	Author:  Matthew J. Beattie
*	Date:	 August 1, 2017
*/

#ifndef _ARRAYCLASS_H
#define _ARRAYCLASS_H

#include "AbstractArray.h"

const int ARRAY_CLASS_DEFAULT_SIZE = 1;

template <class DataType>
class ArrayClass : virtual public AbstractArrayClass<DataType>
{
protected:
	DataType* paObject;
	int _size;
	void copy(const ArrayClass<DataType>& ac);
public:
	ArrayClass();
	ArrayClass(int n);
	ArrayClass(int n, const DataType& val);
	ArrayClass(const ArrayClass<DataType>& ac);

	virtual ~ArrayClass();

	virtual int size() const;
	virtual DataType& operator[] (int k);
	void operator= (const ArrayClass<DataType>& ac);
	friend ostream& operator<< (ostream& s, ArrayClass<DataType>& ac)
	{
		s << "[";
		for (int i = 0; i < ac.size(); i++)
		{
			if (i > 0)
			{
				s << ",";
			}
			s << ac[i];
		}
		s << "]";
		return s;
	}

};


//Constructs an array of size n
template <class DataType>
ArrayClass<DataType>::ArrayClass(int n)
{
	_size = 0;							//default in case allocation fails
	paObject = new DataType[n];
	if (paObject == NULL) throw ArrayMemoryException();
	_size = n;
}


//Destructor
template <class DataType>
ArrayClass<DataType>::~ArrayClass()
{
	if (paObject != NULL) delete[] paObject;
	_size = 0;
}


//Construct an n dimension vector with repeated val elements
template <class DataType>
ArrayClass<DataType>::ArrayClass(int n, const DataType& val)
{
	_size = 0;							//default in case allocation fails
	paObject = new DataType[n];			//dynamic creation of DataType
	if (paObject == NULL) throw ArrayMemoryException();
	_size = n;
	for (int i = 0; i < n; i++)
		paObject[i] = val;
}


//Empty constructor -- creates an array of size 1
template <class DataType>
ArrayClass<DataType>::ArrayClass()
{
	_size = 0;							//default in case allocation fails
	paObject = new DataType[ARRAY_CLASS_DEFAULT_SIZE];
	if (paObject == NULL) throw ArrayMemoryException();
	_size = ARRAY_CLASS_DEFAULT_SIZE;
}


//Copy constructor
template <class DataType>
void ArrayClass<DataType>::copy(const ArrayClass<DataType>& ac)
{
	_size = 0;							//default in case allocation fails;
	paObject = new DataType[ac._size];
	if (paObject == NULL) throw ArrayMemoryException();
	_size = ac._size;
	for (int i = 0; i < _size; i++)
	{
		paObject[i] = ac.paObject[i];
	}
}

template <class DataType>
ArrayClass<DataType>::ArrayClass(const ArrayClass<DataType>& ac)
{
	if (&ac != this)					//disallow self copy
		copy(ac);
}

template <class DataType>
void ArrayClass<DataType>::operator= (const ArrayClass<DataType>& ac)
{
	if (&ac != this)					//disallow self copy
	{
		if (paObject != NULL) delete[] paObject;		//delete existing array and replace
		copy(ac);
	}
}


//Returns the size of the array
template <class DataType>
int ArrayClass<DataType>::size() const
{
	return _size;
}


//Overloaded operator [] to access elements
template <class DataType>
DataType& ArrayClass<DataType>::operator[] (int k)
{
	if ((k < 0) || (k >= size())) throw ArrayBoundsException();
	return paObject[k];
}


#endif // !_ARRAYCLASS_H

