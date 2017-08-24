/*	AbstractArray.h
*	Header file for the declaration of the abstract class for arrays
*	Author:  Matthew J. Beattie
*	Date:	 August 1, 2017
*/

#ifndef _ABSTRACTARRAY_H
#define _ABSTRACTARRAY_H

#include <iostream>
#include "Exception.h"
#include "Enumeration.h"

using namespace std;

class ArrayException : public Exception { };
class ArrayMemoryException : public ArrayException { };
class ArrayBoundsException : public ArrayException { };

template <class DataType>
class AbstractArrayClass
{

friend ostream& operator<< (ostream& s, AbstractArrayClass<DataType>& ac);

public:
	virtual int size() const = NULL;
	virtual DataType& operator[] (int k) = NULL;

};



#endif	//_ABSTRACTARRAY_H