//////////////////////////////////////////////////////////////////////
//
// FTIMonotoneTriangulation.cpp: This class will take as input a doubly 
// connected edge list for a polygon that has been subdivided into
// y-monotone sub-polygons. It will perform a triangulation on the 
// sub polygons
//
// Company: Forming Technologies Inc.
//
// REVISIONS:
//  Feb. 16, 2017 created (Alex Ashbourne)
//
//////////////////////////////////////////////////////////////////////
#include "FTIMonotoneTriangulation.h"

//====================================================================
// DESCRIPTION: Get all vertices in CCW around a given face
// INPUT: Pointer to query face
//====================================================================
std::vector<FTIPolyVertex*> FTIMonotoneTriangulation::GetAllVertices(FTIPolyFace *i_poPolyFace)
{
	std::vector<FTIPolyVertex*> AllVertices;

	FTIHalfEdge* StartEdge = i_poPolyFace->getEdge();

	AllVertices.push_back(StartEdge->getOrigin());

	FTIHalfEdge* IndexEdge = StartEdge->getNext();

	while( *IndexEdge != *StartEdge)
	{
		AllVertices.push_back(IndexEdge->getOrigin());

		IndexEdge = IndexEdge ->getNext();
	};

	std::sort(AllVertices.begin(), AllVertices.end(), FTIPolyVertex::FTIPolyVertexDescendingOrder());

	return AllVertices;
};

//====================================================================
// DESCRIPTION: Get all vertices in left chain of a given face
// INPUT: Vector of pointers to vertices on face, pointer to query face
//====================================================================
std::vector<FTIPolyVertex*> FTIMonotoneTriangulation::GetLeftChain(std::vector<FTIPolyVertex*> i_oAllVertices, FTIPolyFace *i_poPolyFace)
{
	std::vector<FTIPolyVertex*> LeftChain;

	FTIPolyVertex *LastVertex = (i_oAllVertices[i_oAllVertices.size()-1]);

	FTIPolyVertex *IndexVertex = (i_oAllVertices[0]);

	FTIHalfEdge *StartEdge = IndexVertex->getEdge();
	FTIHalfEdge *IndexEdge = IndexVertex->getEdge();
	do{
		if(IndexEdge->getFace() == i_poPolyFace)
			break;
		else
			IndexEdge = IndexEdge->getTwin()->getNext();
	}while(IndexEdge != StartEdge);

	while(*IndexVertex != *LastVertex)
	{
		LeftChain.push_back(IndexVertex);

		IndexEdge = IndexEdge ->getNext();
		IndexVertex = IndexEdge->getOrigin();
	};

	LeftChain.push_back(LastVertex);

	return LeftChain;
};

//====================================================================
// DESCRIPTION: Function to get all verticies in right chain of a face
// INPUT: Vector of pointers to vertices on face, pointer to query face
//====================================================================
std::vector<FTIPolyVertex*> FTIMonotoneTriangulation::GetRightChain(std::vector<FTIPolyVertex*> i_oAllVertices, FTIPolyFace *i_poPolyFace)
{
	std::vector<FTIPolyVertex*> RightChain;

	FTIPolyVertex *LastVertex = (i_oAllVertices[i_oAllVertices.size()-1]);

	FTIPolyVertex *IndexVertex = (i_oAllVertices[0]);

	FTIHalfEdge *StartEdge = IndexVertex->getEdge();
	FTIHalfEdge *IndexEdge = IndexVertex->getEdge();
	do{
		if(IndexEdge->getFace() == i_poPolyFace)
			break;
		else
			IndexEdge = IndexEdge->getTwin()->getNext();
	}while(IndexEdge != StartEdge);

	while(*IndexVertex != *LastVertex)
	{
		RightChain.push_back(IndexVertex);

		IndexEdge = IndexEdge ->getPrev();
		IndexVertex = IndexEdge->getOrigin();
	};

	RightChain.push_back(LastVertex);

	return RightChain;
};

//====================================================================
// DESCRIPTION: Boolean function to determine if a vertex is in a chain.
// TRUE if it is in chain, FLASE if NOT in chain
//
// INPUT: Pointer to vertex, chain of vertices
//====================================================================
bool FTIMonotoneTriangulation::IsInChain(FTIPolyVertex *i_poVertex, std::vector<FTIPolyVertex*> i_oChain)
{
	for(unsigned int i = 0; i < i_oChain.size(); i++)
	{
		if(*i_poVertex == *i_oChain[i])
			return true;
	};

	return false;

};

//====================================================================
// DESCRIPTION: Function to determine if two vertices are in the same chain.
// TRUE if they ARE NOT in chain, FALSE if THEY ARE.
//
// INPUT: Pointers to two vertices and one of the chains
//====================================================================
bool FTIMonotoneTriangulation::NotInSameChain(FTIPolyVertex *i_poVertex1, FTIPolyVertex *i_poVertex2, std::vector<FTIPolyVertex*> i_oChain)
{
	if (IsInChain(i_poVertex1, i_oChain) && IsInChain(i_poVertex2, i_oChain))
		return false;
	else if (IsInChain(i_poVertex1, i_oChain) && !IsInChain(i_poVertex2, i_oChain))
		return true;
	else if (!IsInChain(i_poVertex1, i_oChain) && IsInChain(i_poVertex2, i_oChain))
		return true;
	else
		return false;
};

//====================================================================
// DESCRIPTION: Function looping through each monotone face of a 
// monotone subdivision. Calls private function to triangulate a face
//====================================================================
void FTIMonotoneTriangulation::TriangulatePolygon()
{
	int nNumFaces = m_poMonotoneDCEL->getNumFaces();

	std::vector<FTIPolyFace*> Vector_Faces = m_poMonotoneDCEL->getFaces();

	for(int i = 1; i < nNumFaces; i++)
	{
		TriangulateMonotonePolygon(Vector_Faces[i], m_poMonotoneDCEL);
	};

};

//====================================================================
// DESCRIPTION: Private function to triangulate a monotone face of a polygon
// INPUT: pointer to the face which will be triangulated, pointer to reference 
// of DCEL.
//====================================================================
void FTIMonotoneTriangulation::TriangulateMonotonePolygon(FTIPolyFace *&i_poPolyFace, FTIDblyConnectedEdgeList *&io_poMonotoneDCEL)
{
	// Sort Vertices in descending order
	std::vector<FTIPolyVertex*> AllVertices = GetAllVertices(i_poPolyFace);
	// Create Left Chain of vertices 
	std::vector<FTIPolyVertex*> LeftChain = GetLeftChain(AllVertices, i_poPolyFace);
	// Create Right Chain of vertices (not necessary?)
	std::vector<FTIPolyVertex*> RightChain = GetRightChain(AllVertices, i_poPolyFace);

	// Initialize an empty stack
	std::vector<FTIPolyVertex*> VertexStack;

	int nNumVert = AllVertices.size();

	// push top two vertices to the stack
	VertexStack.push_back(AllVertices[0]);
	VertexStack.push_back(AllVertices[1]);

	for(int i = 2; i < nNumVert - 1; i++) 
	{
		if(NotInSameChain(AllVertices[i], VertexStack[VertexStack.size()-1], LeftChain))
		{ // Vertices are not in the same chain
			for(int j = VertexStack.size()-1; j > 0; j--)
			{
				// Get vertex from the stack
				FTIPolyVertex *NewVertex = VertexStack.back();

				// create new edge
				FTIPolyVertex* MaxVert = GetMax(AllVertices[i], NewVertex);
				FTIPolyVertex* MinVert = GetMin(AllVertices[i], NewVertex);
				io_poMonotoneDCEL -> InsertNewEdge(MaxVert, MinVert);

				// pop vertex from the stack
				VertexStack.pop_back();
			};

			// pop last vertex (stack should be empty)
			VertexStack.pop_back();

			// push last two vertices to the stack
			VertexStack.push_back(AllVertices[i-1]);
			VertexStack.push_back(AllVertices[i]);
			
		}
		else // vertices are in the same chain
		{
			FTIPolyVertex *TestVertex = VertexStack.back();

			VertexStack.pop_back();

			//for(int j = VertexStack.size()-1; j > 0; j--)
			while(!VertexStack.empty())
			{
				// Get vertex from the stack
				FTIPolyVertex *NewVertex = VertexStack.back();

				if(IsInChain(AllVertices[i], LeftChain))
				{// if vertex is in left chain, insert new edge if orientation > 0
					if(Orientation(TestVertex, AllVertices[i], NewVertex) > 0)
					{
						FTIPolyVertex* MaxVert = GetMax(AllVertices[i], NewVertex);
						FTIPolyVertex* MinVert = GetMin(AllVertices[i], NewVertex);
						io_poMonotoneDCEL -> InsertNewEdge(MaxVert, MinVert);
					}
					else
					{
						break;
					};
				}
				else
				{// if vertex is in right chain, insert new edge if orientation < 0
					if(Orientation(TestVertex, AllVertices[i], NewVertex) < 0)
					{
						FTIPolyVertex* MaxVert = GetMax(AllVertices[i], NewVertex);
						FTIPolyVertex* MinVert = GetMin(AllVertices[i], NewVertex);
						io_poMonotoneDCEL -> InsertNewEdge(MaxVert, MinVert);
					}
					else
					{
						break;
					};
				};
				
				// Switch test vertex to most recently popped
				TestVertex = NewVertex;

				// pop it from the stack
				VertexStack.pop_back();

			};// end of same chain loop

			// push last verted that has been popped to stack
			VertexStack.push_back(TestVertex);

			// push current vertex to stack
			VertexStack.push_back(AllVertices[i]);
		
		}; // end of major conditional check
	
	}; // end of vertex loop

	// pop last vertex in stack
	VertexStack.pop_back();

	// Need to connect last verted to all vertices remaining in the stack
	for(int j = VertexStack.size()-1; j > 0; j--)
	{
		// Get vertex from the stack
		FTIPolyVertex *NewVertex = VertexStack.back();

		// create new edge
		FTIPolyVertex* MaxVert = GetMax(AllVertices[AllVertices.size()-1], NewVertex);
		FTIPolyVertex* MinVert = GetMin(AllVertices[AllVertices.size()-1], NewVertex);
		io_poMonotoneDCEL -> InsertNewEdge(MaxVert, MinVert);

		// pop vertex from the stack
		VertexStack.pop_back();
	};

	// triangulation complete
};// end of TriangulateMonotonePolygon function