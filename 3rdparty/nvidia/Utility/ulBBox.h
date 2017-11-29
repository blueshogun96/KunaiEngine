/*********************************************************************NVMH1****
File:
ulBBox.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/


#ifndef _BBOX_H_
#define _BBOX_H_

#ifdef _MSC_VER_
#pragma pack( push, 1 )
#endif

#include <ul.h>
#include "float.h"

#include <vector>
// #include <d3d.h>

const unsigned int bbNear = 0;
const unsigned int bbLeft = 1;
const unsigned int bbRight = 2;
const unsigned int bbBottom = 3;
const unsigned int bbFar = 4;
const unsigned int bbTop = 5;

enum ClipResult
{
	crFullyInside = 0,
	crFullyOutside = 1,
	crPartiallyClipped = 2
};

const unsigned int BBOX_HIGH = 0;
const unsigned int BBOX_LOW = 6;

class BoundingBox
{
    public :

    std::vector< Vec4 > mPlanes;
    std::vector< Vec3 > mPoints;

    BoundingBox( const BoundingBox& aBox )
    {
        *this = aBox;
    }
    BoundingBox& operator=( const BoundingBox& aBox )
    {
        if ( this != &aBox )
        {
            mPlanes = aBox.mPlanes;
            mPoints = aBox.mPoints;
        }
        return *this;
    }
    BoundingBox()
    {
        mPlanes.reserve( 6 );
        mPlanes.push_back( Vec4(0.0f, 0.0f, 0.0f, 0.0f ) );
        mPlanes.push_back( Vec4(0.0f, 0.0f, 0.0f, 0.0f ) );
        mPlanes.push_back( Vec4(0.0f, 0.0f, 0.0f, 0.0f ) );
        mPlanes.push_back( Vec4(0.0f, 0.0f, 0.0f, 0.0f ) );
        mPlanes.push_back( Vec4(0.0f, 0.0f, 0.0f, 0.0f ) );
        mPlanes.push_back( Vec4(0.0f, 0.0f, 0.0f, 0.0f ) );

        mPoints.reserve( 8 );
        mPoints.push_back( Vec3( 0.0f, 0.0f, 0.0f ) );
        mPoints.push_back( Vec3( 0.0f, 0.0f, 0.0f ) );
        mPoints.push_back( Vec3( 0.0f, 0.0f, 0.0f ) );
        mPoints.push_back( Vec3( 0.0f, 0.0f, 0.0f ) );
        mPoints.push_back( Vec3( 0.0f, 0.0f, 0.0f ) );
        mPoints.push_back( Vec3( 0.0f, 0.0f, 0.0f ) );
        mPoints.push_back( Vec3( 0.0f, 0.0f, 0.0f ) );
        mPoints.push_back( Vec3( 0.0f, 0.0f, 0.0f ) );

    }
    BoundingBox( std::vector< Vec4 >& sixPlanes )
    {
        mPlanes = sixPlanes;
    }

    BoundingBox& operator+= ( const BoundingBox& aBox )
    {
        std::vector< Vec3 > thePoints( mPoints );
        thePoints.insert( thePoints.end() ,aBox.mPoints.begin(), aBox.mPoints.end() );
        ComputeAxisAligned( thePoints );
        return ( *this );
    }

    BoundingBox& operator+= ( const std::vector< Vec3 > inPoints )
    {
        std::vector< Vec3 > thePoints( mPoints );
        thePoints.insert( thePoints.end() ,inPoints.begin(), inPoints.end() );
        ComputeAxisAligned( thePoints );
        return ( *this );
    }

//gjuch
//    void ComputeAxisAligned( const std::vector< D3DVERTEX >& aVertList )
    void ComputeAxisAligned( const std::vector< Vec3 >& aVertList )
    {
        Vec3 low( FLT_MAX, FLT_MAX, FLT_MAX );
        Vec3 high( -FLT_MAX, -FLT_MAX, -FLT_MAX );
        
        for ( unsigned int m = 0; m < aVertList.size(); ++m )
        {
            low.x =  min( low.x,  aVertList[ m ].x );
            high.x = max( high.x, aVertList[ m ].x );
            low.y =  min( low.y,  aVertList[ m ].y );
            high.y = max( high.y, aVertList[ m ].y );
            low.z =  min( low.z,  aVertList[ m ].z );
            high.z = max( high.z, aVertList[ m ].z );
        }

        mPlanes[ bbNear ].set(   0.0f, 0.0f, 1.0f, low.z );
        mPlanes[ bbLeft ].set(   1.0f, 0.0f, 0.0f, low.x );
        mPlanes[ bbRight ].set( -1.0f, 0.0f, 0.0f, high.x );
        mPlanes[ bbBottom ].set( 0.0f, 1.0f, 0.0f, low.y );
        mPlanes[ bbFar ].set(    0.0f, 0.0f,-1.0f, high.z );
        mPlanes[ bbTop ].set(    0.0f,-1.0f, 0.0f, high.y );

        mPoints[ 0 ].set( high.x, high.y, high.z );
        mPoints[ 1 ].set( high.x, high.y, low.z );

        mPoints[ 2 ].set( low.x,  high.y, low.z );
        mPoints[ 3 ].set( low.x,  high.y, high.z );

        mPoints[ 4 ].set( high.x,  low.y, high.z );
        mPoints[ 5 ].set( high.x,  low.y, low.z );

        mPoints[ 6 ].set( low.x,   low.y, low.z );
        mPoints[ 7 ].set( low.x,   low.y, high.z );
   }
    

    void ComputeAxisAligned( const std::vector< Vec3 >& aList )
    {
        Vec3 low( FLT_MAX, FLT_MAX, FLT_MAX );
        Vec3 high( -FLT_MAX, -FLT_MAX, -FLT_MAX );
        
        for ( unsigned int m = 0; m < aList.size(); ++m )
        {
            low.x =  min( low.x,  aList[ m ].x );
            high.x = max( high.x, aList[ m ].x );
            low.y =  min( low.y,  aList[ m ].y );
            high.y = max( high.y, aList[ m ].y );
            low.z =  min( low.z,  aList[ m ].z );
            high.z = max( high.z, aList[ m ].z );
        }
        SetExtents( low.x, high.x, low.y, high.y, low.z, high.z );
    }

    void SetExtents( real low_x, real high_x, real low_y, real high_y, real low_z, real high_z )
    {
        mPlanes[ bbNear ].set(   0.0f, 0.0f, 1.0f, low_z );
        mPlanes[ bbLeft ].set(   1.0f, 0.0f, 0.0f, low_x );
        mPlanes[ bbRight ].set( -1.0f, 0.0f, 0.0f, high_x );
        mPlanes[ bbBottom ].set( 0.0f, 1.0f, 0.0f, low_y );
        mPlanes[ bbFar ].set(    0.0f, 0.0f,-1.0f, high_z );
        mPlanes[ bbTop ].set(    0.0f,-1.0f, 0.0f, high_y );

        mPoints[ 0 ].set( high_x, high_y, high_z );
        mPoints[ 1 ].set( high_x, high_y, low_z );

        mPoints[ 2 ].set( low_x,  high_y, low_z );
        mPoints[ 3 ].set( low_x,  high_y, high_z );

        mPoints[ 4 ].set( high_x,  low_y, high_z );
        mPoints[ 5 ].set( high_x,  low_y, low_z );

        mPoints[ 6 ].set( low_x,   low_y, low_z );
        mPoints[ 7 ].set( low_x,   low_y, high_z );
    }
    
    void GetExtents( real& x, real& x2, real& y, real& y2, real& z, real& z2 )
    {
        x  = mPoints[ BBOX_LOW ].x;
        x2 = mPoints[ BBOX_HIGH ].x;

        y  = mPoints[ BBOX_LOW ].y;
        y2 = mPoints[ BBOX_HIGH ].y;

        z  = mPoints[ BBOX_LOW ].z;
        z2 = mPoints[ BBOX_HIGH ].z;
    }

    bool IsWithinEarlyOut( const Vec4& aVector ) const
    {
        for ( unsigned int i = 0; i < 6; ++i )
        {
            if ( ( mPlanes[ i ] DOT aVector ) >= -mPlanes[ i ].mDistance )
                return false;
        }
        return true;
    }

    bool IsWithinKeepResults( const Vec4& aVector, real* theResults ) const
    {
        bool isWithin = TRUE;
        for ( unsigned int i = 0; i < 6; ++i )
        {
            theResults[ i ] = ( mPlanes[ i ] DOT aVector );
            isWithin &= ( theResults[ i ] >= 0.0f );
        }
        return isWithin;
    }


    // this is a Trival accept or Reject test for an Oriented ( or AA ) bounding box
	// Doesn't take into account this box inside aBox
    ClipResult IsWithinEarlyOut( const BoundingBox& aBox ) const
    {
        // NOTE: Important that packing be set to 1
        unsigned char aPointArray[ 8 ];

        for ( unsigned int j = 0; j < 8; ++j )
        {
            aPointArray[ j ] = 0;
        }

        for ( unsigned int i = 0; i < 6; ++i )
        {
            for ( unsigned int k = 0; k < 8; ++k )
            {
                aPointArray[ k ] |= ( ( ( aBox.mPlanes[ i ].mNormal DOT mPoints[ k ] ) >= -aBox.mPlanes[ i ].w ) << i );
            }
        }

        // if no point pairs disagree, then we are either fully in or outside the Box
        unsigned int* aQuickArray = (unsigned int*)aPointArray;

        // do box tests in parallel
        unsigned int aResult = ( aQuickArray[ 0 ] & aQuickArray[ 1 ] );

        // only if all points agree are we fully inside
        if ( aResult == ( ( 63 << 24) | ( 63 << 16 ) | ( 63 << 8 ) | 63 ) )
        {
            return crFullyInside;
        }

        if ( aResult == 0 )
            return crFullyOutside;

        return crPartiallyClipped;
    }

};

#ifdef _MSC_VER_
#pragma pack( pop )
#endif

#endif
