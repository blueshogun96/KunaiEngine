/*********************************************************************NVMH2****
Path:  c:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulBCylinder.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/

#ifndef _BCYLINDER_H_
#define _BCYLINDER_H_

#ifdef _MSC_VER_
#pragma pack( push, 1 )
#endif

//#include <ul.h>
#include "ulCommon.h"
#include "ulVec4.h"

class BoundingCylinder
{
    public :

    Vec4 mBaseLocation;
    Float32 mHeight;
    Float32 mRadius;

    BoundingCylinder(){;}

    BoundingCylinder( const BoundingCylinder& rhs ) 
    {
        mBaseLocation = rhs.mBaseLocation;
        mHeight = rhs.mHeight;
        mRadius = rhs.mRadius;
    }

    BoundingCylinder& operator=( const BoundingCylinder& rhs ) 
    {
        mBaseLocation = rhs.mBaseLocation;
        mHeight = rhs.mHeight;
        mRadius = rhs.mRadius;
        return *this;
    }

    void SetBaseLocation( Vec4& aLocation )
    {
        mBaseLocation = aLocation;
    }

    bool Interpenetrates( const Vec4& aPoint )
    {
        if ( ( aPoint.y - mBaseLocation.y ) > mRadius ) return false;
        Vec4 aVector( aPoint );

        aVector.x -=  mBaseLocation.x;
        aVector.y = 0.0f;
        aVector.z -=  mBaseLocation.z;

        if ( aVector.GetMagnitudeSquared() < ( mRadius * mRadius ) )
        {
            return true;
        }
        return false;
    }
    bool InterpenetratesOrTouches( const Vec4& aPoint )
    {
        if ( ( aPoint.y - mBaseLocation.y ) >= mRadius ) return false;
        Vec4 aVector( aPoint );

        aVector.x -=  mBaseLocation.x;
        aVector.y = 0.0f;
        aVector.z -=  mBaseLocation.z;

        if ( aVector.GetMagnitudeSquared() < ( mRadius * mRadius ) )
        {
            return true;
        }
        return false;
    }

    // NOTE Doesn't count touching
    bool Interpenetrates( const BoundingCylinder& aSphere )
    {
        Vec4 aVector( aSphere.mBaseLocation );
        aVector -= mBaseLocation ;
        Float32 aRadius = mRadius += aSphere.mRadius;

        if ( aVector.GetMagnitudeSquared() < ( aRadius * aRadius ) )
        {
            return true;
        }
        return false;
    }
    bool InterpenetratesOrTouches( const BoundingCylinder& aSphere )
    {
        Vec4 aVector( aSphere.mBaseLocation  );
        aVector -= mBaseLocation ;
        Float32 aRadius = mRadius += aSphere.mRadius;

        if ( aVector.GetMagnitudeSquared() <= ( aRadius * aRadius ) )
        {
            return true;
        }
        return false;
    }

    bool IsWithinPositiveHalfSpace( const Vec4& aPlane, const Vec4& aPointOnPlane, float& aResult )
    {
        Vec4 aVector( mBaseLocation );
        aVector -= aPointOnPlane;
        aResult = aVector DOT aPlane;

        return ( aResult >= mRadius );
    }

    bool IsOutside( const Vec4& aNormal, const Vec4& aPointOnPlane, float& aResult )
    {
        Vec4 aVector( mBaseLocation );
        aVector -= aPointOnPlane;
        aResult = aVector DOT aNormal;

        return ( aResult < -mRadius );
    }

    bool Interpenetrates( const Vec4& aPlane, const Vec4& aPointOnPlane )
    {
        Vec4 aVector( mBaseLocation );
        aVector -= aPointOnPlane;
        if ( fabs( aVector DOT aPlane ) < mRadius )
        {
            return true;
        }
        return false;
    }
    bool InterpenetratesOrTouches( const Vec4& aPlane, const Vec4& aPointOnPlane )
    {
        Vec4 aVector( mBaseLocation );
        aVector -= aPointOnPlane;
        if ( fabs( aVector DOT aPlane ) <= mRadius )
        {
            return true;
        }
        return false;
    }

};

#endif  _BCYLINDER_H_
