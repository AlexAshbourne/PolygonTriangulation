//////////////////////////////////////////////////////////////////////
//
// FTIMonotonePolygon.h: This class is used to take any simple n-sided
// polygon and decompose it into a set of y-monotone polygons.
// The original polygon is inputted as a doubly connected edge list.
//
// Using a sweep line algorith we scan over the verticies of the 
// polygon and add edged as needed. The events of the sweep line 
// are the verticies and are handled in a binary search tree.
//
// Elements of the tree are edges
//
// Company: Forming Technologies Inc.
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
#include "FTIDblyConnectedEdgeList.h"
#include "FTIEdgeSearchTree.h"


//====================================================================
// DESCRIPTION: Class that will allow use to partition an arbitrary 
// polygon into monotone segments 
//====================================================================
class FTIMonotonePolygon
{
public:
	//====================================================================
	// DESCRIPTION: Construct class from vector of polygon vertices in CCW
	// order by calling constructor for DCEL class
	//====================================================================
	explicit FTIMonotonePolygon(std::vector<double> i_adPolygonVertices) 
	{
		m_poPolygonDCEL = new FTIDblyConnectedEdgeList(i_adPolygonVertices);
	}
	//====================================================================
	// DESCRIPTION: Constructor from pointer to DCEL
	//====================================================================
	explicit FTIMonotonePolygon(FTIDblyConnectedEdgeList *&i_poInputDCEL) 
	{
		m_poPolygonDCEL = i_poInputDCEL;
	}

	//====================================================================
	// DESCRIPTION: Copy Constructor
	//====================================================================
	FTIMonotonePolygon(const FTIMonotonePolygon &i_oRHSMontonePolygon) 
	{
		m_poPolygonDCEL = i_oRHSMontonePolygon.m_poPolygonDCEL;
	};

	//====================================================================
	// DESCRIPTION: Assignment operator
	//====================================================================
	FTIMonotonePolygon &operator= (const FTIMonotonePolygon &i_oRHSMontonePolygon) 
	{
		if(this != &i_oRHSMontonePolygon)
			m_poPolygonDCEL = i_oRHSMontonePolygon.m_poPolygonDCEL;
		return *this;
	};

	//====================================================================
	// DESCRIPTION: Destructor
	//====================================================================
	~FTIMonotonePolygon()
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
	FTIDblyConnectedEdgeList* getDCEL() {return m_poPolygonDCEL;}

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
	unsigned int VertexKind(FTIPolyVertex *i_poInputVertex, FTIPolyFace *i_poFace);

	//====================================================================
	// DESCRIPTION: Public function to insert an edge from a vertex to the "helper"
	// of an edge currently being handled by the sweep line algorithm
	//====================================================================
	void FixUp(FTIEdgeNode *i_poEdgeNode, FTIPolyVertex *i_poVertex);

	//====================================================================
	// DESCRIPTION: Functions that will handle vertices based on their kind
	// INPUT: Pointer to vertex, pointer to reference to the search tree.
	//====================================================================
	void HandleStartVertex(FTIPolyVertex *i_poInputVertex, FTIEdgeSearchTree *&i_poTree);
	void HandleEndVertex(FTIPolyVertex *i_poInputVertex, FTIEdgeSearchTree *&i_poTree);
	void HandleSplitVertex(FTIPolyVertex *i_poInputVertex, FTIEdgeSearchTree *&i_poTree);
	void HandleMergeVertex(FTIPolyVertex *i_poInputVertex, FTIEdgeSearchTree *&i_poTree);
	void HandleRegularVertexLeft(FTIPolyVertex *i_poInputVertex, FTIEdgeSearchTree *&i_poTree);
	void HandleRegularVertexRight(FTIPolyVertex *i_poInputVertex, FTIEdgeSearchTree *&i_poTree);

	//====================================================================
	// DESCRIPTION: Private funtion call to Fix Up
	//====================================================================
	void FixUp(FTIEdgeNode *i_poEdgeNode, FTIPolyVertex *i_poVertex, FTIDblyConnectedEdgeList *&i_poDCEL);

	//====================================================================
	// DESCRIPTION: Private function call to create monotone subdivision
	//====================================================================
	void MakeMonotone(FTIDblyConnectedEdgeList *&i_poDCEL);

	FTIDblyConnectedEdgeList *m_poPolygonDCEL; // as a member we use the doubly connected edge list.
};

