/*	CommonMethods.h
*	This header contains common methods used in multiple classes
*	Author:  Matthew J. Beattie
*	Date:	 August 8, 2017
*/

#ifndef _COMMONMETHODS_H
#define _COMMONMETHODS_H

#include <iostream>
#include <vector>

using namespace std;

class MyCommonMethods
{
public:
	void printVector(vector<int> v, ostream& os);						//prints a vector
};

void MyCommonMethods::printVector(vector<int> v, ostream& os)
{
	int n = v.size();
	os << "[";
	for (int i = 0; i < n; i++)
	{
		os << v[i];
		if (i < (n - 1)) os << ", ";
	}
	os << "]" << endl;
}



#endif	//_COMMONMETHODS_H
