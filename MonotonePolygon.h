//////////////////////////////////////////////////////////////////////
//
// MonotonePolygon.h: This class is used to take any simple n-sided
// polygon and decompose it into a set of y-monotone polygons.
// The original polygon is inputted as a doubly connected edge list.
//
// Using a sweep line algorith we scan over the verticies of the 
// polygon and add edged as needed. The events of the sweep line 
// are the verticies and are handled in a binary search tree.
//
// Elements of the tree are edges
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

enum 
{
	START_VERTEX = 1U, SPLIT_VERTEX, END_VERTEX, MERGE_VERTEX, REGULAR_VERTEX_LEFT, REGULAR_VERTEX_RIGHT
};

#include <algorithm>
#include <iostream>
#include <math.h>
#include "DblyConnectedEdgeList.h"
#include "EdgeSearchTree.h"


//====================================================================
// DESCRIPTION: Class that will allow use to partition an arbitrary 
// polygon into monotone segments 
//====================================================================
class MonotonePolygon
{
public:
	//====================================================================
	// DESCRIPTION: Construct class from vector of polygon vertices in CCW
	// order by calling constructor for DCEL class
	//====================================================================
	explicit MonotonePolygon(std::vector<double> i_adPolygonVertices) 
	{
		m_poPolygonDCEL = new DblyConnectedEdgeList(i_adPolygonVertices);
	}
	//====================================================================
	// DESCRIPTION: Constructor from pointer to DCEL
	//====================================================================
	explicit MonotonePolygon(DblyConnectedEdgeList *&i_poInputDCEL) 
	{
		m_poPolygonDCEL = i_poInputDCEL;
	}

	//====================================================================
	// DESCRIPTION: Copy Constructor
	//====================================================================
	MonotonePolygon(const MonotonePolygon &i_oRHSMontonePolygon) 
	{
		m_poPolygonDCEL = i_oRHSMontonePolygon.m_poPolygonDCEL;
	};

	//====================================================================
	// DESCRIPTION: Assignment operator
	//====================================================================
	MonotonePolygon &operator= (const MonotonePolygon &i_oRHSMontonePolygon) 
	{
		if(this != &i_oRHSMontonePolygon)
			m_poPolygonDCEL = i_oRHSMontonePolygon.m_poPolygonDCEL;
		return *this;
	};

	//====================================================================
	// DESCRIPTION: Destructor
	//====================================================================
	~MonotonePolygon()
	{
		//std::cout << "Monotone delete \n" << std::endl;
		// check to see if DCEL is empty
		//if( !((m_poPolygonDCEL->getNumVertex() == 0) && (m_poPolygonDCEL->getNumEdges() == 0) && (m_poPolygonDCEL->getNumFaces() == 0)))
		if( !m_poPolygonDCEL)
		{
			//std::cout << "Deleting DCEL \n" << std::endl;
			delete m_poPolygonDCEL;
			m_poPolygonDCEL = nullptr;
		};
	};

	//====================================================================
	// DESCRIPTION: Return pointer to DCEL
	//====================================================================
	DblyConnectedEdgeList* getDCEL() {return m_poPolygonDCEL;}

	//====================================================================
	// DESCRIPTION: Public function for polygon subdivision into monotone
	// pieces.
	//====================================================================
	void MakeMonotone();

private:

	//====================================================================
	// DESCRIPTION: Function for determining the type of vertex in the polygon
	// INPUTS: pointer to vertex to test and pointer to face we are viewing 
	// the vertex from
	//====================================================================
	unsigned int VertexKind(PolyVertex *i_poInputVertex, PolyFace *i_poFace);

	//====================================================================
	// DESCRIPTION: Public function to insert an edge from a vertex to the "helper"
	// of an edge currently being handled by the sweep line algorithm
	//====================================================================
	void FixUp(EdgeNode *i_poEdgeNode, PolyVertex *i_poVertex);

	//====================================================================
	// DESCRIPTION: Functions that will handle vertices based on their kind
	// INPUT: Pointer to vertex, pointer to reference to the search tree.
	//====================================================================
	void HandleStartVertex(PolyVertex *i_poInputVertex, EdgeSearchTree *&i_poTree);
	void HandleEndVertex(PolyVertex *i_poInputVertex, EdgeSearchTree *&i_poTree);
	void HandleSplitVertex(PolyVertex *i_poInputVertex, EdgeSearchTree *&i_poTree);
	void HandleMergeVertex(PolyVertex *i_poInputVertex, EdgeSearchTree *&i_poTree);
	void HandleRegularVertexLeft(PolyVertex *i_poInputVertex, EdgeSearchTree *&i_poTree);
	void HandleRegularVertexRight(PolyVertex *i_poInputVertex, EdgeSearchTree *&i_poTree);

	//====================================================================
	// DESCRIPTION: Private funtion call to Fix Up
	//====================================================================
	void FixUp(EdgeNode *i_poEdgeNode, PolyVertex *i_poVertex, DblyConnectedEdgeList *&i_poDCEL);

	//====================================================================
	// DESCRIPTION: Private function call to create monotone subdivision
	//====================================================================
	void MakeMonotone(DblyConnectedEdgeList *&i_poDCEL);

	DblyConnectedEdgeList *m_poPolygonDCEL; // as a member we use the doubly connected edge list.
};

