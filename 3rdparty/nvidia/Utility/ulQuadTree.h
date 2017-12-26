#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include <list>
//#include "BSphere.h"


// ID =  ( Parent << 2 ) | { LL,LR,UL,UR }

const unsigned int LL = 0x00;
const unsigned int UL = 0x01;
const unsigned int LR = 0x10;
const unsigned int UR = 0x11;
 
template < class T, class P > class QNode
{
    public :

        typedef  std::list< P > ItemList;

//        unsigned int  mID;
//        unsigned char mLevel;

/*        T             mLeft;
        T             mRight;
        T             mTop;
        T             mBottom;*/

        T             mSplitX;
        T             mSplitY;

        ItemList      mItemList;

/*        inline
        bool IsWithin( const T& x, const T& y ) const
        {
            return ( ( x >= mLeft )   && ( x < mRight ) &&
                     ( y >= mBottom ) && ( x < mTop ) );
                  
        }
*/
        inline
        unsigned int WithinChildEXEY( const T& x, const T& y ) const
        {
            if ( x >= mSplitX ) 
            {
                if ( y >= mSplitY )
                {
                    return ( ( mID << 2 ) | UR );
                }
                else
                {
                    return ( ( mID << 2 ) | LR );            
                }
            }
            else
            {
                if ( y >= mSplitY )
                {
                    return ( ( mID << 2 ) | UL );
                }
                else
                {
                    return ( ( mID << 2 ) | LL );            
                }
            }
            assert( false );
            return 0;
        }

        inline
        unsigned int WithinChildIXEY( const T& x, const T& y ) const
        {
            if ( x > mSplitX ) 
            {
                if ( y >= mSplitY )
                {
                    return ( ( mID << 2 ) | UR );
                }
                else
                {
                    return ( ( mID << 2 ) | LR );            
                }
            }
            else
            {
                if ( y >= mSplitY )
                {
                    return ( ( mID << 2 ) | UL );
                }
                else
                {
                    return ( ( mID << 2 ) | LL );            
                }
            }
            assert( false );
            return 0;
        }

        inline
        unsigned int WithinChildEXIY( const T& x, const T& y ) const
        {
            if ( x >= mSplitX ) 
            {
                if ( y > mSplitY )
                {
                    return ( ( mID << 2 ) | UR );
                }
                else
                {
                    return ( ( mID << 2 ) | LR );            
                }
            }
            else
            {
                if ( y > mSplitY )
                {
                    return ( ( mID << 2 ) | UL );
                }
                else
                {
                    return ( ( mID << 2 ) | LL );            
                }
            }
            assert( false );
            return 0;
        }

        inline
        unsigned int WithinChildIXIY( const T& x, const T& y ) const
        {
            if ( x > mSplitX ) 
            {
                if ( y > mSplitY )
                {
                    return ( ( mID << 2 ) | UR );
                }
                else
                {
                    return ( ( mID << 2 ) | LR );            
                }
            }
            else
            {
                if ( y > mSplitY )
                {
                    return ( ( mID << 2 ) | UL );
                }
                else
                {
                    return ( ( mID << 2 ) | LL );            
                }
            }
            assert( false );
            return 0;
        }


};


template < class T, class P > class QuadTree
{
    private :


        QNode< T, P >* mpQTree;
        unsigned char mLevels;

        unsigned int mSize;

        unsigned int mNodeCount;

    public :

        QuadTree( const unsigned int& theSize,
                  const unsigned char& theLevels)
        {
            mLevels = theLevels;
            mSize = theSize;

            mNodeCount = 1; // For 0x01 ( level 0 )

            unsigned int theLastLevelsCount = 1;

            for ( unsigned int theLevel = 0; theLevel < mLevels; ++theLevel )
            {
                theLastLevelsCount *= 4;
                mNodeCount += ( theLastLevelsCount );
            }


            mNodeCount += 1; // Handle the unused 0x00 node

            mpQTree = new QNode< T, P >[ mNodeCount ];

//            mpQTree[ 0 ].mID = 0x01;
//            mpQTree[ 0 ].mLevel = 0x0;
//            mpQTree[ 0 ].mLeft = 0;
//            mpQTree[ 0 ].mRight = theSize;
//            mpQTree[ 0 ].mTop = 0;
//            mpQTree[ 0 ].mBottom = theSize;*/

            unsigned int theID = 0x1;

            RecursiveCreate( theID, 0, 0, mSize, 0, mSize );

        }

        ~QuadTree()
        {
               delete []mpQTree, mpQTree = 0;
        }

        void RecursiveCreate( unsigned int theID, unsigned char theLevel, T minX, T maxX, T minY, T maxY )
        {
            assert( maxX > minX );
            assert( maxY > minY );

            mpQTree[ theID ].mSplitX = minX + ( maxX - minX ) / 2;
            mpQTree[ theID ].mSplitY = minY + ( maxY - minY ) / 2;


            if ( theLevel < ( mLevels - 1 ) )
            {
                RecursiveCreate(    ( theID << 2 ) | LL,
                                    theLevel + 1, 
                                    minX, 
                                    mpQTree[ theID ].mSplitX,
                                    minY,
                                     mpQTree[ theID ].mSplitY );

                RecursiveCreate(    ( theID << 2 ) | LR,
                                    theLevel + 1, 
                                    mpQTree[ theID ].mSplitX,
                                    maxX,
                                    minY,
                                    mpQTree[ theID ].mSplitY );

                RecursiveCreate(    ( theID << 2 ) | UR,
                                    theLevel + 1, 
                                    mpQTree[ theID ].mSplitX,
                                    maxX,
                                    mpQTree[ theID ].mSplitY,
                                    maxY );

                RecursiveCreate(    ( theID << 2 ) | UL,
                                    theLevel + 1, 
                                    minX, 
                                    mpQTree[ theID ].mSplitX,
                                    mpQTree[ theID ].mSplitY,
                                    maxY );
            }
        }

        unsigned int RecursiveInsert( const T& minX, const T& maxX,
                                      const T& minY, const T& maxY, 
                                      const P& anItem, const unsigned char& theLevel, unsigned int theID )
        {
            // if I have children
            if ( theLevel < ( mLevels - 1 ) )
            {
                //if 4 points have different children, I'm done, else recurse
                
                unsigned int theChildID1 = mpQTree[ theID ].WithinChildEXEY( minX, minY );
                unsigned int theChildID2 = mpQTree[ theID ].WithinChildIXEY( maxX, minY );
                unsigned int theChildID3 = mpQTree[ theID ].WithinChildIXIY( maxX, maxY );
                unsigned int theChildID4 = mpQTree[ theID ].WithinChildEXIY( minX, minY );

                if ( ( theChildID1 == theChildID2 ) &&( theChildID1 == theChildID3 ) &&
                     ( theChildID1 == theChildID4 ) )
                {
                   // Same child, time to recurse
                   return RecursiveInsert( minX, maxX, minY, maxY, anItem, theLevel + 1, theChildID1 );
                }
            }

            // If box was in different children, or
            //  I am a leaf node, insert and return

            // Goes in us
            mpQTree[ theID ].mItemList.insert( anItem );
            return theID;
        }


        inline
        unsigned int IterativeInsert( const T& minX, const T& maxX,
                                      const T& minY, const T& maxY, 
                                      const P& anItem, const unsigned char& theLevel, unsigned int theID )
        {
            for ( unsigned int theCurrentLevel = 0; theCurrentLevel < ( mLevels - 1 ); ++theCurrentLevel )
            {
                //if 4 points have different children, I'm done, else recurse
        
                unsigned int theChildID1 = mpQTree[ theID ].WithinChildEXEY( minX, minY );
                unsigned int theChildID2 = mpQTree[ theID ].WithinChildIXEY( maxX, minY );
                unsigned int theChildID3 = mpQTree[ theID ].WithinChildIXIY( maxX, maxY );
                unsigned int theChildID4 = mpQTree[ theID ].WithinChildEXIY( minX, maxY );

                if ( ( theChildID1 == theChildID2 ) &&( theChildID1 == theChildID3 ) &&
                     ( theChildID1 == theChildID4 ) )
                {
                    // Same child, time to iterate
                    theID = theChildID1;
                    continue;
                }
                else
                {
                    break;
                }
            }
        }

        inline
        unsigned int IterativeDelete( const T& minX, const T& maxX,
                                      const T& minY, const T& maxY, 
                                      const P& anItem, const unsigned char& theLevel, unsigned int theID )
        {
            for ( unsigned int theCurrentLevel = 0; theCurrentLevel < ( mLevels - 1 ); ++theCurrentLevel )
            {
                //if 4 points have different children, I'm done, else recurse
        
                unsigned int theChildID1 = mpQTree[ theID ].WithinChildEXEY( minX, minY );
                unsigned int theChildID2 = mpQTree[ theID ].WithinChildIXEY( maxX, minY );
                unsigned int theChildID3 = mpQTree[ theID ].WithinChildIXIY( maxX, maxY );
                unsigned int theChildID4 = mpQTree[ theID ].WithinChildEXIY( minX, maxY );

                if ( ( theChildID1 == theChildID2 ) &&( theChildID1 == theChildID3 ) &&
                     ( theChildID1 == theChildID4 ) )
                {
                    // Same child, time to iterate
                    theID = theChildID1;
                    continue;
                }
                else
                {
                    break;
                }
            }

            // If box was in different children, or
            //  I am a leaf node, it should be here...

            for ( ItemList::iterator iter = mpQtree[ theID ].mItemList.begin();
                  iter != mpQtree[ theID ].mItemList.end();
                  ++iter )
            {
                if ( (*iter) == anItem )
                {
                    mItemList.erase( iter );
                    return theID;
                }
            }
            assert( false );
            return 0;
        }

        bool Delete( const unsigned int& theID, const P& theItem )
        {
            QNode< T, P >* pNode = GetNode( theID );
            if ( pNode )
            {
                for ( ItemList::iterator iter = pNode->mItemList.begin();
                      iter != pNode->mItemList.end();
                      ++iter )
                {
                    if ( (*iter) == anItem )
                    {
                        mItemList.erase( iter );
                        return true;
                    }
                }
            }
            assert( false );
            return 0;
        }

        const QNode< T, P >* const GetNode( const unsigned int& theID ) const
        {
            if ( theID < mNodeCount )
            {
                return &( mpQTree[ theID ] );
            }
            assert( false );
            return 0;
        }

};




#endif _QUADTREE_H_


