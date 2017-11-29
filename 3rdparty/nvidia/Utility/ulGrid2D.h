#ifndef _GRID2D_H_
#define _GRID2D_H_

// GRID2D.H
// This header file desribes a grid data structure
// It represents a series of regularly spaced half-open rectangular intervals
// the width and height dimensions are independent
// The grid can perform a constant time lookup of one or more
// grid areas based on points, lines or rectangular queries
// More complicated queries require knowledge of a particular
// polygonal data structure, and can be performed in terms of
// line or rectangle queries.

#include <ul.h>

#define SUBGRID_RESOLUTION 256.0f
#define SUBGRID_MASK 0xFF
#define SUBGRID_SHIFT 8

template< class Element > class Grid2D
{
    private :

        int mWidth;
        int mHeight;
        int mUnitWidth;
        int mUnitHeight;

        float mInvWidth;
        float mInvHeight;

        Element* mpGrid;

        int SnapToGridWidth( const float& aFloat ) const
        {
            // The grid consists of half-open intervals,
            int i;
            ulFloatToInt( &i, aFloat * invWidth );

            // if this does not fall on the boundary, return as normal
            if ( i & SUBGRID_MASK )
            {
                return i >> SUBGRID_SHIFT;
            }
            else
            {
                return ( i >> SUBGRID_SHIFT ) - 1;
            }
        }

        int SnapToGridHeight( const float& aFloat ) const
        {
            // The grid consists of half-open intervals,
            int i;
            ulFloatToInt( &i, aFloat * invHeight );

            // if this does not fall on the boundary, return as normal
            if ( i & SUBGRID_MASK )
            {
                return i >> 8;
            }
            else
            {
                return ( i >> 8 ) - 1;
            }
        }

    public :
        Grid2D( const unsigned int& width, const unsigned int& height, const unsigned int& unitWidth, const unsigned int& unitHeight )
        {
            mWidth = width;
            mHeight = height;
            mUnitWidth = unitWidth;
            mUnitHeight = unitHeight;

            mInvWidth = SUBGRID_RESOLUTION / mUnitWidth;
            mInvHeight = SUBGRID_RESOLUTION / mUnitHeight;

            mpGrid = new Element[ width * height ];
        }

        virtual ~Grid2D()
        {
            SAFE_ARRAY_DELETE(mpGrid);
          
        }

        Element* LookUp( const float& x, const float& y ) const
        {
            int myx = SnapToGridWidth( x );
            int myy = SnapToGridHeight( y );

            if ( ( myx < mWidth ) && ( myy < mHeight ) )
            {
                return &( mpGrid[ mWidth * myy + myx ] );
            }
            return 0;
        }

        std::list< Element > LookUp( const float& x, const float& y, const float& x2, const float& y2 ) const
        {
            std::list< Element > aList;

            int myx = SnapToGridWidth( x );
            int myy = SnapToGridWidth( y );

            int myx2 = SnapToGridHeight( x2 );
            int myy2 = SnapToGridHeight( y2 );

            assert( myx != myx2 );
            assert( myy != myy2 );

            for ( unsigned int j = myy; j < myy2; ++j )
            {
                for ( unsigned int i = myx; i < myx2; ++i )
                {
                    aList.insert( mpGrid[ mWidth * j + i ] );
                }
            }
            return aList;
        }

};

#endif  _GRID2D_H_
