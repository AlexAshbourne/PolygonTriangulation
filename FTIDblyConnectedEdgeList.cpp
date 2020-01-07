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


//#include "tchar.h"
////- FTIStitchDLL module headers -------------------------------------------
//#include "FTIDblVec3D.h"
//#include "FTIVector.h"
//#include "FTIObjectList.h"
//#include "FTIScrapEntity.h" 

#include "FTIDblyConnectedEdgeList.h"
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
FTIPolyVertex* GetMax(FTIPolyVertex* i_poFTIPolyVertex1, FTIPolyVertex* i_poFTIPolyVertex2)
{
	if(*i_poFTIPolyVertex1 > *i_poFTIPolyVertex2)
		return i_poFTIPolyVertex1;
	else if (*i_poFTIPolyVertex1 < *i_poFTIPolyVertex2)
		return i_poFTIPolyVertex2;
	else
		return i_poFTIPolyVertex1;
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
FTIPolyVertex* GetMin(FTIPolyVertex* i_poFTIPolyVertex1, FTIPolyVertex* i_poFTIPolyVertex2)
{
	if(*i_poFTIPolyVertex1 < *i_poFTIPolyVertex2)
		return i_poFTIPolyVertex1;
	else if (*i_poFTIPolyVertex1 > *i_poFTIPolyVertex2)
		return i_poFTIPolyVertex2;
	else
		return i_poFTIPolyVertex1;
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
double Orientation(FTIPolyVertex* i_poFTIPolyVertex1, FTIPolyVertex* i_poFTIPolyVertex2, FTIPolyVertex* i_poFTIPolyVertex3)
{
	double* Pnode = i_poFTIPolyVertex1 ->getNode();
	double* Qnode = i_poFTIPolyVertex2 ->getNode();
	double* Rnode = i_poFTIPolyVertex3 ->getNode();

	return (Qnode[0]*Rnode[1] - Rnode[0]*Qnode[1]) - Pnode[0]*(Rnode[1]-Qnode[1]) + Pnode[1]*(Rnode[0]-Qnode[0]);

};

bool IsVertexLeftofEdge(FTIPolyVertex *i_poVertex, FTIHalfEdge *i_poHalfEdge)
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
bool IsVertexRightofEdge(FTIPolyVertex *i_poVertex, FTIHalfEdge *i_poHalfEdge)
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
//	Private initializer for doubly connected edge list. Initialized FTIPolyVertex, FTIHalfEdge 
//	and FTIPolyFace arrays to the amount needed:
//
//	For n verticies there are 4*n - 6 half edges and n-1 FTIPolyFaces after the polynomial 
//	triangulation. Note Face[0] is defined to be the outside of the polygon
//
//	After initialization there are two FTIPolyFace.  Note Face[0] is defined to be the outside 
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
void FTIDblyConnectedEdgeList::InitializeDCEL(std::vector<double> i_adPolygonVertices, int i_nSize)
{

	//std::cout << "inside private initializer \n" << std::endl;
	m_pFTIPolyVertexNodes.reserve(i_nSize);
	m_pFTIHalfEdgeVect.reserve(4*i_nSize - 6);
	m_pFTIPolyFaceVect.reserve(i_nSize-2+1);

	FTIPolyFace *pOutsideFace = new FTIPolyFace; //in first position insert outside FTIPolyFace, i.e. FTIPolyFace outside polygon
	m_pFTIPolyFaceVect.push_back(pOutsideFace);

	// initialize first FTIPolyVertex and first edge
	FTIPolyVertex *pNew_Vert = new FTIPolyVertex(i_adPolygonVertices[0], i_adPolygonVertices[1]);

	m_pFTIPolyVertexNodes.push_back(pNew_Vert);

	FTIHalfEdge *pNew_half_edge = new FTIHalfEdge(pNew_Vert);
	m_pFTIHalfEdgeVect.push_back(pNew_half_edge);

	pNew_Vert->setEdge(pNew_half_edge);

	// Set interior FTIPolyFace
	FTIPolyFace *pNew_FTIPolyFace = new FTIPolyFace(m_pFTIHalfEdgeVect[0]);
	m_pFTIPolyFaceVect.push_back(pNew_FTIPolyFace);

	m_pFTIHalfEdgeVect[0]->setFTIPolyFace(m_pFTIPolyFaceVect[1]);

	// initializing vector of verticies and edges along with "previous edge"
	for(int i = 1; i < i_nSize; i++)
	{
		// initialize FTIPolyVertex
		//FTIPolyVertex new_vert(i_adPolygonVertices[2*i], i_adPolygonVertices[2*i + 1]);
		FTIPolyVertex *pNew_Vert = new FTIPolyVertex(i_adPolygonVertices[2*i], i_adPolygonVertices[2*i + 1]);
		
		//m_pFTIPolyVertexNodes.push_back(&new_vert);
		m_pFTIPolyVertexNodes.push_back(pNew_Vert);

		//// initialize interior half edge
		FTIHalfEdge *pNew_half_edge = new FTIHalfEdge(pNew_Vert);
		pNew_Vert->setEdge(pNew_half_edge);
		// add to vector
		m_pFTIHalfEdgeVect.push_back(pNew_half_edge);

		//// set interior FTIPolyFace
		m_pFTIHalfEdgeVect[i]->setFTIPolyFace(m_pFTIPolyFaceVect[1]);

		//// set previous edge		
		m_pFTIHalfEdgeVect[i]->setPrev(m_pFTIHalfEdgeVect[i-1]);

		//delete pNew_Vert;
		//delete pNew_half_edge;

	};

	m_pFTIHalfEdgeVect[0]->setPrev(m_pFTIHalfEdgeVect[i_nSize-1]);

	// set "next edge" 
	for(int i = 0; i < i_nSize-1; i++)
	{
		m_pFTIHalfEdgeVect[i]->setNext(m_pFTIHalfEdgeVect[i+1]);
	};

	m_pFTIHalfEdgeVect[i_nSize-1]->setNext(m_pFTIHalfEdgeVect[0]);

	// Create "twin" half edges
	FTIHalfEdge *pNew_Twin_Edge = new FTIHalfEdge(m_pFTIPolyVertexNodes[0]);
	pNew_Twin_Edge->setTwin(m_pFTIHalfEdgeVect[i_nSize - 1]);
	pNew_Twin_Edge->setFTIPolyFace(m_pFTIPolyFaceVect[0]);

	m_pFTIHalfEdgeVect.push_back(pNew_Twin_Edge);

	for(int i = 1; i < i_nSize; i++)
	{
		// create twin half edge
		FTIHalfEdge *pNew_Twin_Edge = new FTIHalfEdge(m_pFTIPolyVertexNodes[i]);
		// set twin
		pNew_Twin_Edge->setTwin(m_pFTIHalfEdgeVect[i - 1]);
		// set FTIPolyFace (null FTIPolyFace, outside)
		pNew_Twin_Edge->setFTIPolyFace(m_pFTIPolyFaceVect[0]);

		// place half edge in vector
		m_pFTIHalfEdgeVect.push_back(pNew_Twin_Edge);

		// set next half edge
		pNew_Twin_Edge->setNext(m_pFTIHalfEdgeVect[i_nSize + i - 1]);
		
	};

	m_pFTIHalfEdgeVect[i_nSize]->setNext(m_pFTIHalfEdgeVect[2*i_nSize - 1]);
	
	// set previous edges for the twin half edges
	m_pFTIHalfEdgeVect[2*i_nSize - 1]->setPrev(m_pFTIHalfEdgeVect[i_nSize]);

	for(int i = i_nSize; i < 2*i_nSize - 1; i++)
	{
		m_pFTIHalfEdgeVect[i]->setPrev(m_pFTIHalfEdgeVect[i + 1]);
	};

	// set pointer of outside FTIPolyFace to edge
	m_pFTIPolyFaceVect[0]->setEdge(m_pFTIHalfEdgeVect[i_nSize]);

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

void FTIDblyConnectedEdgeList::InsertNewEdge(FTIPolyVertex* &i_poFTIPolyVertex1, FTIPolyVertex* &i_poFTIPolyVertex2)
{
	FTIHalfEdge *pNew_Edge = new FTIHalfEdge;
	FTIHalfEdge *pNew_Edge_Twin = new FTIHalfEdge;
	FTIPolyFace *pNew_FTIPolyFace = new FTIPolyFace;

	pNew_Edge->setOrigin(i_poFTIPolyVertex1);
	pNew_Edge_Twin->setOrigin(i_poFTIPolyVertex2);
	pNew_Edge->setTwin(pNew_Edge_Twin);
	pNew_FTIPolyFace->setEdge(pNew_Edge_Twin);
	pNew_Edge_Twin->setFTIPolyFace(pNew_FTIPolyFace);

	// search for common Face:

	FTIPolyFace *CommonFace;
	FTIPolyFace *OutsideFace = m_pFTIPolyFaceVect[0];
	FTIHalfEdge *V1StartEdge = i_poFTIPolyVertex1->getEdge(); // face of reference edge for vertex 1
	FTIHalfEdge *V2StartEdge = i_poFTIPolyVertex2->getEdge(); // face of reference edge for vertex 2

	if((*(V1StartEdge->getFace()) == *(V2StartEdge->getFace())) && (*(V1StartEdge->getFace()) != *OutsideFace))
		CommonFace = V1StartEdge->getFace();
	else
	{
		FTIHalfEdge *V1IndexEdge = V1StartEdge; // face of reference edge for vertex 1

		do
		{
			FTIHalfEdge *V2IndexEdge = V2StartEdge;

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

	pNew_Edge->setFTIPolyFace(CommonFace);
	CommonFace -> setEdge(pNew_Edge);

	// Have Common FTIPolyFace, now we can add edge and add connectivity

	// Query FTIPolyVertex 1
	FTIHalfEdge *Start_Search_Edge1 = i_poFTIPolyVertex1->getEdge();
	FTIHalfEdge *Index_Edge1;

	if(((*(Start_Search_Edge1->getFace()) == *CommonFace) && (*(Start_Search_Edge1->getOrigin()) == *i_poFTIPolyVertex1)))
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
	
			if ((*(Index_Edge1->getFace()) == *CommonFace) && (*(Index_Edge1->getOrigin()) == *i_poFTIPolyVertex1))
			{
				pNew_Edge_Twin->setNext(Index_Edge1);
				pNew_Edge->setPrev(Index_Edge1->getPrev());

				Index_Edge1->getPrev()->setNext(pNew_Edge);
				Index_Edge1->setPrev(pNew_Edge_Twin);				
			};

			Index_Edge1 = (Index_Edge1->getTwin()->getNext());
		};
	};

	// Query FTIPolyVertex 2
	FTIHalfEdge *Start_Search_Edge2 = (i_poFTIPolyVertex2->getEdge());
	FTIHalfEdge *Index_Edge2;

	if(((*(Start_Search_Edge2->getFace()) == *CommonFace) && (*(Start_Search_Edge2->getOrigin()) == *i_poFTIPolyVertex2)))
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
			if ((*(Index_Edge2->getFace()) == *CommonFace) && (*(Index_Edge2->getOrigin()) == *i_poFTIPolyVertex2))
			{
				pNew_Edge->setNext(Index_Edge2);
				pNew_Edge_Twin->setPrev(Index_Edge2->getPrev());

				Index_Edge2->getPrev()->setNext(pNew_Edge_Twin);
				Index_Edge2->setPrev(pNew_Edge);
			};

			Index_Edge2 = (Index_Edge2->getTwin()->getNext());
		};
	};

	// Need to change all half edges on pNew_Edge_Twin path to have the new FTIPolyFace.
	FTIHalfEdge *Start_Edge3 = pNew_Edge_Twin;
	FTIHalfEdge *Index_Edge3 = Start_Edge3->getNext();

	while( *Index_Edge3 != *Start_Edge3 )
	{
		Index_Edge3->setFTIPolyFace(pNew_FTIPolyFace);
		Index_Edge3 = (Index_Edge3->getNext());
	};

	// Push new data to DCEL
	m_pFTIHalfEdgeVect.push_back(pNew_Edge);
	m_pFTIHalfEdgeVect.push_back(pNew_Edge_Twin);
	m_pFTIPolyFaceVect.push_back(pNew_FTIPolyFace);
};

//====================================================================
// DESCRIPTION: Write DCEL to output file
//====================================================================
void FTIDblyConnectedEdgeList::WriteToFile(std::ofstream &outputFile)
{
	outputFile << "FEMB DATA (LARGE FORMAT)" << std::endl;
	outputFile << "PART GEOMET" << std::endl;

	unsigned int numEdges = m_pFTIHalfEdgeVect.size();
	unsigned int numVert = m_pFTIPolyVertexNodes.size();

	for(unsigned int i = 0; i < numVert; i++)
	{
		double* origin = m_pFTIPolyVertexNodes[i]->getNode();

		outputFile << "LINE       "<< i << "     0.00000     0.00000     0.00000" << std::endl;
		outputFile << std::setw(10) << std::left << origin[0] << std::setw(10) << std::left << origin[1] << std::setw(10) << std::left << 0.0 << std::endl;
	};

	for(unsigned int i = 0; i < numVert; i++)
	{
		double* origin = m_pFTIHalfEdgeVect[i]->getOrigin()->getNode();
		double* end =  m_pFTIHalfEdgeVect[i]->getTwin()->getOrigin()->getNode();

		outputFile << "LINE       "<< i + numVert << "     0.00000     0.00000     0.00000" << std::endl;
		outputFile << std::setw(10) << std::left << origin[0] << std::setw(10) << std::left << origin[1] << std::setw(10) << std::left << 0.0 << std::endl;
		outputFile << std::setw(10) << std::left << end[0] << std::setw(10) << std::left << end[1] << std::setw(10) << std::left << 0.0 << std::endl;
	};

	for(unsigned int i = 2*numVert; i < numEdges; i++)
	{
		double* origin = m_pFTIHalfEdgeVect[i]->getOrigin()->getNode();
		double* end =  m_pFTIHalfEdgeVect[i]->getTwin()->getOrigin()->getNode();

		outputFile << "LINE       "<< i + numVert << "     1.00000     0.00000     0.00000" << std::endl;
		outputFile << std::setw(10) << std::left << origin[0] << std::setw(10) << std::left << origin[1] << std::setw(10) << std::left << 0.0 << std::endl;
		outputFile << std::setw(10) << std::left << end[0] << std::setw(10) << std::left << end[1] << std::setw(10) << std::left << 0.0 << std::endl;
	};
};