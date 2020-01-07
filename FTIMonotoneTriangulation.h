//////////////////////////////////////////////////////////////////////
//
// MonotoneTriangulation.h: This class will take as input a doubly 
// connected edge list for a polygon that has been subdivided into
// y-monotone sub-polygons. It will perform a triangulation on the 
// sub polygons
//
// REVISIONS:
//  Feb. 16, 2017 created (Alex Ashbourne)
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


#include "DblyConnectedEdgeList.h"
#include "MonotonePolygon.h"

//====================================================================
// DESCRIPTION: This class allows for the efficient triangulation of 
// a monotone polygon. We can triangulate any simple polygon, provided 
// we have access to the monotono subdivision.
//====================================================================
class MonotoneTriangulation
{
public:

	//====================================================================
	// DESCRIPTION: Defualt constructor
	//====================================================================
	MonotoneTriangulation() : m_poMonotoneDCEL(0) {}

	//====================================================================
	// DESCRIPTION: Constructor from DCEL - assumed to have monotone subdivision
	//====================================================================
	explicit MonotoneTriangulation(DblyConnectedEdgeList *&i_poInputDCEL)
	{
		m_poMonotoneDCEL = i_poInputDCEL;
	}

	//====================================================================
	// DESCRIPTION: Constructor from monotone polygon object - assumed to 
	// already have monotone subdivision
	//====================================================================
	explicit MonotoneTriangulation(MonotonePolygon &i_poMonotonePolygon)
	{
		m_poMonotoneDCEL = i_poMonotonePolygon.getDCEL();
	}

	//====================================================================
	// DESCRIPTION: Destructor
	//====================================================================
	~MonotoneTriangulation()
	{
		//std::cout << "Triangulation delete \n" << std::endl;
		if(!((m_poMonotoneDCEL->getNumVertex() == 0) &&(m_poMonotoneDCEL->getNumEdges() == 0) &&(m_poMonotoneDCEL->getNumFaces() == 0)))
		{
			//std::cout << "Deleting DCEL \n" << std::endl;
			delete m_poMonotoneDCEL;
			m_poMonotoneDCEL = nullptr;
		};
	};

	//====================================================================
	// DESCRIPTION: Copy Constructor
	//====================================================================
	MonotoneTriangulation(const MonotoneTriangulation &i_oRHSMonoTriangle)
	{
		m_poMonotoneDCEL = i_oRHSMonoTriangle.m_poMonotoneDCEL;
	}

	//====================================================================
	// DESCRIPTION: Assignment operator
	//====================================================================
	MonotoneTriangulation &operator= (const MonotoneTriangulation &i_oRHSMonoTriangle)
	{
		if(this != &i_oRHSMonoTriangle)
			m_poMonotoneDCEL = i_oRHSMonoTriangle.m_poMonotoneDCEL;
		return *this;
	}

	//====================================================================
	// DESCRIPTION: Get pointer to DCEL
	//====================================================================
	DblyConnectedEdgeList* getDCEL() {return m_poMonotoneDCEL;}

	//====================================================================
	// DESCRIPTION: Public function to call private function to triangulate
	// a monotone polygon.
	//====================================================================
	void TriangulatePolygon();

private:

	//====================================================================
	// DESCRIPTION: Function to get all verticies in CCW order around a face
	// INPUT: Pointer to query face
	//====================================================================
	std::vector<PolyVertex*> GetAllVertices(PolyFace *i_poPolyFace);

	//====================================================================
	// DESCRIPTION: Function to get all verticies in left chain of a face
	// INPUT: Vector of pointers to vertices on face, pointer to query face
	//====================================================================
	std::vector<PolyVertex*> GetLeftChain(std::vector<PolyVertex*> i_oAllVertices, PolyFace *i_poPolyFace);

	//====================================================================
	// DESCRIPTION: Function to get all verticies in right chain of a face
	// INPUT: Vector of pointers to vertices on face, pointer to query face
	//====================================================================
	std::vector<PolyVertex*> GetRightChain(std::vector<PolyVertex*> i_oAllVertices, PolyFace *i_poPolyFace);

	//====================================================================
	// DESCRIPTION: Boolean function to determine if vertex is in a particular
	// chain. True if it is, false if it is NOT
	// INPUT: Vector of pointers to vertices on face, pointer to query face
	//====================================================================
	bool IsInChain(PolyVertex *i_poVertex, std::vector<PolyVertex*> i_oChain);

	//====================================================================
	// DESCRIPTION: Boolean function to determine if two vertices are NOT in
	// the same chain. True if they ARE NOT, false if THEY ARE.
	// INPUT: Pointers to both vertices, one of the chains
	//====================================================================
	bool NotInSameChain(PolyVertex *i_poVertex1, PolyVertex *i_poVertex2, std::vector<PolyVertex*> i_oChain);

	//====================================================================
	// DESCRIPTION: Private function that will triangulate a monotone face 
	// of a polygon.
	// INPUTS: pointer to face, pointer to DCEL.
	//====================================================================
	void TriangulateMonotonePolygon(PolyFace *&i_poFace, DblyConnectedEdgeList *&io_poMonotoneDCEL);

	DblyConnectedEdgeList *m_poMonotoneDCEL; // the doubly connected edge list which holds the triangulation.
};