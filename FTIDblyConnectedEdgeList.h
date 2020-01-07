//////////////////////////////////////////////////////////////////////
//
// FTIFTIDblyConnectedEdgeList.h: Class defined to store information for 
// local polygon triangluation.
//
// Company: Forming Technologies Inc.
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

//#include "tchar.h"
////- FTIStitchDLL module headers -------------------------------------------
//#include "FTIDblVec3D.h"
//#include "FTIVector.h"
//#include "FTIObjectList.h"
//#include "FTIScrapEntity.h" 
//-------------------------------------------------------------------------

class FTIHalfEdge;
class FTIPolyFace;

//====================================================================
// DESCRIPTION: Class for defining the vertex elements for 
// the doubly connected edge list
//====================================================================
class FTIPolyVertex
{

public:

	//====================================================================
	// DESCRIPTION: Default constructor
	//====================================================================
	FTIPolyVertex()
	{
		m_adNode[0] = 0;
		m_adNode[1] = 0;
		m_pEdge = 0;
	}

	//====================================================================
	// DESCRIPTION: Constructor from an array of two points
	//====================================================================
	explicit FTIPolyVertex(double* i_adNode)
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
	FTIPolyVertex(double i_dCoorX, double i_dCoorY)
	{
		m_adNode[0] = i_dCoorX;
		m_adNode[1] = i_dCoorY;
		m_pEdge = 0;
	}

	//====================================================================
	// DESCRIPTION: Destructor
	//====================================================================
	~FTIPolyVertex()
	{
		//std::cout << "Vertex Delete \n" << std::endl;

		//delete m_pEdge;
		//m_pEdge = 0;
	}


	//====================================================================
	// DESCRIPTION: Copy Constructor
	//====================================================================
	FTIPolyVertex(const FTIPolyVertex &i_oFTIPolyVertex)
	{

		m_adNode[0] = i_oFTIPolyVertex.m_adNode[0];
		m_adNode[1] = i_oFTIPolyVertex.m_adNode[1];
		m_pEdge = i_oFTIPolyVertex.m_pEdge;
	};

	//====================================================================
	// DESCRIPTION: Assignment operator
	//====================================================================
	FTIPolyVertex &operator=(const FTIPolyVertex &i_oFTIPolyVertex)
	{
		if(this != &i_oFTIPolyVertex)
		{
			m_adNode[0] = i_oFTIPolyVertex.m_adNode[0];
			m_adNode[1] = i_oFTIPolyVertex.m_adNode[1];
			m_pEdge = i_oFTIPolyVertex.m_pEdge;
		};
		return *this;
	};

	//====================================================================
	// DESCRIPTION: Boolean equal - return true if equal to input vertex
	//====================================================================
	bool operator==(const FTIPolyVertex& i_oRHSFTIPolyVertex) const
	{
		return ((m_adNode[0] == i_oRHSFTIPolyVertex.m_adNode[0]) && (m_adNode[1] == i_oRHSFTIPolyVertex.m_adNode[1]));
	}

	//====================================================================
	// DESCRIPTION: Boolean not equal - return true if not equal to input vertex
	//====================================================================
	bool operator!=(const FTIPolyVertex& i_oRHSFTIPolyVertex) const
	{
		return ((m_adNode[0] != i_oRHSFTIPolyVertex.m_adNode[0]) || (m_adNode[1] != i_oRHSFTIPolyVertex.m_adNode[1]));
	}

	//====================================================================
	// DESCRIPTION: Boolean < - return true if lower in order than input vertex
	//====================================================================
	bool operator<( const FTIPolyVertex i_oOtherFTIPolyVertex) const
	{
		if (m_adNode[1] < i_oOtherFTIPolyVertex.m_adNode[1])
		{
			return true;
		}
		else if ((m_adNode[1] == i_oOtherFTIPolyVertex.m_adNode[1])&&(i_oOtherFTIPolyVertex.m_adNode[0] < m_adNode[0]))
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
	bool operator>( const FTIPolyVertex i_oOtherFTIPolyVertex) const
	{
		if (m_adNode[1] > i_oOtherFTIPolyVertex.m_adNode[1])
		{
			return true;
		}
		else if ((m_adNode[1] == i_oOtherFTIPolyVertex.m_adNode[1])&&(i_oOtherFTIPolyVertex.m_adNode[0] > m_adNode[0]))
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
	class FTIPolyVertexAscendingOrder
	{
	public:
		bool operator() (const FTIPolyVertex* i_poFTIPolyVertex1,const FTIPolyVertex* i_poFTIPolyVertex2)
		{
			return *i_poFTIPolyVertex1 < *i_poFTIPolyVertex2;
		};
	};

	//====================================================================
	// DESCRIPTION: Class used to order vector of vertex nodes in descending order
	//====================================================================
	class FTIPolyVertexDescendingOrder
	{
	public:
		bool operator() (const FTIPolyVertex* i_poFTIPolyVertex1,const FTIPolyVertex* i_poFTIPolyVertex2)
		{
			return *i_poFTIPolyVertex1 > *i_poFTIPolyVertex2;
		};
	};

	//====================================================================
	// DESCRIPTION: set new edge for vertex node
	//====================================================================
	void setEdge(FTIHalfEdge* &i_poNewEdge){ this->m_pEdge = i_poNewEdge; }

	//====================================================================
	// DESCRIPTION: Get the coordinates of the vertex (x,y)
	//====================================================================
	double* getNode() { return m_adNode; }

	//====================================================================
	// DESCRIPTION: Pointer to arbitrary edge that has vertex as origin
	//====================================================================
	FTIHalfEdge* getEdge() const { return m_pEdge; }

private:

	double m_adNode[2]; // (x,y) coordinate of node
	FTIHalfEdge* m_pEdge; // pointer to an edge from the FTIPolyVertex
};

FTIPolyVertex* GetMax(FTIPolyVertex* i_poFTIPolyVertex1, FTIPolyVertex* i_poFTIPolyVertex2);

FTIPolyVertex* GetMin(FTIPolyVertex* i_poFTIPolyVertex1, FTIPolyVertex* i_poFTIPolyVertex2);

double Orientation(FTIPolyVertex* i_poFTIPolyVertex1, FTIPolyVertex* i_poFTIPolyVertex2, FTIPolyVertex* i_poFTIPolyVertex3);

//====================================================================
// DESCRIPTION: Class for defining the half edge elements for 
// the doubly connected edge list
//====================================================================	
class FTIHalfEdge
{

public:

	//====================================================================
	// DESCRIPTION:Defualt constructor
	//====================================================================
	FTIHalfEdge() : m_pOrigin(0), m_pTwin(0),m_pNext(0), m_pPrev(0), m_pIncidentFTIPolyFace(0){}

	//====================================================================
	// DESCRIPTION: Construct a half edge from a vertex
	//====================================================================
	explicit FTIHalfEdge(FTIPolyVertex* i_poFTIPolyVertex) :  m_pOrigin(0), m_pTwin(0), m_pNext(0), m_pPrev(0), m_pIncidentFTIPolyFace(0)
	{
		m_pOrigin = i_poFTIPolyVertex;
	}

	//====================================================================
	// DESCRIPTION: Destructor
	//====================================================================
	~FTIHalfEdge()
	{
	}

	//====================================================================
	// DESCRIPTION: Copy Constructor
	//====================================================================
	FTIHalfEdge(const FTIHalfEdge &i_oEdge)
	{
		m_pOrigin = i_oEdge.m_pOrigin;
		m_pNext = i_oEdge.m_pNext;
		m_pPrev = i_oEdge.m_pPrev;
		m_pTwin = i_oEdge.m_pTwin;
		m_pIncidentFTIPolyFace = i_oEdge.m_pIncidentFTIPolyFace;
	}

	//====================================================================
	// DESCRIPTION: Assignment operator
	//====================================================================
	FTIHalfEdge &operator=(const FTIHalfEdge &i_oEdge)
	{
		if(this != &i_oEdge)
		{
			m_pOrigin = i_oEdge.m_pOrigin;
			m_pNext = i_oEdge.m_pNext;
			m_pPrev = i_oEdge.m_pPrev;
			m_pTwin = i_oEdge.m_pTwin;
			m_pIncidentFTIPolyFace = i_oEdge.m_pIncidentFTIPolyFace;
		};
		return *this;
	}

	//====================================================================
	// DESCRIPTION: Boolean == - test to see if edges are equal by looking 
	// at the origin and origin of twin half edge
	//====================================================================
	bool operator==(const FTIHalfEdge& i_oRHSEdge) const
	{
		return ((*m_pOrigin == *(i_oRHSEdge.m_pOrigin)) && (*(m_pTwin->getOrigin()) == *(i_oRHSEdge.m_pTwin->getOrigin() ) )); 
	};

	//====================================================================
	// DESCRIPTION: Boolean != - test to see if edges are equal by looking 
	// at the origin and origin of twin half edge
	//====================================================================
	bool operator!=(const FTIHalfEdge& i_oRHSEdge) const
	{
		return ((*m_pOrigin != *(i_oRHSEdge.m_pOrigin)) || (*(m_pTwin->getOrigin()) != *(i_oRHSEdge.m_pTwin->getOrigin() ) )); 
	};

	//====================================================================
	// DESCRIPTION: Boolean < - tests to see if edge is "less than" input edge
	// by determining order based on edge origin.
	//====================================================================
	bool operator<( const FTIHalfEdge i_oOtherFTIHalfEdge) const
	{
		if (*(m_pOrigin) < *(i_oOtherFTIHalfEdge.getOrigin()))
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
	bool operator>( const FTIHalfEdge i_oOtherFTIHalfEdge) const
	{
		if (*(m_pOrigin) > *(i_oOtherFTIHalfEdge.getOrigin()))
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
	bool operator%( const FTIHalfEdge i_oOtherFTIHalfEdge) const
	{
		double* EdgeStart = m_pOrigin->getNode();
		double* EdgeEnd = m_pTwin->m_pOrigin->getNode();
		double* OtherEdgeStart = i_oOtherFTIHalfEdge.getOrigin()->getNode();
		double* OtherEdgeEnd = i_oOtherFTIHalfEdge.getTwin()->getOrigin()->getNode();

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
	class FTIHalfEdgeAscendingOrder
	{
	public:
		bool operator() (const FTIHalfEdge* i_poFTIHalfEdge1,const FTIHalfEdge* i_poFTIHalfEdge2)
		{
			return *i_poFTIHalfEdge1 < *i_poFTIHalfEdge2;
		};
	};

	//====================================================================
	// DESCRIPTION: Class used to sort edges in descending order
	//====================================================================
	class FTIHalfEdgeDescendingOrder
	{
	public:
		bool operator() (const FTIHalfEdge* i_poFTIHalfEdge1,const FTIHalfEdge* i_poFTIHalfEdge2)
		{
			return *i_poFTIHalfEdge1 > *i_poFTIHalfEdge2;
		};
	};

	//====================================================================
	// DESCRIPTION: Set the origin of a half edge
	//====================================================================
	void setOrigin(FTIPolyVertex *i_poNewOrigin){ this->m_pOrigin = i_poNewOrigin; }

	//====================================================================
	// DESCRIPTION: Set the twin of a half edge
	//====================================================================
	void setTwin(FTIHalfEdge *i_poNewTwin){
		 this->m_pTwin = i_poNewTwin;
		i_poNewTwin->m_pTwin = this;
	}

	//====================================================================
	// DESCRIPTION: Set next half edge
	//====================================================================
	void setNext(FTIHalfEdge *i_poNewNext){
		 this->m_pNext = i_poNewNext;
	}

	//====================================================================
	// DESCRIPTION: set previous half edge
	//====================================================================
	void setPrev(FTIHalfEdge *i_poNewPrev){
		 this->m_pPrev = i_poNewPrev;
	}

	//====================================================================
	// DESCRIPTION: Set incident face to the left of the half edge
	//====================================================================
	void setFTIPolyFace(FTIPolyFace *&i_poNewFTIPolyFace){	this->m_pIncidentFTIPolyFace = i_poNewFTIPolyFace; }

	//====================================================================
	// DESCRIPTION: get the pointer to origin vertex
	//====================================================================
	FTIPolyVertex* getOrigin() const { return m_pOrigin; }

	//====================================================================
	// DESCRIPTION: Get pointer to the next half edge
	//====================================================================
	FTIHalfEdge* getNext() const { return m_pNext; }

	//====================================================================
	// DESCRIPTION: Get the pointer to the previous half edge
	//====================================================================
	FTIHalfEdge* getPrev() const { return m_pPrev; }

	//====================================================================
	// DESCRIPTION: Get pointer to twin half edge
	//====================================================================
	FTIHalfEdge* getTwin() const { return m_pTwin; }

	//====================================================================
	// DESCRIPTION: Get pointer to incident face which lies to the left of the half edge
	//====================================================================
	FTIPolyFace* getFace()	const { return m_pIncidentFTIPolyFace; }

private:

	FTIPolyVertex* m_pOrigin; // pointer to origin of edge
	FTIHalfEdge* m_pTwin; // pointer to edge twin
	FTIHalfEdge* m_pNext; // pointer to next edge on FTIPolyFace
	FTIHalfEdge* m_pPrev; // pointer to previous edge on FTIPolyFace
	FTIPolyFace* m_pIncidentFTIPolyFace; // pointer to FTIPolyFace lying to the left of the edge
};

bool IsVertexLeftofEdge(FTIPolyVertex *i_poVertex, FTIHalfEdge *i_poHalfEdge);
bool IsVertexRightofEdge(FTIPolyVertex *i_poVertex, FTIHalfEdge *i_poHalfEdge);
//====================================================================
// DESCRIPTION: Class used to define a polynomial face in the DCEL
//====================================================================
class FTIPolyFace
{
public:

	//====================================================================
	// DESCRIPTION: Default Constructor 
	//====================================================================
	FTIPolyFace() : m_pIncidentEdge(0) {}

	//====================================================================
	// DESCRIPTION: Constructor from a half edge 
	//====================================================================
	explicit FTIPolyFace(FTIHalfEdge* i_poRHSEdge)
	{
		m_pIncidentEdge = i_poRHSEdge;
	}

	//====================================================================
	// DESCRIPTION: Destructor
	//====================================================================
	~FTIPolyFace()
	{
		//std::cout << "face delete \n" << std::endl;
		//delete m_pIncidentEdge;
		//m_pIncidentEdge = 0;
	}

	//====================================================================
	// DESCRIPTION: Assignment operator
	//====================================================================
	FTIPolyFace(const FTIPolyFace &i_oRHSFTIPolyFace)
	{
		m_pIncidentEdge = i_oRHSFTIPolyFace.m_pIncidentEdge;
	}

	//====================================================================
	// DESCRIPTION: Assignment operator
	//====================================================================
	FTIPolyFace &operator=(const FTIPolyFace &i_oRHSFTIPolyFace)
	{
		if(this != &i_oRHSFTIPolyFace)
			m_pIncidentEdge = i_oRHSFTIPolyFace.m_pIncidentEdge;
		return *this;
	}

	//====================================================================
	// DESCRIPTION: Boolean == - tests to see if faces are equal by comparing
	// their incident edges
	//====================================================================
	bool operator==(const FTIPolyFace& i_oRHSFTIPolyFace) const
	{
		return (*(m_pIncidentEdge) == *(i_oRHSFTIPolyFace.m_pIncidentEdge)); 
	};

	//====================================================================
	// DESCRIPTION: Boolean != - tests to see if faces are not equal by comparing
	// their incident edges
	//====================================================================
	bool operator!=(const FTIPolyFace& i_oRHSFTIPolyFace) const
	{
		return (*(m_pIncidentEdge) != *(i_oRHSFTIPolyFace.m_pIncidentEdge)); 
	};

	//====================================================================
	// DESCRIPTION: Sets edge for face
	//====================================================================
	void setEdge(FTIHalfEdge* &i_poNewEdge) { this->m_pIncidentEdge = i_poNewEdge; }

	//====================================================================
	// DESCRIPTION: returns pointer to incident edge to face
	//====================================================================
	FTIHalfEdge* getEdge() const { return m_pIncidentEdge; }

private:

	//int m_nAttribute; // null?
	FTIHalfEdge* m_pIncidentEdge; // pointer to an edge on its bounday
};

//====================================================================
// DESCRIPTION: This class is for the doubly connected edge list data
// structure. This allows for efficient storage and access to elements 
// of the polygon including vertices, directed half edges and faces.
//====================================================================
class FTIDblyConnectedEdgeList
{

public:

	//====================================================================
	// DESCRIPTION: Default constructor
	//====================================================================
	FTIDblyConnectedEdgeList()
		: m_pFTIPolyVertexNodes(0),
		m_pFTIHalfEdgeVect(0),
		m_pFTIPolyFaceVect(0) {}

	//====================================================================
	// DESCRIPTION: Constructor of DCEL from a vector of vertices in counter
	// clockwise order around the polygon: { x0, y0, x1, y1, ... , xn, yn} 
	//====================================================================
	explicit FTIDblyConnectedEdgeList(std::vector<double> i_adPolygonVertices)
	{
		int nSize = i_adPolygonVertices.size() / 2;
		
		// Calls private initialize function
		InitializeDCEL(i_adPolygonVertices, nSize);
	}

	//====================================================================
	// DESCRIPTION: Destructor
	//====================================================================
	~FTIDblyConnectedEdgeList()
	{
		//std::cout << "DCEL delete \n" << std::endl;

		for(unsigned int i = 0; i < m_pFTIPolyVertexNodes.size(); i++)
		{
			delete m_pFTIPolyVertexNodes[i];
			m_pFTIPolyVertexNodes[i] = 0;
		};
		m_pFTIPolyVertexNodes.clear();

		for(unsigned int i = 0; i < m_pFTIHalfEdgeVect.size(); i++)
		{
			delete m_pFTIHalfEdgeVect[i];
			m_pFTIHalfEdgeVect[i] = 0;
		};
		m_pFTIHalfEdgeVect.clear();

		for(unsigned int i = 0; i < m_pFTIPolyFaceVect.size(); i++)
		{
			delete m_pFTIPolyFaceVect[i];
			m_pFTIPolyFaceVect[i] = 0;
		};
		m_pFTIPolyFaceVect.clear();
	};
	
	//====================================================================
	// DESCRIPTION: Copy Constructor
	//====================================================================
	FTIDblyConnectedEdgeList(const FTIDblyConnectedEdgeList &i_oRHSDCEL)
	{
		m_pFTIPolyVertexNodes = i_oRHSDCEL.m_pFTIPolyVertexNodes;
		m_pFTIHalfEdgeVect = i_oRHSDCEL.m_pFTIHalfEdgeVect;
		m_pFTIPolyFaceVect = i_oRHSDCEL.m_pFTIPolyFaceVect;
	}

	//====================================================================
	// DESCRIPTION: Assignment operator
	//====================================================================
	FTIDblyConnectedEdgeList &operator=(const FTIDblyConnectedEdgeList &i_oRHSDCEL)
	{
		if(this != &i_oRHSDCEL)
		{
			m_pFTIPolyVertexNodes = i_oRHSDCEL.m_pFTIPolyVertexNodes;
			m_pFTIHalfEdgeVect = i_oRHSDCEL.m_pFTIHalfEdgeVect;
			m_pFTIPolyFaceVect = i_oRHSDCEL.m_pFTIPolyFaceVect;
		};
		return *this;
	}

	//====================================================================
	// DESCRIPTION: Function for inputing a new edge into the DCEL. 
	// INPUT: pointer to vertex 1, pointer to vertex 2.
	// Note: v1 > v2.
	//====================================================================
	void InsertNewEdge(FTIPolyVertex* &i_poFTIPolyVertex1, FTIPolyVertex* &i_poFTIPolyVertex2);

	//====================================================================
	// DESCRIPTION: Return vector of pointers to the vertex notes
	//====================================================================
	std::vector<FTIPolyVertex*> getVertices() {return  m_pFTIPolyVertexNodes; }

	//====================================================================
	// DESCRIPTION: Return vector of pointers to half edges 
	//====================================================================
	std::vector<FTIHalfEdge*> getHalfEdges() {return  m_pFTIHalfEdgeVect; }

	//====================================================================
	// DESCRIPTION: Return vector of pointers of all the polygon faces
	//====================================================================
	std::vector<FTIPolyFace*> getFaces() {return m_pFTIPolyFaceVect; }

	//====================================================================
	// DESCRIPTION: Return the number of faces
	//====================================================================
	int getNumVertex() {return m_pFTIPolyVertexNodes.size();}

	//====================================================================
	// DESCRIPTION: Return the number of faces
	//====================================================================
	int getNumEdges() {return m_pFTIHalfEdgeVect.size();}

	//====================================================================
	// DESCRIPTION: Return the number of faces
	//====================================================================
	int getNumFaces() {return m_pFTIPolyFaceVect.size();}

	//====================================================================
	// DESCRIPTION: Write DCEL to output file
	//====================================================================
	void WriteToFile(std::ofstream &outputFile);

private:
	
	//====================================================================
	// DESCRIPTION: Private funtion to initialize DCEL
	//====================================================================
	void InitializeDCEL(std::vector<double> i_adPolygonVerticies, int i_nSize);

	std::vector<FTIPolyVertex*> m_pFTIPolyVertexNodes; // vector of pointers to vertices
	std::vector<FTIHalfEdge*> m_pFTIHalfEdgeVect; // vector of pointers to half edges
	std::vector<FTIPolyFace*> m_pFTIPolyFaceVect; // vector of pointers to faces

};