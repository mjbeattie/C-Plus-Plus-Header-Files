/*	AbstractGraph.h
*	Defines the abstract class for the graph classes.  Based upon the textbook
*	by Radhakrishnan.
*	Author:  Matthew J. Beattie
*	Date:  August 6, 2017
*/


#include <iostream>
#include <vector>
#include "Enumeration.h"
#include "Exception.h"

using namespace std;

class GraphException : public Exception { };
class GraphDuplicateEdge : public GraphException { };
class GraphEdgeOutOfBounds : public GraphException { };
class GraphMemory : public GraphException { };
class GraphNegativeCount : public GraphException { };
class GraphNonExistentEdge : public GraphException { };
class GraphVertexOutOfBounds : public GraphException { };


//AbstractGraph abstract class -- top parent graph class, serves as base for all other classes.
//Defines graph as set of vertices and edges
template <class VertexObject, class EdgeObject>
class AbstractGraph
{
public:
	virtual ~AbstractGraph();
	virtual int vertexCount() = NULL;						//returns the number of vertices
	virtual int edgeCount() = NULL;							//returns the number of edges
	virtual bool hasEdge(int start, int end) = NULL;		//returns true if the edge between vertices exists
	virtual VertexObject& vertexInfo(int v) = NULL;			//returns data stored in vertex v
	virtual double edgeWeight(int start, int end) = NULL;	//returns the weight of the edge, returns 1 in
															//an unweighted graph
	virtual EdgeObject& edgeInfo(int start, int end) = NULL;	//returns data associated with the edge
	virtual vector<int> neighbors(int v) = NULL;			//returns the neighbors of vertex v
	virtual void setVertexInfo(int v, VertexObject& info) = NULL;	//sets vertex info
	virtual void setEdgeInfo(int start, int end, EdgeObject& info) = NULL;	//sets edge info
	virtual void deleteEdge(int start, int end) = NULL;		//deletes edge between two vertices
	virtual void display(ostream& os);						//displays vertices and edges of the graph
	virtual void displayNeighbors(int v, ostream& os) = NULL;		//displays the elements of the neighbors vector
	void printVector(vector<int>, ostream& os);				//prints a vector

	friend ostream& operator<< (ostream& s, AbstractGraph<VertexObject, EdgeObject>& g)
	{
		g.display(s);
		return s;
	}
};

template <class VertexObject, class EdgeObject>
AbstractGraph<VertexObject, EdgeObject>::~AbstractGraph() { }

//display() -- display method for the set of graph classes.  Displays vertices
//in a vector format and edges in (x,y) format.
template <class VertexObject, class EdgeObject>
void AbstractGraph<VertexObject, EdgeObject>::display(ostream& os)
{
	bool first = true;
	os << "Vertices: [";
	for (int i = 0; i < vertexCount(); i++)
	{
		if (first) first = false;
		else os << ", ";
		os << "(" << i << ") " << vertexInfo(i);
	}
	os << "];  Edges: [";
	first = true;
	for (int i = 0; i < vertexCount(); i++)
	{
		for (int j = 0; j < vertexCount(); j++)
		{
			if (hasEdge(i, j))
			{
				if (first) first = false;
				else os << ", ";
				os << "(" << i << "," << j << ") " << edgeInfo(i, j);
			}
		}
	}
	os << "]";
}


//overloaded<< -- overloaded << operator calls display() and returns to an ostream
template <class VertexObject, class EdgeObject>
ostream& operator<< (ostream& os, AbstractGraph<VertexObject, EdgeObject>& g)
{
	g.display(os);
	return os;
}


//AbstractWeightedGraph -- abstract class for graphs with weighted edges
template <class VertexObject, class EdgeObject>
class AbstractWeightedGraph :
	virtual public AbstractGraph<VertexObject, EdgeObject>
{
public:
	virtual ~AbstractWeightedGraph();
	virtual void addEdge(int start, int end, double weight) = NULL;
	virtual void addEdge(int start, int end, double weight, EdgeObject& info) = NULL;
};

template <class VertexObject, class EdgeObject>
AbstractWeightedGraph<VertexObject, EdgeObject>::~AbstractWeightedGraph() { }


//AbstractUnweightedGraph -- abstract class for graphs with no edge weights
template <class VertexObject, class EdgeObject>
class AbstractUnweightedGraph : 
	virtual public AbstractGraph<VertexObject, EdgeObject>
{
public:
	virtual ~AbstractUnweightedGraph();
	virtual void addEdge(int start, int end) = NULL;
	virtual void addEdge(int start, int end, EdgeObject& info) = NULL;
};

template <class VertexObject, class EdgeObject>
AbstractUnweightedGraph<VertexObject, EdgeObject>::~AbstractUnweightedGraph() { };

//printVector prints vectors in standard math form and returns to an ostream
template <class VertexObject, class EdgeObject>
void AbstractGraph<VertexObject, EdgeObject>::printVector(vector<int> v, ostream& os)
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
