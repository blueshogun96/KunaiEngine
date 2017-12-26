/*********************************************************************NVMH2****
Path:  c:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulBsp.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/

#ifndef _BSP_H_
#define _BSP_H_

#pragma warning( disable : 4786 )

#include <ul.h>
#include <list>
#include <vector>
#include <stack>
#include <set>
#ifndef MULTI_THREAD
#include "windows.h"
#endif

#include <Assert.h>

class Poly;
class BSPTree;

namespace BSP
{
    typedef std::vector< Vec3 >  VertexList;
};

template < class T >
INLINE
T LinearInterpolation( const T& aStart, const T& aEnd, const real& anAmount, T& theOutput)
{
    theOutput = aStart + ( aEnd - aStart ) * anAmount;
    return theOutput;
}

class BSPPlane
{
    private :
        static const double Epsilon;

        Vec3 mNormal;
        real   mDistance;

        friend class BSPNode;
        real TestPoint( const Vec3& aPoint ) const
        {
            real theDistanceFromBSPPlane = ( mNormal DOT aPoint ) + mDistance;
            if ( fabs( theDistanceFromBSPPlane ) <= Epsilon )
            {
                return 0.0f;
            }
            else
            {
                return theDistanceFromBSPPlane;
            }
        }

    public :
        typedef enum _Classification
        {
            Coplanar = 0,
            Intersection,
            Front,
            Back
        }Classification;

        BSPPlane( const real& x = 0.0f, const real& y = 0.0f , const real& z = 1.0f , const real& d = 0.0f )
            : mNormal( x, y, z ), mDistance( d )
        {
            assert( mNormal.GetMagnitudeSquared() == 1.0f ); // Nothing
        }

        BSPPlane( const Vec3& aNormal, const real& d ) : mNormal( aNormal ), mDistance( d )
        {
            assert( mNormal.GetMagnitudeSquared() == 1.0f ); // Nothing
        }

        BSPPlane( const BSPPlane& aBSPPlane )
        {
            (*this) = aBSPPlane;
        }
        const Vec3& GetNormal() const
        {
            return mNormal;
        }
        const real& GetDistance() const
        {
            return mDistance;
        }

        void Set( const Vec3& aNormal, const real& d )
        {
            mNormal = aNormal;
            mDistance = d;
        }
        Classification ClassifyPoint( const Vec3& aPoint ) const
        {
            real theTest = TestPoint( aPoint );

            if ( theTest > 0.0f )
            {
                return Front;
            }
            else
            if ( theTest < 0.0f )
            {
                return Back;
            }
            return Coplanar;
        }

        Classification ClassifyPoints( const BSP::VertexList& thePoints ) const
        {
            real theSum = 0.0f;
            real theAbsoluteSum = 0.0f;
            for ( unsigned int i = 0; i < thePoints.size() ; ++i )
            {
                const real theNext = TestPoint( thePoints[ i ] );
                theSum += theNext;
                theAbsoluteSum += ( fabs( theNext ) );
            }

            if ( theAbsoluteSum == 0.0f )
            {
                return Coplanar;
            }
            else
            if ( theSum == theAbsoluteSum )
            {
                return Front;
            }
            else
            if ( theSum == -theAbsoluteSum  )
            {
                return Back;
            }
            return Intersection;
        }

        Classification ClassifyPoly( const Poly& aPoly ) const;
};


class Poly
{
public :

    typedef unsigned int PolyID;
    typedef std::set< Poly >        PolyList;

    private :

    static const PolyID smInvalidPolyID;
    static PolyID smNextID;

    static PolyID GenerateID()
    {
        return ++smNextID;
    }

    PolyID  mID;
    BSPPlane      mBSPPlane;
    BSP::VertexList mVertexList;


    Poly(){;}

    Poly( const BSP::VertexList& aList, const BSPPlane& aBSPPlane ) : mVertexList( aList ), mBSPPlane( aBSPPlane )
    {
        ;
    }

    public :

    void CalculateBSPPlaneEquation()
    {
        assert( mVertexList.size() >= 3 );

        Vec3 aNormal = ( mVertexList[ 1 ] - mVertexList[ 0 ] ) CP
                          ( mVertexList[ 2 ] - mVertexList[ 1 ] );

        real aLength = aNormal.GetMagnitude();

        if ( aLength != 0 )
        {
            aNormal /= aLength;
        }
        else
        {
            assert( false );
        }

        real aDistance = ( mVertexList[ 0 ] DOT aNormal );

        mBSPPlane.Set( aNormal, aDistance );
    }

    bool operator<( const Poly& aPoly ) const
    {
        return ( mID < aPoly.mID );
    }
    bool operator==( const Poly& aPoly ) const
    {
        return ( mID == aPoly.mID );
    }

    static Poly* LookUpPoly( const PolyID& anID );
    static Poly& CreatePoly();
    
    static Poly& CreatePoly( const BSP::VertexList& aList, const BSPPlane& aBSPPlane );

    const PolyID& GetID() const
    {
        return mID;
    }

    const BSPPlane& GetBSPPlane() const
    {
        return mBSPPlane;
    }
    const BSP::VertexList& GetVertexList() const
    {
        return mVertexList;
    }

    void SetVertexList( const BSP::VertexList& aList )
    {
        // Assuming the normal is still valid for these vertices
        mVertexList = aList;
    }

    const Vec3& GetNormal() const
    {
        return mBSPPlane.GetNormal();
    }
    const real& GetDistance() const
    {
        return mBSPPlane.GetDistance();
    }

};


class BSPNode
{
    private :

        typedef std::list< Poly::PolyID > PolyList;
        //typedef std::list< ObjectID > ObjectList;

        // List of polygons that are coplanar with my partition BSPPlane
        PolyList mCoplanarPolyList;

        // List of objects that are partially within my leaf-node subspace
        PolyList mLeafPolyList;

        BSPPlane mBSPPlane;

        BSPNode* mpFront;
        BSPNode* mpBack;

    public :

        virtual ~BSPNode()
        {
            delete mpFront, mpFront = 0;
            delete mpBack, mpBack = 0;
        }

        BSPNode( const real x = 0.0f, const real y = 0.0f, const real z = 1.0f, const real aDistance = 0.0f ) : mpFront( 0 ), mpBack( 0 ), mBSPPlane( x, y, z, aDistance )
        {
            ;
        }

        BSPNode( const Vec3& aNormal, const real aDistance ) : mpFront( 0 ), mpBack( 0 ), mBSPPlane( aNormal, aDistance )
        {
            ;
        }

        BSPNode( const BSPPlane& aBSPPlane ) : mpFront( 0 ), mpBack( 0 ), mBSPPlane( aBSPPlane )
        {
            ;
        }

        BSPNode( const Poly& aPoly ): mpFront( 0 ), mpBack( 0 ), mBSPPlane( aPoly.GetBSPPlane() )
        {
            mCoplanarPolyList.push_back( aPoly.GetID() );
        }

        BSPPlane::Classification ClassifyPoint( const Vec3& aPoint ) const
        {
            return mBSPPlane.ClassifyPoint( aPoint );
        }

        BSPPlane::Classification ClassifyPoly( const Poly& aPoly ) const
        {
            return mBSPPlane.ClassifyPoly( aPoly );
        }

        BSPNode* GetFront() const
        {
            return mpFront;
        }   
        BSPNode* GetBack() const
        {
            return mpBack;
        }   

        bool IsLeafNode() const
        {
            return ( (reinterpret_cast<unsigned int>(mpFront) | (reinterpret_cast<unsigned int>(mpBack) ) ) == 0 );
        }

        bool InsertAndSplitPoly( const Poly& aPoly )
        {
            std::vector< real > aVector;

            aVector.reserve( aPoly.GetVertexList().size() );

            real theSum = 0.0f;
            real theAbsoluteSum = 0.0f;

            for ( unsigned int i = 0; i < aPoly.GetVertexList().size(); ++i )
            {
                const real theNext = mBSPPlane.TestPoint( aPoly.GetVertexList()[ i ] );
                aVector.push_back( theNext );
                theSum += theNext;
                theAbsoluteSum += ( fabs( theNext ) );
            }

            if ( theAbsoluteSum == 0.0f )
            {
                  // The Polygon's extents are not split by this leaf's partitioning BSPPlane, so no need to split the poly,
                  // just add it to the coplanar list
                mCoplanarPolyList.push_back( aPoly.GetID() );
                return true;
            }
            else // It's on our front side completely
            if ( theSum == theAbsoluteSum ) 
            {
                if ( mpFront != 0 )
                {
                    return mpFront->InsertAndSplitPoly( aPoly );
                }
                else
                {
                    // No need to recurse again, b/c this is a new leaf node
                    mpFront = new BSPNode( aPoly );
                    return true;
                }
            }
            else // It's on our back side completely
            if ( theSum == -theAbsoluteSum ) 
            {
                if ( mpBack != 0 )
                {
                    return mpBack->InsertAndSplitPoly( aPoly );
                }
                else
                {
                    // No need to recurse again, b/c this is a new leaf node
                    mpBack = new BSPNode( aPoly );
                    return true;
                }
            }
            else // The poly is split by our partition BSPPlane, clip it
            {
                BSP::VertexList aFrontVertexList;
                BSP::VertexList aBackVertexList;

                aFrontVertexList.reserve( aPoly.GetVertexList().size() );
                aBackVertexList.reserve( aPoly.GetVertexList().size() );

                // Find points of intersection and split this poly along our partition BSPPlane
                // If we got here, we know there are is least one non-zero distance in the vector
                real theLast = aVector[ 0 ];
                Vec3 theLastPoint = aPoly.GetVertexList()[ 0 ] ;

                real theNext;
                Vec3 theNextPoint;
                Vec3 theNewPoint;

                for ( unsigned int i = 1; i <= aVector.size(); ++i )
                {
                    if ( i == aVector.size() )
                    {
                        theNext = aVector[ 0 ];
                        theNextPoint = aPoly.GetVertexList()[ 0 ];
                    }
                    else
                    {
                        theNext = aVector[ i ];
                        theNextPoint = aPoly.GetVertexList()[ i ];
                    }

                    if ( theNext > 0.0f )
                    {
                        if ( theLast < 0.0f )
                        {
                            // theNext and theLast can't be equal, or it wouldn't get here. so no / by 0 check
                            real theScale = ( ( theNext ) / ( theNext - theLast ) );
                            LinearInterpolation< Vec3 >( theLastPoint, theNextPoint, theScale, theNewPoint );
                            aBackVertexList.push_back( theNewPoint );
                            aFrontVertexList.push_back( theNewPoint );
                            aFrontVertexList.push_back( theNextPoint );
                        }
                        else // they're both in
                        if ( theLast > 0.0f )
                        {
                            aFrontVertexList.push_back( theNextPoint );
                        }
                    }
                    else
                    if ( theNext < 0.0f ) 
                    {
                        if ( theLast > 0.0f )
                        {
                            // theNext and theLast can't be equal, or it wouldn't get here. so no / by 0 check
                            real theScale = ( ( theNext ) / ( theNext - theLast ) );
                            LinearInterpolation< Vec3 >( theLastPoint, theNextPoint, theScale, theNewPoint );
                            aBackVertexList.push_back( theNewPoint );
                            aBackVertexList.push_back( theNextPoint );
                            aFrontVertexList.push_back( theNewPoint );
                        }
                        else // they're both out
                        if ( theLast < 0.0f )
                        {
                            aBackVertexList.push_back( theNextPoint );
                        }
                    }

                    if ( theNext == 0.0f ) 
                    {
                        // It's on the BSPPlane, so don't clip it
                        //  it will be part of both front & back polys
                        aBackVertexList.push_back( theNextPoint );
                        aFrontVertexList.push_back( theNextPoint );
                    }
                    theLastPoint = theNextPoint;
                    theLast = theNext;
                }

                if ( aFrontVertexList.size() >= 3 )
                {
                    // Add the vertices to the poly
                    Poly frontPoly( Poly::CreatePoly( aFrontVertexList, aPoly.GetBSPPlane() ) );

                    // Now add the new Front Poly
                    if ( mpFront != 0 )
                    {
                        mpFront->InsertAndSplitPoly( frontPoly );
                    }
                    else
                    {
                        // No need to recurse again, b/c this is a new leaf node
                        mpFront = new BSPNode( frontPoly );
                    }
                }
                else
                {
                    assert( false );
                }

                if ( aBackVertexList.size() >= 3 )
                {
                    Poly backPoly( Poly::CreatePoly( aBackVertexList, aPoly.GetBSPPlane() ) );
                    if ( mpBack != 0 )
                    {
                        return mpBack->InsertAndSplitPoly( backPoly );
                    }
                    else
                    {
                        // No need to recurse again, b/c this is a new leaf node
                        mpBack = new BSPNode( backPoly );
                        return true;
                    }
                }
                else
                {
                    // Degenerate triangle, not inserting it
                    assert( false );
                    return false;
                }
            }
        }

        // Inserts poly into any leaf nodes it intersects
        bool InsertPolyDontSplit( const Poly& aPoly )
        {
            std::vector< real > aVector;

            aVector.reserve( aPoly.GetVertexList().size() );

            real theSum = 0.0f;
            real theAbsoluteSum = 0.0f;

            for ( unsigned int i = 0; i < aPoly.GetVertexList().size(); ++i )
            {
                const real theNext = mBSPPlane.TestPoint( aPoly.GetVertexList()[ i ] );
                aVector.push_back( theNext );
                theSum += theNext;
                theAbsoluteSum += ( fabs( theNext ) );
            }

            if ( theAbsoluteSum == 0.0f )
            {
                  // The Polygon's extents are not split by this leaf's partitioning BSPPlane, so no need to split the poly,
                  // just add it to the coplanar list
                mCoplanarPolyList.push_back( aPoly.GetID() );
                return true;
            }
            else // It's on our front side completely
            if ( theSum == theAbsoluteSum ) 
            {
                if ( mpFront != 0 )
                {
                    return mpFront->InsertPolyDontSplit( aPoly );
                }
                else // it goes here
                {
                    mLeafPolyList.push_back( aPoly.GetID() );
                    return true;
                }
            }
            else // It's on our back side completely
            if ( theSum == -theAbsoluteSum ) 
            {
                if ( mpBack != 0 )
                {
                    return mpBack->InsertPolyDontSplit( aPoly );
                }
                else // it goes here
                {
                    mLeafPolyList.push_back( aPoly.GetID() );
                    return true;
                }
            }
            else // its in between, try to go down to a leaf node
            {
                bool bSuccess1 = true;
                bool bSuccess2 = true;
                // if there is a node here, send it down
                if ( mpFront != 0 )
                {
                    bSuccess1 = mpFront->InsertPolyDontSplit( aPoly );
                }
                else
                {
                    mLeafPolyList.push_back( aPoly.GetID() );
                }

                if ( mpBack != 0 )
                {
                    bSuccess2 = mpBack->InsertPolyDontSplit( aPoly );
                }
                else // as long as we didn't already put it in above
                if ( mpFront == 0 )
                {
                     mLeafPolyList.push_back( aPoly.GetID() );
                }
                return ( bSuccess1 && bSuccess2 );
            }
        }  // InsertPolyDontSplit
};

class BSPTree
{
    public :

	static Poly::PolyList smPolyList;

    typedef bool ( CALLBACK * pBSPWalkFunction )( BSPNode* );

    private :
        static bool CALLBACK DefaultWalkFunction( BSPNode* pCurrentNode );

        static pBSPWalkFunction smpBSPWalkFunction;

        BSPNode mRootBSPNode;
    public :
        BSPTree(){;}

        bool InsertAndSplitPoly( const Poly& aPoly )
        {
            return mRootBSPNode.InsertAndSplitPoly( aPoly );
        }

        bool InsertPolyDontSplit( const Poly& aPoly )
        {
            return mRootBSPNode.InsertPolyDontSplit( aPoly );
        }

        // Find which leaf a point lies in
        BSPNode* FindLeaf( const Vec3& aPoint, BSPNode* pStartNode = 0 )
        {
            // if no start node passed in, use the root node
            if ( pStartNode == 0 )
            {
                pStartNode = &mRootBSPNode;
            }
            // if no start node passed in, use the root node, so no need to check for null

            BSPNode* pNextNode = 0;

            while ( pStartNode != 0 )
            {
                BSPPlane::Classification where = pStartNode->ClassifyPoint( aPoint );

                if ( where == BSPPlane::Front ) 
                {
                    pNextNode = pStartNode->GetFront();
                }
                else
                if ( where == BSPPlane::Back )
                {
                    pNextNode = pStartNode->GetBack();
                }
                else
                {
                    assert( where == BSPPlane::Coplanar );
                }
                pStartNode = pNextNode;
            }

            // If I fell through, this is a leaf node
            return pStartNode;
        }


        bool WalkFrontToBack( const Vec3& aPoint, BSPNode* pStartNode = 0, pBSPWalkFunction pFunction = smpBSPWalkFunction)
        {
            // if no start node passed in, use the root node
            if ( pStartNode == 0 )
            {
                pStartNode = &mRootBSPNode;
            }

            BSPNode* pNextNode = 0;

            while ( pStartNode != 0 )
            {
                BSPPlane::Classification where = pStartNode->ClassifyPoint( aPoint );

                if ( where == BSPPlane::Front ) 
                {
                    if ( pStartNode->GetBack() != 0 )
                    {
                        // Recurse the back Tree,
                        WalkFrontToBack( aPoint, pStartNode->GetBack(), pFunction );
                    }
                    // Then do my leaf stuff
                    (*pFunction)( pStartNode );                   

                    if ( pStartNode->GetFront() != 0 )
                    {
                        // Recurse front tree, 
                        WalkFrontToBack( aPoint, pStartNode->GetFront(), pFunction );
                    }
                }
                else
                if ( ( where == BSPPlane::Back ) || ( where == BSPPlane::Coplanar ) )
                {
                    if ( pStartNode->GetFront() != 0 )
                    {
                        // Recurse the front Tree,
                        WalkFrontToBack( aPoint, pStartNode->GetFront(), pFunction );
                    }
                    // Then do my leaf stuff
                    (*pFunction)( pStartNode );                   

                    if ( pStartNode->GetBack() != 0 )
                    {
                        // Recurse back tree, 
                        WalkFrontToBack( aPoint, pStartNode->GetBack(), pFunction );
                    }
                }
            }
            // If I fell through, this is a leaf node
            return true;
        }

        bool WalkBackToFront( const Vec3& aPoint, BSPNode* pStartNode = 0, pBSPWalkFunction pFunction = smpBSPWalkFunction)
        {
            // if no start node passed in, use the root node
            if ( pStartNode == 0 )
            {
                pStartNode = &mRootBSPNode;
            }

            BSPNode* pNextNode = 0;

            while ( pStartNode != 0 )
            {
                BSPPlane::Classification where = pStartNode->ClassifyPoint( aPoint );

                if ( where == BSPPlane::Back ) 
                {
                    if ( pStartNode->GetBack() != 0 )
                    {
                        // Recurse the back Tree,
                        WalkBackToFront( aPoint, pStartNode->GetBack(), pFunction );
                    }
                    // Then do my leaf stuff
                    (*pFunction)( pStartNode );                   

                    if ( pStartNode->GetFront() != 0 )
                    {
                        // Recurse front tree, 
                        WalkBackToFront( aPoint, pStartNode->GetFront(), pFunction );
                    }
                }
                else
                if ( ( where == BSPPlane::Front ) || ( where == BSPPlane::Coplanar ) )
                {
                    if ( pStartNode->GetFront() != 0 )
                    {
                        // Recurse the front Tree,
                        WalkBackToFront( aPoint, pStartNode->GetFront(), pFunction );
                    }
                    // Then do my leaf stuff
                    (*pFunction)( pStartNode );                   

                    if ( pStartNode->GetBack() != 0 )
                    {
                        // Recurse back tree, 
                        WalkBackToFront( aPoint, pStartNode->GetBack(), pFunction );
                    }
                }
            }
            // If I fell through, this is a leaf node
            return true;
        }

};
#endif // _BSP_H_

