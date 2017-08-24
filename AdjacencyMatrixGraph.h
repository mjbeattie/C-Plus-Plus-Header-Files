/*	AdjacencyMatrixGraph.h
*	AdjacencyMatrixGraph class implements the adjacency matrix graph model to represent
*	a set of edges and vertices.  The graph is assumed to be non-directional and unweighted.  
*	It can be extended to include weighted edges by overloading the addEdge, etc., methods 
*	or by defining an EdgeObject as a double.
*	Author:  Matthew J. Beattie
*	Date:  August 6, 2017
*/


#include "AbstractGraph.h"
#include <fstream>
#include <queue>
#include <stack>

using namespace std;

template <class VertexObject, class EdgeObject>
class AdjacencyMatrixGraph :
	virtual public AbstractGraph<VertexObject, EdgeObject>
{
protected:
	vector<VertexObject>* vertexData;
	void _deleteEdge(int start, int end);
	int _vertexCount;
	int _edgeCount;

public:
	//See AbstractGraph.h for descriptions of methods
	AdjacencyMatrixGraph();
	AdjacencyMatrixGraph(const int n);
	AdjacencyMatrixGraph(AdjacencyMatrixGraph<VertexObject, EdgeObject>& g);
	void copy(AdjacencyMatrixGraph<VertexObject, EdgeObject>& g);
	virtual ~AdjacencyMatrixGraph();
	vector<vector<double>> edges;
	vector<vector<EdgeObject>> edgeData;
	int edgeCount();
	int vertexCount();
	void setVertexCount(int v);
	void setVertexInfo(int v, VertexObject& info);
	void setEdgeInfo(int start, int end, EdgeObject& info);
	VertexObject& vertexInfo(int v);
	bool hasEdge(int start, int end);
	EdgeObject& edgeInfo(int start, int end);
	double edgeWeight(int start, int end);
	vector<int> neighbors(int v);
	void displayNeighbors(int v, ostream& os);
	void deleteEdge(int start, int end);
	void addEdge(int start, int end, EdgeObject info);
	void addEdge(int start, int end);
	void operator= (AdjacencyMatrixGraph<VertexObject, EdgeObject>& g);	//Overloaded = operator to assign graph to another

	vector<int> breadthFirstSearch(int u, vector<int> &parent);			//returns a vector of the graph vertices in bfs order
	vector<int> depthFirstSearch(int u, vector<int> &parent);			//returns a vector of the graph vertices in dfs order

};

//default constructor for BitAdjacencyGraph class
template <class VertexObject, class EdgeObject>
AdjacencyMatrixGraph<VertexObject, EdgeObject>::AdjacencyMatrixGraph()
{
	this->setVertexCount(0);
	vertexData = new vector<VertexObject>(0);
}

//constructor for BitAdjacencyGraph to create graph with n vertices
template <class VertexObject, class EdgeObject>
AdjacencyMatrixGraph<VertexObject, EdgeObject>::AdjacencyMatrixGraph(const int n)
{
	this->setVertexCount(n);
	vertexData = new vector<VertexObject>(n);

	edges.resize(n);
	edgeData.resize(n);
	for (int i = 0; i < n; i++)
	{
		edges[i].resize(n);
		edgeData[i].resize(n);
	}
}



//copy():  Copies an existing vector hash table onto an empty one
//works by getting number of vertices and copying info and edges one by one
template <class VertexObject, class EdgeObject>
void AdjacencyMatrixGraph<VertexObject, EdgeObject>::copy(AdjacencyMatrixGraph<VertexObject, EdgeObject>& g)
{
	//if exists, delete destination graph
	if (this->vertexCount() != 0)
	{
		for (int i = 0; i < this->vertexCount(); i++)
		{
			if (this->vertexInfo(i) != NULL) this->vertexInfo(i) = NULL;
			for (int j = 0; j < this->vertexCount(); j++)
			{
				if (this->hasEdge(i, j)) this->deleteEdge(i, j);
			}
		}
	}

	//sets vertexCount , vertexData, and edges structures equal to the size of the source graph and begins copying
	int n = g.vertexCount();
	this->setVertexCount(n);
	vertexData = new vector<VertexObject>(n);

	edges.resize(n);
	edgeData.resize(n);
	for (int i = 0; i < n; i++)
	{
		edges[i].resize(n);
		edgeData[i].resize(n);
	}
	if (this->vertexCount() != g.vertexCount()) throw GraphVertexOutOfBounds();

	for (int i = 0; i < n; i++)
	{
		this->setVertexInfo(i, g.vertexInfo(i));
	}
}

//creates a new graph as a copy of an existing one
template <class VertexObject, class EdgeObject>
AdjacencyMatrixGraph<VertexObject, EdgeObject>::AdjacencyMatrixGraph(AdjacencyMatrixGraph<VertexObject, EdgeObject>& g)
{
	if (&g != this)						//Prevents self copy
	{
		int tmpVertexCount = g.vertexCount();
		this->setVertexCount(tmpVertexCount);
		vertexData = new vector<VertexObject>(tmpVertexCount);
		edges.resize(tmpVertexCount);
		edgeData.resize(tmpVertexCount);
		for (int i = 0; i < tmpVertexCount; i++)
		{
			edges[i].resize(tmpVertexCount);
			edgeData[i].resize(tmpVertexCount);
		}

		this->copy(g);
	}
}

//overloaded = operator:  copies one graph onto another using the = operator
template <class VertexObject, class EdgeObject>
void AdjacencyMatrixGraph<VertexObject, EdgeObject>::operator= (AdjacencyMatrixGraph<VertexObject, EdgeObject>& g)
{
	if (&g != this)
	{
		copy(g);
	}
}


//destructor for BitAdjacencyGraph
template <class VertexObject, class EdgeObject>
AdjacencyMatrixGraph<VertexObject, EdgeObject>::~AdjacencyMatrixGraph() { };

//set vertex count
template <class VertexObject, class EdgeObject>
void AdjacencyMatrixGraph<VertexObject, EdgeObject>::setVertexCount(int v)
{
	_vertexCount = v;
}

//return the number of vertices in the graph
template <class VertexObject, class EdgeObject>
int AdjacencyMatrixGraph<VertexObject, EdgeObject>::vertexCount()
{
	return _vertexCount;
}

//returns the number of edges in the graph
template <class VertexObject, class EdgeObject>
int AdjacencyMatrixGraph<VertexObject, EdgeObject>::edgeCount()
{
	int count = 0;
	for (unsigned int i = 0; i < edges.size(); i++)
	{
		for (unsigned int j = 0; j < edges[i].size(); i++)
		{
			if (edges[i][j] != 0.0)
				count++;
		}
	}
	return count/2;								//for an undirected graph, divide by 2
}

//return data associated with a vertex
template <class VertexObject, class EdgeObject>
VertexObject& AdjacencyMatrixGraph<VertexObject, EdgeObject>::vertexInfo(int v)
{
	if ((v < 0) || (v >= vertexCount()))
		throw GraphVertexOutOfBounds();
	return (*vertexData)[v];
}

//return true if edge exists, false otherwise
template <class VertexObject, class EdgeObject>
bool AdjacencyMatrixGraph<VertexObject, EdgeObject>::hasEdge(int start, int end)
{
	if ((start < 0) || (start >= vertexCount()) || (end < 0)
		|| (end >= vertexCount()))
		throw GraphEdgeOutOfBounds();
	return (edges[start][end] != 0.0);
}

//return info associated with an edge
template <class VertexObject, class EdgeObject>
EdgeObject& AdjacencyMatrixGraph<VertexObject, EdgeObject>::edgeInfo(int start, int end)
{
	if ((start < 0) || (start >= vertexCount()) || (end < 0) || (end >= vertexCount())
		|| (!hasEdge(start, end)))
		throw GraphEdgeOutOfBounds();
	return edgeData[start][end];
}

//return weight of an edge
template <class VertexObject, class EdgeObject>
double AdjacencyMatrixGraph<VertexObject, EdgeObject>::edgeWeight(int start, int end)
{
	if ((start < 0) || (start >= vertexCount()) || (end < 0) || (end >= vertexCount()))
		throw GraphEdgeOutOfBounds();
	return edges[start][end];
}

//return vector of neighbors of vertex
template <class VertexObject, class EdgeObject>
vector<int> AdjacencyMatrixGraph<VertexObject, EdgeObject>::neighbors(int v)
{
	vector<int> result;
	if ((v < 0) || (v >= vertexCount())) return result;
	for (int i = 0; i < vertexCount(); i++)
		if (edges[v][i] != 0.0) result.push_back(i);
	return result;
}

//displayNeighbors():  displays the neighbors of a vertex into an ostream
template <class VertexObject, class EdgeObject>
void AdjacencyMatrixGraph<VertexObject, EdgeObject>::displayNeighbors(int v, ostream& os)
{
	if ((v < 0) || (v >= vertexCount())) throw GraphVertexOutOfBounds();
	vector<int> neighborList = neighbors(v);
	printVector(neighborList, os);
	cout << endl;
}

//set the information of a vertex in a graph
template <class VertexObject, class EdgeObject>
void AdjacencyMatrixGraph<VertexObject, EdgeObject>::setVertexInfo(int v, VertexObject& info)
{
	if ((v < 0) || (v >= vertexCount())) return;
	(*vertexData)[v] = info;
}

//set the information of an edge in a graph
template <class VertexObject, class EdgeObject>
void AdjacencyMatrixGraph<VertexObject, EdgeObject>::setEdgeInfo(int start, int end, EdgeObject& info)
{
	if ((start < 0) || (start >= vertexCount()) || (end < 0) || (end >= vertexCount()))
		throw GraphEdgeOutOfBounds();
	edgeData[start][end] = info;
}

//delete an edge in the graph using the protected method
template <class VertexObject, class EdgeObject>
void AdjacencyMatrixGraph<VertexObject, EdgeObject>::_deleteEdge(int start, int end)
{
	if ((start < 0) || (start >= vertexCount()) || (end < 0) || (end >= vertexCount())
		|| (!hasEdge(start, end)))
		throw GraphEdgeOutOfBounds();
	edges[start][end] = 0.0;
}

//delete an edge in the graph using the public method
template <class VertexObject, class EdgeObject>
void AdjacencyMatrixGraph<VertexObject, EdgeObject>::deleteEdge(int start, int end)
{
	_deleteEdge(start, end);
	_deleteEdge(end, start);
}

//add an edge in the graph that doesn't contain any info
template <class VertexObject, class EdgeObject>
void AdjacencyMatrixGraph<VertexObject, EdgeObject>::addEdge(int start, int end)
{
	if ((start < 0) || (start >= vertexCount()) || (end < 0) || (end >= vertexCount()))
		throw GraphEdgeOutOfBounds();
	if (hasEdge(start, end)) throw GraphDuplicateEdge();
	edges[start][end] = 1;
	edges[end][start] = 1;
}

//add an edge in the graph that contains info
template <class VertexObject, class EdgeObject>
void AdjacencyMatrixGraph<VertexObject, EdgeObject>::addEdge(int start, int end, EdgeObject info)
{
	if ((start < 0) || (start >= vertexCount()) || (end < 0) || (end >= vertexCount()))
		throw GraphEdgeOutOfBounds();
	if (hasEdge(start, end)) throw GraphDuplicateEdge();
	edges[start][end] = 1;
	edgeData[start][end] = info;

	edges[end][start] = 1;
	edgeData[end][start] = info;
}

//breadthFirstSearch():  implements a bfs across the graph and stores the order of vertices
//visited into a vector and returns to the calling function
template <class VectorObject, class EdgeObject>
vector<int> AdjacencyMatrixGraph<VectorObject, EdgeObject>::breadthFirstSearch(int u, vector<int> &parent)
{
	int v;											//node popped from the queue
	int w;											//neighbor node of v
	vector<int> nbors;								//neighbors vector of v
	int BFSnum = 1;

	//breadth-first-serach counter starts at 1 (value for u)
	std::queue<int> Q;
	vector<int> BFSnums(this->vertexCount(), 0);	//initialize a vector of visited vertices to 0
	BFSnums[u] = 1;									//counter starts at 1
	Q.push(u);
	parent[u] = -1;
	while (!Q.empty())
	{
		v = Q.front();
		Q.pop();
		nbors = neighbors(v);
		for (unsigned int i = 0; i < nbors.size(); i++)
		{
			w = nbors[i];
			if (BFSnums[w] == 0)
			{
				BFSnum = BFSnum + 1;
				BFSnums[w] = BFSnum;
				Q.push(w);
				parent[w] = v;
			}
		}
	}
	return BFSnums;
}

//depthFirstSearch():  implements a dfs across the graph and stores the order of vertices
//of visited.  Updates a parent node vector to show the parents of each nodes.
template <class VertexObject, class EdgeObject>
vector<int> AdjacencyMatrixGraph<VertexObject, EdgeObject>::depthFirstSearch(int u, vector<int> &parent)
{
	int v;							//node popped from stack
	int w;							//neighbor of node v
	int depth = 1;					//depth-first-search number
	vector<int> nbors;				//array of neighbors for node v

	vector<int> DFSnums(this->vertexCount(), 0);	//initialize a vector of visited vertices to 0
	std::stack<int> S;
	DFSnums[u] = depth;
	S.push(u);
	parent[u] = -1;
	while (!S.empty())
	{
		v = S.top();
		S.pop();
		nbors = neighbors(v);
		for (unsigned int i = 0; i < nbors.size(); i++)
		{
			w = nbors[i];
			if (DFSnums[w] == 0)
			{
				depth = depth + 1;
				DFSnums[w] = depth;
				S.push(w);
				parent[w] = v;
			}
		}
	}
	return DFSnums;
}
