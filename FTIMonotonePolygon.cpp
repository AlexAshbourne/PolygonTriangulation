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

#include "MonotonePolygon.h"

//====================================================================
// DESCRIPTION: Function for determining the type of vertex in the polygon
//
// INPUTS: pointer to vertex to test and pointer to face we are viewing 
// the vertex from
//
// RETURN: unsigned int refering to vertex kind
//====================================================================
unsigned int MonotonePolygon::VertexKind(PolyVertex *i_poInputVertex, PolyFace *i_poFace)
{
	HalfEdge *StartEdge = i_poInputVertex->getEdge();
	HalfEdge *IndexEdge = i_poInputVertex->getEdge();
	do{
		if(IndexEdge->getFace() == i_poFace)
			break;
		else
			IndexEdge = IndexEdge->getTwin()->getNext();
	}while(IndexEdge != StartEdge);
	//Get neighbouring Verticies. v1 = left, v2 = right.
	//PolyVertex Vertex1 = *( i_poInputVertex->getEdge()->getTwin()->getOrigin());
	PolyVertex Vertex1 = *( IndexEdge->getTwin()->getOrigin());
	PolyVertex Vertex2 = *( IndexEdge->getPrev()->getOrigin());

	//Get coords of verticies to created edges.
	double *Vertex0Node = i_poInputVertex->getNode();
	double *Vertex1Node = Vertex1.getNode();
	double *Vertex2Node = Vertex2.getNode();

	// define edge direction vectors in 2D
	double edge1[2]; 
	double edge2[2];
	edge1[0] = Vertex1Node[0] - Vertex0Node[0];
	edge1[1] = Vertex1Node[1] - Vertex0Node[1];
	edge2[0] = Vertex2Node[0] - Vertex0Node[0];
	edge2[1] = Vertex2Node[1] - Vertex0Node[1];

	// Define dot product and determinate between two direction vectors 
	double Dot = edge1[0]*edge2[0] + edge1[1]*edge2[1];
	double Det = edge1[0]*edge2[1] - edge1[1]*edge2[0];

	double Angle = atan2(Det, Dot);

	if((Vertex1 < *i_poInputVertex)&&(Vertex2 < *i_poInputVertex))
	{
		if( Angle > 0.0)
			return START_VERTEX;
		else
			return SPLIT_VERTEX;
	}
	else if ((Vertex1 > *i_poInputVertex)&&(Vertex2 > *i_poInputVertex))
	{
		if( Angle > 0.0)
			return END_VERTEX;
		else
			return MERGE_VERTEX;
	}
	else if ((edge1[1] <= 0) && (-edge2[1] <= 0))
	{
		return REGULAR_VERTEX_LEFT;
	}
	else //((edge1[1] > 0) && (-edge2[1] > 0));
	{
		return REGULAR_VERTEX_RIGHT;
	};
};

//====================================================================
// DESCRIPTION: Public funtion to call private Fix Up function
//====================================================================
void MonotonePolygon::FixUp(EdgeNode *i_poEdgeNode, PolyVertex *i_poVertex)
{// Calls private fix up function which can alter the DCEL
	FixUp(i_poEdgeNode, i_poVertex, m_poPolygonDCEL);
};

//====================================================================
// DESCRIPTION: Private funtion to call private Fix Up function. This 
// checks to see if helper vertex of an edge is a "merge vertex" and if so,
// we insert an edge from the helper vertex to the query vertex.
//====================================================================
void MonotonePolygon::FixUp(EdgeNode *i_poEdgeNode, PolyVertex *i_poVertex, DblyConnectedEdgeList *&i_poDCEL)
{
	unsigned int TYPE = VertexKind(i_poEdgeNode->getHelper(), i_poVertex->getEdge()->getFace());

	if (TYPE == MERGE_VERTEX)
	{
		PolyVertex* HelperVertex = i_poEdgeNode->getHelper();

		PolyVertex* MaxVert = GetMax(HelperVertex, i_poVertex);
		PolyVertex* MinVert = GetMin(HelperVertex, i_poVertex);
		i_poDCEL -> InsertNewEdge(MaxVert, MinVert);
	};
};

//====================================================================
// DESCRIPTION: Functions that will handle start vertices
// INPUT: Pointer to vertex, pointer to reference to the search tree.
//====================================================================
void MonotonePolygon::HandleStartVertex(PolyVertex *i_poInputVertex, EdgeSearchTree *&i_poTree)
{
	// insert both incident edges
	EdgeNode *pNewEdgeLeft = new EdgeNode(i_poInputVertex->getEdge());

	// insert these nodes into the search tree
	i_poTree->InsertEdgeNode(pNewEdgeLeft);
};

//====================================================================
// DESCRIPTION: Functions that will handle end vertices
// INPUT: Pointer to vertex, pointer to reference to the search tree.
//====================================================================
void MonotonePolygon::HandleEndVertex(PolyVertex *i_poInputVertex, EdgeSearchTree *&i_poTree)
{
	// Find corresponding edge node in search tree to edge leading into vertex
	EdgeNode* PrevEdgeNode = i_poTree->FindEdgeNode(i_poInputVertex->getEdge()->getPrev());

	// if diagonal can be added, add it
	FixUp(PrevEdgeNode, i_poInputVertex);

	// remove edges leading into and out of vertex from search tree
	i_poTree->RemoveEdgeNode(PrevEdgeNode);
};

//====================================================================
// DESCRIPTION: Functions that will handle split vertices
// INPUT: Pointer to vertex, pointer to reference to the search tree.
//====================================================================
void MonotonePolygon::HandleSplitVertex(PolyVertex *i_poInputVertex, EdgeSearchTree *&i_poTree)
{
	// Find edge closest to the left of the vertex
	EdgeNode* ClosestEdge = i_poTree->FindClosestLeft(i_poInputVertex);
	// Get Helper vertex for this edge
	PolyVertex* HelperVertex = ClosestEdge->getHelper();

	// Insert diagonal from helper to this vertex
	PolyVertex* MaxVert = GetMax(HelperVertex, i_poInputVertex);
	PolyVertex* MinVert = GetMin(HelperVertex, i_poInputVertex);
	m_poPolygonDCEL -> InsertNewEdge(MaxVert, MinVert);

	ClosestEdge->setHelper(i_poInputVertex);

	// Find both edges incident to the vertex and add them to search tree
	EdgeNode *pNewEdgeRight = new EdgeNode(i_poInputVertex->getEdge());
	// set right edge helper to current vertex
	pNewEdgeRight -> setHelper(i_poInputVertex);

	// insert these nodes into the search tree
	i_poTree->InsertEdgeNode(pNewEdgeRight);
};

//====================================================================
// DESCRIPTION: Functions that will handle merge vertices
// INPUT: Pointer to vertex, pointer to reference to the search tree.
//====================================================================
void MonotonePolygon::HandleMergeVertex(PolyVertex *i_poInputVertex, EdgeSearchTree *&i_poTree)
{
	// get edge nodes incident to vertex. They will both be in the search tree
	//EdgeNode* pLeftEdgeNode = i_poTree->FindEdgeNode(i_poInputVertex->getEdge());
	EdgeNode* pRightEdgeNode = i_poTree->FindEdgeNode(i_poInputVertex->getEdge()->getPrev());

	// if diagonal can be added to the right edge node, add it
	FixUp(pRightEdgeNode, i_poInputVertex);

	// remove them both from the search tree
	i_poTree->RemoveEdgeNode(pRightEdgeNode);

	// Find edge closest to the left of the vertex
	EdgeNode* ClosestEdge = i_poTree->FindClosestLeft(i_poInputVertex);
	// Insert Edge if Possible
	if(ClosestEdge)
		FixUp(ClosestEdge, i_poInputVertex);

	// Set helper of the closest edge to V
	ClosestEdge->setHelper(i_poInputVertex);
};

//====================================================================
// DESCRIPTION: Functions that will handle regular vertices on left chain
// INPUT: Pointer to vertex, pointer to reference to the search tree.
//====================================================================
void MonotonePolygon::HandleRegularVertexLeft(PolyVertex *i_poInputVertex, EdgeSearchTree *&i_poTree)
{
	// Find edge leading into current vertex
	// HalfEdge* PreviousEdge = i_poInputVertex->getEdge()->getPrev();
	// Find corresponding edge node in search tree.
	EdgeNode* PrevEdgeNode = i_poTree->FindEdgeNode(i_poInputVertex->getEdge()->getPrev());

	// if diagonal can be added, add it
	FixUp(PrevEdgeNode, i_poInputVertex);

	// adjust search tree
	i_poTree->RemoveEdgeNode(PrevEdgeNode);
	i_poTree->InsertHalfEdge(i_poInputVertex->getEdge());
};

//====================================================================
// DESCRIPTION: Functions that will handle regular vertices on right chain
// INPUT: Pointer to vertex, pointer to reference to the search tree.
//====================================================================
void MonotonePolygon::HandleRegularVertexRight(PolyVertex *i_poInputVertex, EdgeSearchTree *&i_poTree)
{
	// Find edge closest to the left of the vertex
	EdgeNode* ClosestEdge = i_poTree->FindClosestLeft(i_poInputVertex);

	// Insert Edge if Possible
	if(ClosestEdge)
		FixUp(ClosestEdge, i_poInputVertex);

	ClosestEdge->setHelper(i_poInputVertex);
};

//====================================================================
// DESCRIPTION: Public Function to make monotono
//====================================================================
void MonotonePolygon::MakeMonotone()
{
	MakeMonotone(m_poPolygonDCEL);
};

//====================================================================
// DESCRIPTION: Private function to makemonotone algorithm.
//====================================================================
void MonotonePolygon::MakeMonotone(DblyConnectedEdgeList *&i_poDCEL)
{
	// Extract vertices from DCEL
	std::vector<PolyVertex*> SortedVertices = i_poDCEL->getVertices();	

	// Store them in a sorted (top to bottom) list since we do not need to add any more vertices
	std::sort(SortedVertices.begin(), SortedVertices.end(), PolyVertex::PolyVertexDescendingOrder());

	// Get number of vertices.
	int nNumVertex = SortedVertices.size();

	//std::cout << "Creation of search tree \n" << std::endl;
	// initialize an empty search tree
	EdgeSearchTree *pEdgeSearchTree = new EdgeSearchTree();

	for(int i = 0; i < nNumVertex; i++)
	{
		// Determine what flavour of vertex we are dealing with
		unsigned int TYPE = VertexKind(SortedVertices[i], SortedVertices[i]->getEdge()->getFace());

		// handle vertex accordingly
		switch(TYPE) 
		{
		case START_VERTEX : HandleStartVertex(SortedVertices[i], pEdgeSearchTree); break;
		case SPLIT_VERTEX : HandleSplitVertex(SortedVertices[i],pEdgeSearchTree); break;
		case END_VERTEX : HandleEndVertex(SortedVertices[i], pEdgeSearchTree); break;
		case MERGE_VERTEX : HandleMergeVertex(SortedVertices[i], pEdgeSearchTree); break;
		case REGULAR_VERTEX_LEFT : HandleRegularVertexLeft(SortedVertices[i], pEdgeSearchTree); break;
		case REGULAR_VERTEX_RIGHT : HandleRegularVertexRight(SortedVertices[i], pEdgeSearchTree); break;
		};

	}; // end of loop

	// delete memory allocated for the binary search tree as it is no longer needed
	delete pEdgeSearchTree;
}; // end of makemonotone
