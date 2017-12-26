/*********************************************************************NVMH1****
File:
ulBsp.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/

#include "ULBsp.h"

/*static*/ const double BSPPlane::Epsilon = 0.001f;

/*static*/ const Poly::PolyID Poly::smInvalidPolyID = 0L;

/*static*/       Poly::PolyID Poly::smNextID = smInvalidPolyID;

/*static*/       Poly::PolyList BSPTree::smPolyList;

BSPPlane::Classification BSPPlane::ClassifyPoly( const Poly& aPoly ) const
{
    return ClassifyPoints( aPoly.GetVertexList() );
}


/*static*/ bool BSPTree::DefaultWalkFunction( BSPNode* pCurrentNode )
{
    // User should override this before walking the BSP tree
    assert( false );
    return false;
}

/*static*/ BSPTree::pBSPWalkFunction BSPTree::smpBSPWalkFunction = &(BSPTree::DefaultWalkFunction);


/*static*/ 
Poly* Poly::LookUpPoly( const PolyID& anID )
{
    Poly aPoly;
    aPoly.mID = anID;
    PolyList::iterator iter = BSPTree::smPolyList.find( aPoly );
    if ( iter != BSPTree::smPolyList.end() )
    {
        return (Poly*) &(*iter);
    }
    else
    {
        return 0;
    }
}

/*static*/
Poly& Poly::CreatePoly()
{
    Poly aPoly;
    PolyID anID = GenerateID();
    aPoly.mID = anID;
    std::pair< PolyList::iterator, bool > aPair = BSPTree::smPolyList.insert( aPoly );
    assert( aPair.second );
    return (Poly&) *( aPair.first );
}

/*static*/
Poly& Poly::CreatePoly( const BSP::VertexList& aList, const BSPPlane& aBSPPlane )
{
    Poly aPoly;
    PolyID anID = GenerateID();
    aPoly.mID = anID;
    std::pair< PolyList::iterator, bool > aPair = BSPTree::smPolyList.insert( aPoly );
    assert( aPair.second );
    (*(aPair.first)).mID = anID;
    (*(aPair.first)).mVertexList = (BSP::VertexList) aList;
    (*(aPair.first)).mBSPPlane = aBSPPlane;
    return (Poly&) *( aPair.first );
}

