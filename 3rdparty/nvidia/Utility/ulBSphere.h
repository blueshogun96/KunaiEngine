/*********************************************************************NVMH2****
Path:  c:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulBSphere.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/

#ifndef _BSPHERE_H_
#define _BSPHERE_H_

#ifdef _MSC_VER_
#pragma pack( push, 1 )
#endif

//#include <ul.h>
#include "ulCommon.h"
#include "ulVec4.h"


class BoundingSphere
{
    public :

    Vec4 mLocation;
    Float32 mRadius;

    BoundingSphere(){;}

    BoundingSphere( const BoundingSphere& aSphere ) 
    {
        mLocation = aSphere.mLocation;
        mRadius = aSphere.mRadius;
    }

    BoundingSphere& operator=( const BoundingSphere& aSphere ) 
    {
        mLocation = aSphere.mLocation;
        mRadius = aSphere.mRadius;
        return *this;
    }

    bool Interpenetrates( const Vec4& aPoint )
    {
        Vec4 aVector( aPoint );
        aVector -= mLocation;
        if ( aVector.GetMagnitudeSquared() < ( mRadius * mRadius ) )
        {
            return true;
        }
        return false;
    }
    bool InterpenetratesOrTouches( const Vec4& aPoint )
    {
        Vec4 aVector( aPoint );
        aVector -= mLocation;
        if ( aVector.GetMagnitudeSquared() <= ( mRadius * mRadius ) )
        {
            return true;
        }
        return false;
    }

    // NOTE Doesn't count touching
    bool Interpenetrates( const BoundingSphere& aSphere )
    {
        Vec4 aVector( aSphere.mLocation );
        aVector -= mLocation;
        Float32 aRadius = mRadius += aSphere.mRadius;

        if ( aVector.GetMagnitudeSquared() < ( aRadius * aRadius ) )
        {
            return true;
        }
        return false;
    }
    bool InterpenetratesOrTouches( const BoundingSphere& aSphere )
    {
        Vec4 aVector( aSphere.mLocation );
        aVector -= mLocation;
        Float32 aRadius = mRadius += aSphere.mRadius;

        if ( aVector.GetMagnitudeSquared() <= ( aRadius * aRadius ) )
        {
            return true;
        }
        return false;
    }

    bool IsWithinPositiveHalfSpace( const Vec4& aPlane, const Vec4& aPointOnPlane, float& aResult )
    {
        Vec4 aVector( mLocation );
        aVector -= aPointOnPlane;
        aResult = aVector DOT aPlane;

        return ( aResult >= mRadius );
    }

    bool IsOutside( const Vec4& aNormal, const Vec4& aPointOnPlane, float& aResult )
    {
        Vec4 aVector( mLocation );
        aVector -= aPointOnPlane;
        aResult = aVector DOT aNormal;

        return ( aResult < -mRadius );
    }

    bool Interpenetrates( const Vec4& aPlane, const Vec4& aPointOnPlane )
    {
        Vec4 aVector( mLocation );
        aVector -= aPointOnPlane;
        if ( fabs( aVector DOT aPlane ) < mRadius )
        {
            return true;
        }
        return false;
    }
    bool InterpenetratesOrTouches( const Vec4& aPlane, const Vec4& aPointOnPlane )
    {
        Vec4 aVector( mLocation );
        aVector -= aPointOnPlane;
        if ( fabs( aVector DOT aPlane ) <= mRadius )
        {
            return true;
        }
        return false;
    }

};

#endif
