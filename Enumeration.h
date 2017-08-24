/* Enumeration.h
*  Date:  July 20, 2017
*  Author:  Matthew J. Beattie (copied from book)
*  Description:  Abstract class definition for enumeration classes and methods
*/

#ifndef _ITERATOR_H
#define _ITERATOR_H

#include "Exception.h"

class EnumerationException : public Exception { };

template <class DataType>
class Enumeration
{
public:
	//Boolean method which determines whether there are any more elements
	//in the data structure being Enumerated
	
	virtual bool hasMoreElements() = NULL;
	virtual DataType& nextElement() = NULL;		//Returns the object which is the next element
};

#endif
