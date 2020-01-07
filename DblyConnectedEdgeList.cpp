//////////////////////////////////////////////////////////////////////
//
// DblyConnectedEdgeList.h: Class defined to store information for 
// local polygon triangluation.
//
// REVISIONS:
//  Feb. 10, 2017 created (Alex Ashbourne)
//
//////////////////////////////////////////////////////////////////////

#include "DblyConnectedEdgeList.h"
//-------------------------------------------------------------------------


//==================================================================
//
// DESCRIPTION:
//	Finds the max (highest) of two vertices
//
// INPUT:
//  Pointers to input vertices
//
// RETURN: 
//	Pointer to the maximum vertex
//
// ACCESS: C++, private.
//
// Revisions:
//	Feb. 13, 2017
//
//==================================================================
PolyVertex* GetMax(PolyVertex* i_poPolyVertex1, PolyVertex* i_poPolyVertex2)
{
	if(*i_poPolyVertex1 > *i_poPolyVertex2)
		return i_poPolyVertex1;
	else if (*i_poPolyVertex1 < *i_poPolyVertex2)
		return i_poPolyVertex2;
	else
		return i_poPolyVertex1;
};

//==================================================================
//
// DESCRIPTION:
//	Finds the min (lowest) of two vertices
//
// INPUT:
//  Pointers to input vertices
//
// RETURN: 
//	Pointer to the minimum vertex
//
// ACCESS: C++, private.
//
// Revisions:
//	Feb. 13, 2017
//
//==================================================================
PolyVertex* GetMin(PolyVertex* i_poPolyVertex1, PolyVertex* i_poPolyVertex2)
{
	if(*i_poPolyVertex1 < *i_poPolyVertex2)
		return i_poPolyVertex1;
	else if (*i_poPolyVertex1 > *i_poPolyVertex2)
		return i_poPolyVertex2;
	else
		return i_poPolyVertex1;
};

//==================================================================
//
// DESCRIPTION:
//	We compute the orientation of three points to determine whether these
//  we can add an edge to the polygon without passing through the boundary.
//
// INPUT: 
//  pointers to three vertices
//  Vertex1 - Previous vertex
//  Vertex2 - Current vertex
//  Vertex3 - Next vertex
// 
// RETURN: 
//	double - the orientation: 
//  > 0: Points define a counter clockwise rotation around the triangle v1-v2-v3
//		we can add edge if in left chain.
//  < 0: Points define a clockwise rotation around the triangle v1-v2-v3
//		we can add edge if in right chain.
//  = 0: points are collinear
//
// ACCESS: C++, private.
//
// Revisions:
//	Feb. 13, 2017
//
//==================================================================
double Orientation(PolyVertex* i_poPolyVertex1, PolyVertex* i_poPolyVertex2, PolyVertex* i_poPolyVertex3)
{
	double* Pnode = i_poPolyVertex1 ->getNode();
	double* Qnode = i_poPolyVertex2 ->getNode();
	double* Rnode = i_poPolyVertex3 ->getNode();

	return (Qnode[0]*Rnode[1] - Rnode[0]*Qnode[1]) - Pnode[0]*(Rnode[1]-Qnode[1]) + Pnode[1]*(Rnode[0]-Qnode[0]);

};

bool IsVertexLeftofEdge(PolyVertex *i_poVertex, HalfEdge *i_poHalfEdge)
{
	double* EdgeStart = i_poVertex->getNode();
	//double* EdgeEnd = m_pTwin->m_pOrigin->getNode();
	double* OtherEdgeStart = i_poHalfEdge->getOrigin()->getNode();
	double* OtherEdgeEnd = i_poHalfEdge->getTwin()->getOrigin()->getNode();

	if(OtherEdgeEnd[0] == OtherEdgeStart[0])
	{
		if(EdgeStart[0] < OtherEdgeStart[0])
			return true;
		else
			return false;
	}
	else
	{
		double Slope = (OtherEdgeEnd[1] - OtherEdgeStart[1]) / (OtherEdgeEnd[0] - OtherEdgeStart[0]);
		//double Intercept = OtherEdgeStart[1] - OtherEdgeStart[0]*Slope;
		if(Slope != 0)
		{
			double Xcoord = (EdgeStart[1] - OtherEdgeStart[1]) / Slope + OtherEdgeStart[0];
			if(EdgeStart[0] < Xcoord)
				return true;
			else
				return false;
		}
		else
		{
			if(EdgeStart[0] < OtherEdgeEnd[0])
				return true;
			else
				return false;
		};
	};
};
bool IsVertexRightofEdge(PolyVertex *i_poVertex, HalfEdge *i_poHalfEdge)
{
	double* EdgeStart = i_poVertex->getNode();
	//double* EdgeEnd = m_pTwin->m_pOrigin->getNode();
	double* OtherEdgeStart = i_poHalfEdge->getOrigin()->getNode();
	double* OtherEdgeEnd = i_poHalfEdge->getTwin()->getOrigin()->getNode();

	if(OtherEdgeEnd[0] == OtherEdgeStart[0])
	{
		if(EdgeStart[0] > OtherEdgeStart[0])
			return true;
		else
			return false;
	}
	else
	{
		double Slope = (OtherEdgeEnd[1] - OtherEdgeStart[1]) / (OtherEdgeEnd[0] - OtherEdgeStart[0]);
		//double Intercept = OtherEdgeStart[1] - OtherEdgeStart[0]*Slope;
		if(Slope != 0)
		{
			double Xcoord = (EdgeStart[1] - OtherEdgeStart[1]) / Slope + OtherEdgeStart[0];
			if(EdgeStart[0] > Xcoord)
				return true;
			else
				return false;
		}
		else
		{
			if(EdgeStart[0] > OtherEdgeEnd[0])
				return true;
			else
				return false;
		};
	};
};


//==================================================================
//
// DESCRIPTION:
//	Private initializer for doubly connected edge list. Initialized PolyVertex, HalfEdge 
//	and PolyFace arrays to the amount needed:
//
//	For n verticies there are 4*n - 6 half edges and n-1 PolyFaces after the polynomial 
//	triangulation. Note Face[0] is defined to be the outside of the polygon
//
//	After initialization there are two PolyFace.  Note Face[0] is defined to be the outside 
//  of the polygon and Face[1] is the inside.
//
// RETURN: 
//	void
//
// ACCESS: C++, private.
//
// Revisions:
//	Feb. 13, 2017
//
//==================================================================
void DblyConnectedEdgeList::InitializeDCEL(std::vector<double> i_adPolygonVertices, int i_nSize)
{

	//std::cout << "inside private initializer \n" << std::endl;
	m_pPolyVertexNodes.reserve(i_nSize);
	m_pHalfEdgeVect.reserve(4*i_nSize - 6);
	m_pPolyFaceVect.reserve(i_nSize-2+1);

	PolyFace *pOutsideFace = new PolyFace; //in first position insert outside PolyFace, i.e. PolyFace outside polygon
	m_pPolyFaceVect.push_back(pOutsideFace);

	// initialize first PolyVertex and first edge
	PolyVertex *pNew_Vert = new PolyVertex(i_adPolygonVertices[0], i_adPolygonVertices[1]);

	m_pPolyVertexNodes.push_back(pNew_Vert);

	HalfEdge *pNew_half_edge = new HalfEdge(pNew_Vert);
	m_pHalfEdgeVect.push_back(pNew_half_edge);

	pNew_Vert->setEdge(pNew_half_edge);

	// Set interior PolyFace
	PolyFace *pNew_PolyFace = new PolyFace(m_pHalfEdgeVect[0]);
	m_pPolyFaceVect.push_back(pNew_PolyFace);

	m_pHalfEdgeVect[0]->setPolyFace(m_pPolyFaceVect[1]);

	// initializing vector of verticies and edges along with "previous edge"
	for(int i = 1; i < i_nSize; i++)
	{
		// initialize PolyVertex
		//PolyVertex new_vert(i_adPolygonVertices[2*i], i_adPolygonVertices[2*i + 1]);
		PolyVertex *pNew_Vert = new PolyVertex(i_adPolygonVertices[2*i], i_adPolygonVertices[2*i + 1]);
		
		//m_pPolyVertexNodes.push_back(&new_vert);
		m_pPolyVertexNodes.push_back(pNew_Vert);

		//// initialize interior half edge
		HalfEdge *pNew_half_edge = new HalfEdge(pNew_Vert);
		pNew_Vert->setEdge(pNew_half_edge);
		// add to vector
		m_pHalfEdgeVect.push_back(pNew_half_edge);

		//// set interior PolyFace
		m_pHalfEdgeVect[i]->setPolyFace(m_pPolyFaceVect[1]);

		//// set previous edge		
		m_pHalfEdgeVect[i]->setPrev(m_pHalfEdgeVect[i-1]);

		//delete pNew_Vert;
		//delete pNew_half_edge;

	};

	m_pHalfEdgeVect[0]->setPrev(m_pHalfEdgeVect[i_nSize-1]);

	// set "next edge" 
	for(int i = 0; i < i_nSize-1; i++)
	{
		m_pHalfEdgeVect[i]->setNext(m_pHalfEdgeVect[i+1]);
	};

	m_pHalfEdgeVect[i_nSize-1]->setNext(m_pHalfEdgeVect[0]);

	// Create "twin" half edges
	HalfEdge *pNew_Twin_Edge = new HalfEdge(m_pPolyVertexNodes[0]);
	pNew_Twin_Edge->setTwin(m_pHalfEdgeVect[i_nSize - 1]);
	pNew_Twin_Edge->setPolyFace(m_pPolyFaceVect[0]);

	m_pHalfEdgeVect.push_back(pNew_Twin_Edge);

	for(int i = 1; i < i_nSize; i++)
	{
		// create twin half edge
		HalfEdge *pNew_Twin_Edge = new HalfEdge(m_pPolyVertexNodes[i]);
		// set twin
		pNew_Twin_Edge->setTwin(m_pHalfEdgeVect[i - 1]);
		// set PolyFace (null PolyFace, outside)
		pNew_Twin_Edge->setPolyFace(m_pPolyFaceVect[0]);

		// place half edge in vector
		m_pHalfEdgeVect.push_back(pNew_Twin_Edge);

		// set next half edge
		pNew_Twin_Edge->setNext(m_pHalfEdgeVect[i_nSize + i - 1]);
		
	};

	m_pHalfEdgeVect[i_nSize]->setNext(m_pHalfEdgeVect[2*i_nSize - 1]);
	
	// set previous edges for the twin half edges
	m_pHalfEdgeVect[2*i_nSize - 1]->setPrev(m_pHalfEdgeVect[i_nSize]);

	for(int i = i_nSize; i < 2*i_nSize - 1; i++)
	{
		m_pHalfEdgeVect[i]->setPrev(m_pHalfEdgeVect[i + 1]);
	};

	// set pointer of outside PolyFace to edge
	m_pPolyFaceVect[0]->setEdge(m_pHalfEdgeVect[i_nSize]);

	//std::cout << "end of initializer \n" << std::endl;
};

//==================================================================
//
// DESCRIPTION:
//	Inserts new edge into DCEL where v1 is the origin of the new edge and v2 is the destination.
//	e = v2 - v1.
//
// RETURN: 
//	void
//
// ACCESS: C++, public.
//
// Revisions:
//	Feb. 13, 2017
//
//==================================================================

void DblyConnectedEdgeList::InsertNewEdge(PolyVertex* &i_poPolyVertex1, PolyVertex* &i_poPolyVertex2)
{
	HalfEdge *pNew_Edge = new HalfEdge;
	HalfEdge *pNew_Edge_Twin = new HalfEdge;
	PolyFace *pNew_PolyFace = new PolyFace;

	pNew_Edge->setOrigin(i_poPolyVertex1);
	pNew_Edge_Twin->setOrigin(i_poPolyVertex2);
	pNew_Edge->setTwin(pNew_Edge_Twin);
	pNew_PolyFace->setEdge(pNew_Edge_Twin);
	pNew_Edge_Twin->setPolyFace(pNew_PolyFace);

	// search for common Face:

	PolyFace *CommonFace;
	PolyFace *OutsideFace = m_pPolyFaceVect[0];
	HalfEdge *V1StartEdge = i_poPolyVertex1->getEdge(); // face of reference edge for vertex 1
	HalfEdge *V2StartEdge = i_poPolyVertex2->getEdge(); // face of reference edge for vertex 2

	if((*(V1StartEdge->getFace()) == *(V2StartEdge->getFace())) && (*(V1StartEdge->getFace()) != *OutsideFace))
		CommonFace = V1StartEdge->getFace();
	else
	{
		HalfEdge *V1IndexEdge = V1StartEdge; // face of reference edge for vertex 1

		do
		{
			HalfEdge *V2IndexEdge = V2StartEdge;

			if((*(V1IndexEdge->getFace()) == *(V2IndexEdge->getFace())) && (*(V1IndexEdge->getFace()) != *OutsideFace))
			{
				CommonFace = V1IndexEdge->getFace();
				break;
			}
			else
			{
				V2IndexEdge = V2IndexEdge->getTwin()->getNext();

				while(*(V2IndexEdge) != *(V2StartEdge))
				{
					if((*(V1IndexEdge->getFace()) == *(V2IndexEdge->getFace())) && (*(V1IndexEdge->getFace()) != *OutsideFace))
					{
						CommonFace = V1IndexEdge->getFace();
						break;
					}
					else
					{
						V2IndexEdge = V2IndexEdge->getTwin()->getNext();
					};
				};
			};

			V1IndexEdge = V1IndexEdge->getTwin()->getNext();
		}while(*(V1IndexEdge) != *(V1StartEdge));
		
	};

	pNew_Edge->setPolyFace(CommonFace);
	CommonFace -> setEdge(pNew_Edge);

	// Have Common PolyFace, now we can add edge and add connectivity

	// Query PolyVertex 1
	HalfEdge *Start_Search_Edge1 = i_poPolyVertex1->getEdge();
	HalfEdge *Index_Edge1;

	if(((*(Start_Search_Edge1->getFace()) == *CommonFace) && (*(Start_Search_Edge1->getOrigin()) == *i_poPolyVertex1)))
	{
		pNew_Edge_Twin->setNext(Start_Search_Edge1);
		pNew_Edge->setPrev(Start_Search_Edge1->getPrev());

		Start_Search_Edge1->getPrev()->setNext(pNew_Edge);
		Start_Search_Edge1->setPrev(pNew_Edge_Twin);	
	}
	else
	{
		Index_Edge1 = (Start_Search_Edge1->getTwin()->getNext());
	
		while( *Index_Edge1 != *Start_Search_Edge1 )
		{
	
			if ((*(Index_Edge1->getFace()) == *CommonFace) && (*(Index_Edge1->getOrigin()) == *i_poPolyVertex1))
			{
				pNew_Edge_Twin->setNext(Index_Edge1);
				pNew_Edge->setPrev(Index_Edge1->getPrev());

				Index_Edge1->getPrev()->setNext(pNew_Edge);
				Index_Edge1->setPrev(pNew_Edge_Twin);				
			};

			Index_Edge1 = (Index_Edge1->getTwin()->getNext());
		};
	};

	// Query PolyVertex 2
	HalfEdge *Start_Search_Edge2 = (i_poPolyVertex2->getEdge());
	HalfEdge *Index_Edge2;

	if(((*(Start_Search_Edge2->getFace()) == *CommonFace) && (*(Start_Search_Edge2->getOrigin()) == *i_poPolyVertex2)))
	{
		pNew_Edge->setNext(Start_Search_Edge2);
		pNew_Edge_Twin->setPrev(Start_Search_Edge2->getPrev());

		Start_Search_Edge2->getPrev()->setNext(pNew_Edge_Twin);
		Start_Search_Edge2->setPrev(pNew_Edge);	
	}
	else
	{
		Index_Edge2 = (Start_Search_Edge2->getTwin()->getNext());
	
		while( Index_Edge2 != Start_Search_Edge2 )
		{
			if ((*(Index_Edge2->getFace()) == *CommonFace) && (*(Index_Edge2->getOrigin()) == *i_poPolyVertex2))
			{
				pNew_Edge->setNext(Index_Edge2);
				pNew_Edge_Twin->setPrev(Index_Edge2->getPrev());

				Index_Edge2->getPrev()->setNext(pNew_Edge_Twin);
				Index_Edge2->setPrev(pNew_Edge);
			};

			Index_Edge2 = (Index_Edge2->getTwin()->getNext());
		};
	};

	// Need to change all half edges on pNew_Edge_Twin path to have the new PolyFace.
	HalfEdge *Start_Edge3 = pNew_Edge_Twin;
	HalfEdge *Index_Edge3 = Start_Edge3->getNext();

	while( *Index_Edge3 != *Start_Edge3 )
	{
		Index_Edge3->setPolyFace(pNew_PolyFace);
		Index_Edge3 = (Index_Edge3->getNext());
	};

	// Push new data to DCEL
	m_pHalfEdgeVect.push_back(pNew_Edge);
	m_pHalfEdgeVect.push_back(pNew_Edge_Twin);
	m_pPolyFaceVect.push_back(pNew_PolyFace);
};

//====================================================================
// DESCRIPTION: Write DCEL to output file
//====================================================================
void DblyConnectedEdgeList::WriteToFile(std::ofstream &outputFile)
{
	outputFile << "FEMB DATA (LARGE FORMAT)" << std::endl;
	outputFile << "PART GEOMET" << std::endl;

	unsigned int numEdges = m_pHalfEdgeVect.size();
	unsigned int numVert = m_pPolyVertexNodes.size();

	for(unsigned int i = 0; i < numVert; i++)
	{
		double* origin = m_pPolyVertexNodes[i]->getNode();

		outputFile << "LINE       "<< i << "     0.00000     0.00000     0.00000" << std::endl;
		outputFile << std::setw(10) << std::left << origin[0] << std::setw(10) << std::left << origin[1] << std::setw(10) << std::left << 0.0 << std::endl;
	};

	for(unsigned int i = 0; i < numVert; i++)
	{
		double* origin = m_pHalfEdgeVect[i]->getOrigin()->getNode();
		double* end =  m_pHalfEdgeVect[i]->getTwin()->getOrigin()->getNode();

		outputFile << "LINE       "<< i + numVert << "     0.00000     0.00000     0.00000" << std::endl;
		outputFile << std::setw(10) << std::left << origin[0] << std::setw(10) << std::left << origin[1] << std::setw(10) << std::left << 0.0 << std::endl;
		outputFile << std::setw(10) << std::left << end[0] << std::setw(10) << std::left << end[1] << std::setw(10) << std::left << 0.0 << std::endl;
	};

	for(unsigned int i = 2*numVert; i < numEdges; i++)
	{
		double* origin = m_pHalfEdgeVect[i]->getOrigin()->getNode();
		double* end =  m_pHalfEdgeVect[i]->getTwin()->getOrigin()->getNode();

		outputFile << "LINE       "<< i + numVert << "     1.00000     0.00000     0.00000" << std::endl;
		outputFile << std::setw(10) << std::left << origin[0] << std::setw(10) << std::left << origin[1] << std::setw(10) << std::left << 0.0 << std::endl;
		outputFile << std::setw(10) << std::left << end[0] << std::setw(10) << std::left << end[1] << std::setw(10) << std::left << 0.0 << std::endl;
	};
};