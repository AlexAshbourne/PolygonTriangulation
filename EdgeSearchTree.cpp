//////////////////////////////////////////////////////////////////////
//
// EdgeSearchTree.h: Need this tree for "event handling" during the
// make monotone function of the triangluation.
//
// Elements of the tree are edges
//
// REVISIONS:
//  Feb. 15, 2017 created (Alex Ashbourne)
//
//////////////////////////////////////////////////////////////////////

#include<iostream>
#include <algorithm>
#include "EdgeSearchTree.h"

//////////////////////////////////////////////////////////////////////
//
// Public call to insert half edge
//
//////////////////////////////////////////////////////////////////////
void EdgeSearchTree::InsertHalfEdge(HalfEdge* i_poHalfEdge)
{
	InsertHalfEdge(i_poHalfEdge, m_pEdgeTreeRoot);
};

//////////////////////////////////////////////////////////////////////
//
// Public call to remove half node
//
//////////////////////////////////////////////////////////////////////
void EdgeSearchTree::RemoveHalfEdge(HalfEdge* i_poHalfEdge)
{
	RemoveHalfEdge(i_poHalfEdge, m_pEdgeTreeRoot);
};

//////////////////////////////////////////////////////////////////////
//
// Public call to insert edge node
//
//////////////////////////////////////////////////////////////////////
void EdgeSearchTree::InsertEdgeNode(EdgeNode* i_poEdgeNode)
{
	InsertEdgeNode(i_poEdgeNode, m_pEdgeTreeRoot);
};

//////////////////////////////////////////////////////////////////////
//
// Public call to remove edge node
//
//////////////////////////////////////////////////////////////////////
void EdgeSearchTree::RemoveEdgeNode(EdgeNode* i_poEdgeNode)
{
	RemoveEdgeNode(i_poEdgeNode, m_pEdgeTreeRoot);
};

//////////////////////////////////////////////////////////////////////
//
// Boolean to test of tree is empty
//
//////////////////////////////////////////////////////////////////////
bool EdgeSearchTree::isEmpty() const
{
	return m_pEdgeTreeRoot == nullptr;
};

//////////////////////////////////////////////////////////////////////
//
// Call to empty the tree
//
//////////////////////////////////////////////////////////////////////
void EdgeSearchTree::makeEmpty()
{
	makeEmpty(m_pEdgeTreeRoot);
};

//////////////////////////////////////////////////////////////////////
//
// Private call to empty the tree recurrsively
//
//////////////////////////////////////////////////////////////////////
void EdgeSearchTree::makeEmpty( EdgeNode* &i_poEdgeNode)
{
	if(!i_poEdgeNode)
	{
		makeEmpty(i_poEdgeNode->m_pLeftElement);
		makeEmpty(i_poEdgeNode->m_pRightElement);
		delete i_poEdgeNode;
	};
	i_poEdgeNode = nullptr;
};

//////////////////////////////////////////////////////////////////////
//
// Function to return the edge node that holds a half edge
//
//////////////////////////////////////////////////////////////////////
EdgeNode* EdgeSearchTree::FindEdgeNode(HalfEdge* i_poHalfEdge)
{
	return FindEdgeNode(i_poHalfEdge, m_pEdgeTreeRoot);
};

//////////////////////////////////////////////////////////////////////
//
// Private Recursive Function to return the edge node that holds a half edge
//
//////////////////////////////////////////////////////////////////////
EdgeNode* EdgeSearchTree::FindEdgeNode(HalfEdge* i_poHalfEdge, EdgeNode* i_poEdgeNode)
{
	if(!i_poEdgeNode)
		return nullptr;
	else if(*i_poHalfEdge % *(i_poEdgeNode->m_pElement))
		return FindEdgeNode(i_poHalfEdge, i_poEdgeNode->m_pLeftElement);
	else if(*(i_poEdgeNode->m_pElement) % *i_poHalfEdge)
		return FindEdgeNode(i_poHalfEdge, i_poEdgeNode->m_pRightElement);
	else
		return i_poEdgeNode;
};

//////////////////////////////////////////////////////////////////////
//
// Public call to find most left edge
//
//////////////////////////////////////////////////////////////////////
HalfEdge* EdgeSearchTree::FindMostLeft()
{
	return FindMostLeft(m_pEdgeTreeRoot)->m_pElement;
};

//////////////////////////////////////////////////////////////////////
//
// Public call to find closest edge to the left of a vertex
//
//////////////////////////////////////////////////////////////////////
EdgeNode* EdgeSearchTree::FindClosestLeft(PolyVertex *i_oPolyVertex)
{
	return FindClosestLeft(i_oPolyVertex, m_pEdgeTreeRoot);
};

//////////////////////////////////////////////////////////////////////
//
// Recursive function to find most left edge
//
//////////////////////////////////////////////////////////////////////
EdgeNode* EdgeSearchTree::FindMostLeft(EdgeNode *i_oEdgeNode)
{
	if(!i_oEdgeNode)
		return nullptr;
	if(!(i_oEdgeNode->m_pLeftElement))
		return i_oEdgeNode;
	return FindMostLeft(i_oEdgeNode->m_pLeftElement);
};

// //////////////////////////////////////////////////////////////////////
// //
// // Recursive function to find closest edge to the left of a vertex
// //
// //////////////////////////////////////////////////////////////////////
// EdgeNode* EdgeSearchTree::FindClosestLeft(PolyVertex *i_oPolyVertex, EdgeNode *i_oEdgeNode)
// {
// 	if(!i_oEdgeNode)
// 		return nullptr;
// 	else if(*(i_oPolyVertex->getEdge()) ^ *(i_oEdgeNode->m_pElement))
// 	{
// 		if((i_oEdgeNode->m_pLeftElement))
// 		{
// 			if(*(i_oPolyVertex->getEdge()) ^ *(i_oEdgeNode->m_pLeftElement->m_pElement))
// 				return FindClosestLeft(i_oPolyVertex, i_oEdgeNode->m_pLeftElement);
// 			else
// 				return i_oEdgeNode->m_pLeftElement;
// 		}
// 		else
// 			return i_oEdgeNode;
// 	}
// 	else if( *(i_oPolyVertex->getEdge()) *= *(i_oEdgeNode->m_pElement))
// 	{
// 		if((i_oEdgeNode->m_pRightElement))
// 		{
// 			if(*(i_oPolyVertex->getEdge()) *= *(i_oEdgeNode->m_pRightElement->m_pElement))
// 				return FindClosestLeft(i_oPolyVertex, i_oEdgeNode->m_pRightElement);
// 			else
// 				return i_oEdgeNode;
// 		}
// 		else
// 			return i_oEdgeNode;
// 	};
// 		//return FindClosestLeft(i_oPolyVertex, i_oEdgeNode->m_pRightElement);
// 	return FindMostLeft(i_oEdgeNode->m_pLeftElement);
// };

//////////////////////////////////////////////////////////////////////
//
// Recursive function to find closest edge to the left of a vertex
//
//////////////////////////////////////////////////////////////////////
EdgeNode* EdgeSearchTree::FindClosestLeft(PolyVertex *i_oPolyVertex, EdgeNode *i_oEdgeNode)
{
	if(!i_oEdgeNode)
		return nullptr;
	else 
	{
		EdgeNode* ReturnNode = i_oEdgeNode;
		EdgeNode* CurrentNode = i_oEdgeNode;

		while(CurrentNode)
		{
			if(IsVertexLeftofEdge(i_oPolyVertex, CurrentNode->m_pElement))
				CurrentNode = CurrentNode -> m_pLeftElement;
			else
			{
				ReturnNode = CurrentNode;
				CurrentNode = CurrentNode -> m_pRightElement;
			};
		};

		return ReturnNode;
	};
};

//////////////////////////////////////////////////////////////////////
//
// Recursive half edge insert to tree
//
//////////////////////////////////////////////////////////////////////

void EdgeSearchTree::InsertHalfEdge(HalfEdge* i_poHalfEdge, EdgeNode *&i_oEdgeNode)
{
	//if(i_oEdgeNode == nullptr)
	if(!i_oEdgeNode)
	{
		i_oEdgeNode = new EdgeNode(i_poHalfEdge, nullptr, nullptr);
	}
	else if (*i_poHalfEdge % *(i_oEdgeNode->m_pElement))
		InsertHalfEdge(i_poHalfEdge, i_oEdgeNode->m_pLeftElement);
	else if (*(i_oEdgeNode->m_pElement) % *i_poHalfEdge)
		InsertHalfEdge(i_poHalfEdge, i_oEdgeNode->m_pRightElement);
	else
		;

	BalanceTree(i_oEdgeNode);
};

//////////////////////////////////////////////////////////////////////
//
// Recursive half edge remove from tree
//
//////////////////////////////////////////////////////////////////////
void EdgeSearchTree::RemoveHalfEdge(HalfEdge* i_poHalfEdge, EdgeNode *&i_oEdgeNode)
{
	if(!i_oEdgeNode)
		return; // do nothing

	if (*i_poHalfEdge % *(i_oEdgeNode->m_pElement))
		RemoveHalfEdge(i_poHalfEdge, i_oEdgeNode->m_pLeftElement);
	else if (*(i_oEdgeNode->m_pElement) % *i_poHalfEdge)
		RemoveHalfEdge(i_poHalfEdge, i_oEdgeNode->m_pRightElement);
	else if( i_oEdgeNode->m_pLeftElement != nullptr && i_oEdgeNode->m_pRightElement != nullptr )
	{
		i_oEdgeNode->m_pElement = FindMostLeft(i_oEdgeNode->m_pRightElement)->m_pElement;
		RemoveHalfEdge(i_oEdgeNode->m_pElement,i_oEdgeNode->m_pRightElement);
	}
	else
	{
		EdgeNode *pOldNode = i_oEdgeNode;
		i_oEdgeNode = (i_oEdgeNode->m_pLeftElement != nullptr) ? i_oEdgeNode->m_pLeftElement : i_oEdgeNode->m_pRightElement;
		delete pOldNode;
	};

	BalanceTree(i_oEdgeNode);
	
};

//////////////////////////////////////////////////////////////////////
//
// Recursive edge node insert to tree
//
//////////////////////////////////////////////////////////////////////
void EdgeSearchTree::InsertEdgeNode(EdgeNode* i_poNewEdgeNode, EdgeNode *&i_oEdgeNode)
{
	if(!i_oEdgeNode)
	{
		i_oEdgeNode = i_poNewEdgeNode;
	}
	else if (*(i_poNewEdgeNode->m_pElement) % *(i_oEdgeNode->m_pElement))
		InsertEdgeNode(i_poNewEdgeNode, i_oEdgeNode->m_pLeftElement);
	else if (*(i_oEdgeNode->m_pElement) % *(i_poNewEdgeNode->m_pElement))
		InsertEdgeNode(i_poNewEdgeNode, i_oEdgeNode->m_pRightElement);
	else
		;

	BalanceTree(i_oEdgeNode);
};

//////////////////////////////////////////////////////////////////////
//
// Recursive edge node removal from tree
//
//////////////////////////////////////////////////////////////////////
void EdgeSearchTree::RemoveEdgeNode(EdgeNode* i_poNewEdgeNode, EdgeNode *&i_oEdgeNode)
{
	if(!i_oEdgeNode)
		return; // do nothing

	if (*(i_poNewEdgeNode->m_pElement) % *(i_oEdgeNode->m_pElement))
		RemoveEdgeNode(i_poNewEdgeNode, i_oEdgeNode->m_pLeftElement);
	else if (*(i_oEdgeNode->m_pElement) % *(i_poNewEdgeNode->m_pElement))
		RemoveEdgeNode(i_poNewEdgeNode, i_oEdgeNode->m_pRightElement);
	else if( i_oEdgeNode->m_pLeftElement != nullptr && i_oEdgeNode->m_pRightElement != nullptr )
	{
		i_oEdgeNode->m_pElement = FindMostLeft(i_oEdgeNode->m_pRightElement)->m_pElement;
		i_oEdgeNode->m_pHelperVertex = FindMostLeft(i_oEdgeNode->m_pRightElement)->m_pHelperVertex;
		RemoveEdgeNode(i_oEdgeNode,i_oEdgeNode->m_pRightElement);
	}
	else
	{
		EdgeNode *pOldNode = i_oEdgeNode;
		i_oEdgeNode = (i_oEdgeNode->m_pLeftElement != nullptr) ? i_oEdgeNode->m_pLeftElement : i_oEdgeNode->m_pRightElement;
		delete pOldNode;
	};

	BalanceTree(i_oEdgeNode);
};

//////////////////////////////////////////////////////////////////////
//
// Calculate the height of an edge node (used for balancing tree)
//
//////////////////////////////////////////////////////////////////////
int Height(EdgeNode *i_poEdgeNode) 
{// Find height of node, if null return -1
	return i_poEdgeNode == nullptr ? -1 : i_poEdgeNode->m_nHeight;
}

//////////////////////////////////////////////////////////////////////
//
// Rotation with left child for balancing
//
//////////////////////////////////////////////////////////////////////
void EdgeSearchTree::rotateWithLeftChild(EdgeNode *& i_poEdgeNode)
{
	EdgeNode *OtherEdge = i_poEdgeNode -> m_pLeftElement;
	i_poEdgeNode->m_pLeftElement = OtherEdge->m_pRightElement;
	OtherEdge->m_pRightElement = i_poEdgeNode;

	// adjust heights
	i_poEdgeNode->m_nHeight = std::max(Height(i_poEdgeNode->m_pLeftElement), Height(i_poEdgeNode->m_pRightElement)) + 1;
	OtherEdge->m_nHeight = std::max(Height(OtherEdge->m_pLeftElement), i_poEdgeNode->m_nHeight) + 1;

	i_poEdgeNode = OtherEdge;
};

//////////////////////////////////////////////////////////////////////
//
// Rotation with right child for balancing
//
//////////////////////////////////////////////////////////////////////
void EdgeSearchTree::rotateWithRightChild(EdgeNode *& i_poEdgeNode)
{
	EdgeNode *OtherEdge = i_poEdgeNode -> m_pRightElement;
	i_poEdgeNode->m_pRightElement = OtherEdge->m_pLeftElement;
	OtherEdge->m_pLeftElement = i_poEdgeNode;

	// adjust heights
	i_poEdgeNode->m_nHeight = std::max(Height(i_poEdgeNode->m_pLeftElement), Height(i_poEdgeNode->m_pRightElement)) + 1;
	OtherEdge->m_nHeight = std::max(Height(OtherEdge->m_pRightElement), i_poEdgeNode->m_nHeight) + 1;

	i_poEdgeNode = OtherEdge;
};

//////////////////////////////////////////////////////////////////////
//
// Double Rotation with left child for balancing
//
//////////////////////////////////////////////////////////////////////
void EdgeSearchTree::doubleWithLeftChild(EdgeNode *& i_poEdgeNode)
{
	rotateWithRightChild(i_poEdgeNode->m_pLeftElement);
	rotateWithLeftChild(i_poEdgeNode);
};

//////////////////////////////////////////////////////////////////////
//
// Double Rotation with left child for balancing
//
//////////////////////////////////////////////////////////////////////
void EdgeSearchTree::doubleWithRightChild(EdgeNode *& i_poEdgeNode)
{
	rotateWithLeftChild(i_poEdgeNode->m_pRightElement);
	rotateWithRightChild(i_poEdgeNode);
};

//////////////////////////////////////////////////////////////////////
//
// Balance tree at given node
//
//////////////////////////////////////////////////////////////////////
void EdgeSearchTree::BalanceTree(EdgeNode *& i_poEdgeNode)
{
	if(!i_poEdgeNode)
		return;

	if( Height(i_poEdgeNode->m_pLeftElement) - Height(i_poEdgeNode->m_pRightElement) > ALLOWED_IMBALANCE)
	{
		if(Height(i_poEdgeNode->m_pLeftElement->m_pLeftElement) >= Height(i_poEdgeNode->m_pLeftElement->m_pRightElement))
			rotateWithLeftChild(i_poEdgeNode);
		else
			doubleWithLeftChild(i_poEdgeNode);
	}
	else if( Height(i_poEdgeNode->m_pRightElement) - Height(i_poEdgeNode->m_pLeftElement) > ALLOWED_IMBALANCE)
	{
		if(Height(i_poEdgeNode->m_pRightElement->m_pRightElement) >= Height(i_poEdgeNode->m_pRightElement->m_pLeftElement))
			rotateWithRightChild(i_poEdgeNode);
		else
			doubleWithRightChild(i_poEdgeNode);
	};

	i_poEdgeNode->m_nHeight = std::max(Height(i_poEdgeNode->m_pLeftElement), Height(i_poEdgeNode->m_pRightElement)) + 1;
};