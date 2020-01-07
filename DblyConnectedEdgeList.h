//////////////////////////////////////////////////////////////////////
//
// DblyConnectedEdgeList.h: Class defined to store information for 
// local polygon triangluation..
//
// REVISIONS:
//  Feb. 10, 2017 created (Alex Ashbourne)
//
//////////////////////////////////////////////////////////////////////

#pragma once

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif 

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>


class HalfEdge;
class PolyFace;

//====================================================================
// DESCRIPTION: Class for defining the vertex elements for 
// the doubly connected edge list
//====================================================================
class PolyVertex
{

public:

	//====================================================================
	// DESCRIPTION: Default constructor
	//====================================================================
	PolyVertex()
	{
		m_adNode[0] = 0;
		m_adNode[1] = 0;
		m_pEdge = 0;
	}

	//====================================================================
	// DESCRIPTION: Constructor from an array of two points
	//====================================================================
	explicit PolyVertex(double* i_adNode)
	{
		for(int i = 0; i < 2; i++)
		{
			m_adNode[i] = i_adNode[i];
		}

		m_pEdge = 0;
	}

	//====================================================================
	// DESCRIPTION: Constructor from two coordinates
	//====================================================================
	PolyVertex(double i_dCoorX, double i_dCoorY)
	{
		m_adNode[0] = i_dCoorX;
		m_adNode[1] = i_dCoorY;
		m_pEdge = 0;
	}

	//====================================================================
	// DESCRIPTION: Destructor
	//====================================================================
	~PolyVertex()
	{
		//std::cout << "Vertex Delete \n" << std::endl;

		//delete m_pEdge;
		//m_pEdge = 0;
	}


	//====================================================================
	// DESCRIPTION: Copy Constructor
	//====================================================================
	PolyVertex(const PolyVertex &i_oPolyVertex)
	{

		m_adNode[0] = i_oPolyVertex.m_adNode[0];
		m_adNode[1] = i_oPolyVertex.m_adNode[1];
		m_pEdge = i_oPolyVertex.m_pEdge;
	};

	//====================================================================
	// DESCRIPTION: Assignment operator
	//====================================================================
	PolyVertex &operator=(const PolyVertex &i_oPolyVertex)
	{
		if(this != &i_oPolyVertex)
		{
			m_adNode[0] = i_oPolyVertex.m_adNode[0];
			m_adNode[1] = i_oPolyVertex.m_adNode[1];
			m_pEdge = i_oPolyVertex.m_pEdge;
		};
		return *this;
	};

	//====================================================================
	// DESCRIPTION: Boolean equal - return true if equal to input vertex
	//====================================================================
	bool operator==(const PolyVertex& i_oRHSPolyVertex) const
	{
		return ((m_adNode[0] == i_oRHSPolyVertex.m_adNode[0]) && (m_adNode[1] == i_oRHSPolyVertex.m_adNode[1]));
	}

	//====================================================================
	// DESCRIPTION: Boolean not equal - return true if not equal to input vertex
	//====================================================================
	bool operator!=(const PolyVertex& i_oRHSPolyVertex) const
	{
		return ((m_adNode[0] != i_oRHSPolyVertex.m_adNode[0]) || (m_adNode[1] != i_oRHSPolyVertex.m_adNode[1]));
	}

	//====================================================================
	// DESCRIPTION: Boolean < - return true if lower in order than input vertex
	//====================================================================
	bool operator<( const PolyVertex i_oOtherPolyVertex) const
	{
		if (m_adNode[1] < i_oOtherPolyVertex.m_adNode[1])
		{
			return true;
		}
		else if ((m_adNode[1] == i_oOtherPolyVertex.m_adNode[1])&&(i_oOtherPolyVertex.m_adNode[0] < m_adNode[0]))
		{
			return true;
		}
		else
		{
			return false;
		};
	}

	//====================================================================
	// DESCRIPTION: Boolean > - return true if higher in order than input vertex
	//====================================================================
	bool operator>( const PolyVertex i_oOtherPolyVertex) const
	{
		if (m_adNode[1] > i_oOtherPolyVertex.m_adNode[1])
		{
			return true;
		}
		else if ((m_adNode[1] == i_oOtherPolyVertex.m_adNode[1])&&(i_oOtherPolyVertex.m_adNode[0] > m_adNode[0]))
		{
			return true;
		}
		else
		{
			return false;
		};
	}

	//====================================================================
	// DESCRIPTION: Class used to order vector of vertex nodes in ascending order 
	//====================================================================
	class PolyVertexAscendingOrder
	{
	public:
		bool operator() (const PolyVertex* i_poPolyVertex1,const PolyVertex* i_poPolyVertex2)
		{
			return *i_poPolyVertex1 < *i_poPolyVertex2;
		};
	};

	//====================================================================
	// DESCRIPTION: Class used to order vector of vertex nodes in descending order
	//====================================================================
	class PolyVertexDescendingOrder
	{
	public:
		bool operator() (const PolyVertex* i_poPolyVertex1,const PolyVertex* i_poPolyVertex2)
		{
			return *i_poPolyVertex1 > *i_poPolyVertex2;
		};
	};

	//====================================================================
	// DESCRIPTION: set new edge for vertex node
	//====================================================================
	void setEdge(HalfEdge* &i_poNewEdge){ this->m_pEdge = i_poNewEdge; }

	//====================================================================
	// DESCRIPTION: Get the coordinates of the vertex (x,y)
	//====================================================================
	double* getNode() { return m_adNode; }

	//====================================================================
	// DESCRIPTION: Pointer to arbitrary edge that has vertex as origin
	//====================================================================
	HalfEdge* getEdge() const { return m_pEdge; }

private:

	double m_adNode[2]; // (x,y) coordinate of node
	HalfEdge* m_pEdge; // pointer to an edge from the PolyVertex
};

PolyVertex* GetMax(PolyVertex* i_poPolyVertex1, PolyVertex* i_poPolyVertex2);

PolyVertex* GetMin(PolyVertex* i_poPolyVertex1, PolyVertex* i_poPolyVertex2);

double Orientation(PolyVertex* i_poPolyVertex1, PolyVertex* i_poPolyVertex2, PolyVertex* i_poPolyVertex3);

//====================================================================
// DESCRIPTION: Class for defining the half edge elements for 
// the doubly connected edge list
//====================================================================	
class HalfEdge
{

public:

	//====================================================================
	// DESCRIPTION:Defualt constructor
	//====================================================================
	HalfEdge() : m_pOrigin(0), m_pTwin(0),m_pNext(0), m_pPrev(0), m_pIncidentPolyFace(0){}

	//====================================================================
	// DESCRIPTION: Construct a half edge from a vertex
	//====================================================================
	explicit HalfEdge(PolyVertex* i_poPolyVertex) :  m_pOrigin(0), m_pTwin(0), m_pNext(0), m_pPrev(0), m_pIncidentPolyFace(0)
	{
		m_pOrigin = i_poPolyVertex;
	}

	//====================================================================
	// DESCRIPTION: Destructor
	//====================================================================
	~HalfEdge()
	{
	}

	//====================================================================
	// DESCRIPTION: Copy Constructor
	//====================================================================
	HalfEdge(const HalfEdge &i_oEdge)
	{
		m_pOrigin = i_oEdge.m_pOrigin;
		m_pNext = i_oEdge.m_pNext;
		m_pPrev = i_oEdge.m_pPrev;
		m_pTwin = i_oEdge.m_pTwin;
		m_pIncidentPolyFace = i_oEdge.m_pIncidentPolyFace;
	}

	//====================================================================
	// DESCRIPTION: Assignment operator
	//====================================================================
	HalfEdge &operator=(const HalfEdge &i_oEdge)
	{
		if(this != &i_oEdge)
		{
			m_pOrigin = i_oEdge.m_pOrigin;
			m_pNext = i_oEdge.m_pNext;
			m_pPrev = i_oEdge.m_pPrev;
			m_pTwin = i_oEdge.m_pTwin;
			m_pIncidentPolyFace = i_oEdge.m_pIncidentPolyFace;
		};
		return *this;
	}

	//====================================================================
	// DESCRIPTION: Boolean == - test to see if edges are equal by looking 
	// at the origin and origin of twin half edge
	//====================================================================
	bool operator==(const HalfEdge& i_oRHSEdge) const
	{
		return ((*m_pOrigin == *(i_oRHSEdge.m_pOrigin)) && (*(m_pTwin->getOrigin()) == *(i_oRHSEdge.m_pTwin->getOrigin() ) )); 
	};

	//====================================================================
	// DESCRIPTION: Boolean != - test to see if edges are equal by looking 
	// at the origin and origin of twin half edge
	//====================================================================
	bool operator!=(const HalfEdge& i_oRHSEdge) const
	{
		return ((*m_pOrigin != *(i_oRHSEdge.m_pOrigin)) || (*(m_pTwin->getOrigin()) != *(i_oRHSEdge.m_pTwin->getOrigin() ) )); 
	};

	//====================================================================
	// DESCRIPTION: Boolean < - tests to see if edge is "less than" input edge
	// by determining order based on edge origin.
	//====================================================================
	bool operator<( const HalfEdge i_oOtherHalfEdge) const
	{
		if (*(m_pOrigin) < *(i_oOtherHalfEdge.getOrigin()))
		{
			return true;
		}
		else
		{
			return false;
		};
	}

	//====================================================================
	// DESCRIPTION: Boolean > - tests to see if edge is "greater than" input edge
	// by determining order based on edge origin.
	//====================================================================
	bool operator>( const HalfEdge i_oOtherHalfEdge) const
	{
		if (*(m_pOrigin) > *(i_oOtherHalfEdge.getOrigin()))
		{
			return true;
		}
		else
		{
			return false;
		};
	}

	//====================================================================
	// DESCRIPTION: Boolean % - this operator is created to order half edges
	// in the binary search tree. We say that one edge is less than another
	// edge by looking at the position of the x-coordinate of the midpoint 
	// of the edge. Might not be the best way, but seems to work fine
	//====================================================================
	bool operator%( const HalfEdge i_oOtherHalfEdge) const
	{
		double* EdgeStart = m_pOrigin->getNode();
		double* EdgeEnd = m_pTwin->m_pOrigin->getNode();
		double* OtherEdgeStart = i_oOtherHalfEdge.getOrigin()->getNode();
		double* OtherEdgeEnd = i_oOtherHalfEdge.getTwin()->getOrigin()->getNode();

		double EdgeXMid = (EdgeStart[0] + EdgeEnd[0]) / 2.0;
		double OtherEdgeXMid = (OtherEdgeStart[0] + OtherEdgeEnd[0]) / 2.0;
		if(EdgeXMid < OtherEdgeXMid)
			return true;
		else
			return false;
		
	}

	//====================================================================
	// DESCRIPTION: Class used to sort edges in ascending order
	//====================================================================
	class HalfEdgeAscendingOrder
	{
	public:
		bool operator() (const HalfEdge* i_poHalfEdge1,const HalfEdge* i_poHalfEdge2)
		{
			return *i_poHalfEdge1 < *i_poHalfEdge2;
		};
	};

	//====================================================================
	// DESCRIPTION: Class used to sort edges in descending order
	//====================================================================
	class HalfEdgeDescendingOrder
	{
	public:
		bool operator() (const HalfEdge* i_poHalfEdge1,const HalfEdge* i_poHalfEdge2)
		{
			return *i_poHalfEdge1 > *i_poHalfEdge2;
		};
	};

	//====================================================================
	// DESCRIPTION: Set the origin of a half edge
	//====================================================================
	void setOrigin(PolyVertex *i_poNewOrigin){ this->m_pOrigin = i_poNewOrigin; }

	//====================================================================
	// DESCRIPTION: Set the twin of a half edge
	//====================================================================
	void setTwin(HalfEdge *i_poNewTwin){
		 this->m_pTwin = i_poNewTwin;
		i_poNewTwin->m_pTwin = this;
	}

	//====================================================================
	// DESCRIPTION: Set next half edge
	//====================================================================
	void setNext(HalfEdge *i_poNewNext){
		 this->m_pNext = i_poNewNext;
	}

	//====================================================================
	// DESCRIPTION: set previous half edge
	//====================================================================
	void setPrev(HalfEdge *i_poNewPrev){
		 this->m_pPrev = i_poNewPrev;
	}

	//====================================================================
	// DESCRIPTION: Set incident face to the left of the half edge
	//====================================================================
	void setPolyFace(PolyFace *&i_poNewPolyFace){	this->m_pIncidentPolyFace = i_poNewPolyFace; }

	//====================================================================
	// DESCRIPTION: get the pointer to origin vertex
	//====================================================================
	PolyVertex* getOrigin() const { return m_pOrigin; }

	//====================================================================
	// DESCRIPTION: Get pointer to the next half edge
	//====================================================================
	HalfEdge* getNext() const { return m_pNext; }

	//====================================================================
	// DESCRIPTION: Get the pointer to the previous half edge
	//====================================================================
	HalfEdge* getPrev() const { return m_pPrev; }

	//====================================================================
	// DESCRIPTION: Get pointer to twin half edge
	//====================================================================
	HalfEdge* getTwin() const { return m_pTwin; }

	//====================================================================
	// DESCRIPTION: Get pointer to incident face which lies to the left of the half edge
	//====================================================================
	PolyFace* getFace()	const { return m_pIncidentPolyFace; }

private:

	PolyVertex* m_pOrigin; // pointer to origin of edge
	HalfEdge* m_pTwin; // pointer to edge twin
	HalfEdge* m_pNext; // pointer to next edge on PolyFace
	HalfEdge* m_pPrev; // pointer to previous edge on PolyFace
	PolyFace* m_pIncidentPolyFace; // pointer to PolyFace lying to the left of the edge
};

bool IsVertexLeftofEdge(PolyVertex *i_poVertex, HalfEdge *i_poHalfEdge);
bool IsVertexRightofEdge(PolyVertex *i_poVertex, HalfEdge *i_poHalfEdge);
//====================================================================
// DESCRIPTION: Class used to define a polynomial face in the DCEL
//====================================================================
class PolyFace
{
public:

	//====================================================================
	// DESCRIPTION: Default Constructor 
	//====================================================================
	PolyFace() : m_pIncidentEdge(0) {}

	//====================================================================
	// DESCRIPTION: Constructor from a half edge 
	//====================================================================
	explicit PolyFace(HalfEdge* i_poRHSEdge)
	{
		m_pIncidentEdge = i_poRHSEdge;
	}

	//====================================================================
	// DESCRIPTION: Destructor
	//====================================================================
	~PolyFace()
	{
		//std::cout << "face delete \n" << std::endl;
		//delete m_pIncidentEdge;
		//m_pIncidentEdge = 0;
	}

	//====================================================================
	// DESCRIPTION: Assignment operator
	//====================================================================
	PolyFace(const PolyFace &i_oRHSPolyFace)
	{
		m_pIncidentEdge = i_oRHSPolyFace.m_pIncidentEdge;
	}

	//====================================================================
	// DESCRIPTION: Assignment operator
	//====================================================================
	PolyFace &operator=(const PolyFace &i_oRHSPolyFace)
	{
		if(this != &i_oRHSPolyFace)
			m_pIncidentEdge = i_oRHSPolyFace.m_pIncidentEdge;
		return *this;
	}

	//====================================================================
	// DESCRIPTION: Boolean == - tests to see if faces are equal by comparing
	// their incident edges
	//====================================================================
	bool operator==(const PolyFace& i_oRHSPolyFace) const
	{
		return (*(m_pIncidentEdge) == *(i_oRHSPolyFace.m_pIncidentEdge)); 
	};

	//====================================================================
	// DESCRIPTION: Boolean != - tests to see if faces are not equal by comparing
	// their incident edges
	//====================================================================
	bool operator!=(const PolyFace& i_oRHSPolyFace) const
	{
		return (*(m_pIncidentEdge) != *(i_oRHSPolyFace.m_pIncidentEdge)); 
	};

	//====================================================================
	// DESCRIPTION: Sets edge for face
	//====================================================================
	void setEdge(HalfEdge* &i_poNewEdge) { this->m_pIncidentEdge = i_poNewEdge; }

	//====================================================================
	// DESCRIPTION: returns pointer to incident edge to face
	//====================================================================
	HalfEdge* getEdge() const { return m_pIncidentEdge; }

private:

	//int m_nAttribute; // null?
	HalfEdge* m_pIncidentEdge; // pointer to an edge on its bounday
};

//====================================================================
// DESCRIPTION: This class is for the doubly connected edge list data
// structure. This allows for efficient storage and access to elements 
// of the polygon including vertices, directed half edges and faces.
//====================================================================
class DblyConnectedEdgeList
{

public:

	//====================================================================
	// DESCRIPTION: Default constructor
	//====================================================================
	DblyConnectedEdgeList()
		: m_pPolyVertexNodes(0),
		m_pHalfEdgeVect(0),
		m_pPolyFaceVect(0) {}

	//====================================================================
	// DESCRIPTION: Constructor of DCEL from a vector of vertices in counter
	// clockwise order around the polygon: { x0, y0, x1, y1, ... , xn, yn} 
	//====================================================================
	explicit DblyConnectedEdgeList(std::vector<double> i_adPolygonVertices)
	{
		int nSize = i_adPolygonVertices.size() / 2;
		
		// Calls private initialize function
		InitializeDCEL(i_adPolygonVertices, nSize);
	}

	//====================================================================
	// DESCRIPTION: Destructor
	//====================================================================
	~DblyConnectedEdgeList()
	{
		//std::cout << "DCEL delete \n" << std::endl;

		for(unsigned int i = 0; i < m_pPolyVertexNodes.size(); i++)
		{
			delete m_pPolyVertexNodes[i];
			m_pPolyVertexNodes[i] = 0;
		};
		m_pPolyVertexNodes.clear();

		for(unsigned int i = 0; i < m_pHalfEdgeVect.size(); i++)
		{
			delete m_pHalfEdgeVect[i];
			m_pHalfEdgeVect[i] = 0;
		};
		m_pHalfEdgeVect.clear();

		for(unsigned int i = 0; i < m_pPolyFaceVect.size(); i++)
		{
			delete m_pPolyFaceVect[i];
			m_pPolyFaceVect[i] = 0;
		};
		m_pPolyFaceVect.clear();
	};
	
	//====================================================================
	// DESCRIPTION: Copy Constructor
	//====================================================================
	DblyConnectedEdgeList(const DblyConnectedEdgeList &i_oRHSDCEL)
	{
		m_pPolyVertexNodes = i_oRHSDCEL.m_pPolyVertexNodes;
		m_pHalfEdgeVect = i_oRHSDCEL.m_pHalfEdgeVect;
		m_pPolyFaceVect = i_oRHSDCEL.m_pPolyFaceVect;
	}

	//====================================================================
	// DESCRIPTION: Assignment operator
	//====================================================================
	DblyConnectedEdgeList &operator=(const DblyConnectedEdgeList &i_oRHSDCEL)
	{
		if(this != &i_oRHSDCEL)
		{
			m_pPolyVertexNodes = i_oRHSDCEL.m_pPolyVertexNodes;
			m_pHalfEdgeVect = i_oRHSDCEL.m_pHalfEdgeVect;
			m_pPolyFaceVect = i_oRHSDCEL.m_pPolyFaceVect;
		};
		return *this;
	}

	//====================================================================
	// DESCRIPTION: Function for inputing a new edge into the DCEL. 
	// INPUT: pointer to vertex 1, pointer to vertex 2.
	// Note: v1 > v2.
	//====================================================================
	void InsertNewEdge(PolyVertex* &i_poPolyVertex1, PolyVertex* &i_poPolyVertex2);

	//====================================================================
	// DESCRIPTION: Return vector of pointers to the vertex notes
	//====================================================================
	std::vector<PolyVertex*> getVertices() {return  m_pPolyVertexNodes; }

	//====================================================================
	// DESCRIPTION: Return vector of pointers to half edges 
	//====================================================================
	std::vector<HalfEdge*> getHalfEdges() {return  m_pHalfEdgeVect; }

	//====================================================================
	// DESCRIPTION: Return vector of pointers of all the polygon faces
	//====================================================================
	std::vector<PolyFace*> getFaces() {return m_pPolyFaceVect; }

	//====================================================================
	// DESCRIPTION: Return the number of faces
	//====================================================================
	int getNumVertex() {return m_pPolyVertexNodes.size();}

	//====================================================================
	// DESCRIPTION: Return the number of faces
	//====================================================================
	int getNumEdges() {return m_pHalfEdgeVect.size();}

	//====================================================================
	// DESCRIPTION: Return the number of faces
	//====================================================================
	int getNumFaces() {return m_pPolyFaceVect.size();}

	//====================================================================
	// DESCRIPTION: Write DCEL to output file
	//====================================================================
	void WriteToFile(std::ofstream &outputFile);

private:
	
	//====================================================================
	// DESCRIPTION: Private funtion to initialize DCEL
	//====================================================================
	void InitializeDCEL(std::vector<double> i_adPolygonVerticies, int i_nSize);

	std::vector<PolyVertex*> m_pPolyVertexNodes; // vector of pointers to vertices
	std::vector<HalfEdge*> m_pHalfEdgeVect; // vector of pointers to half edges
	std::vector<PolyFace*> m_pPolyFaceVect; // vector of pointers to faces

};