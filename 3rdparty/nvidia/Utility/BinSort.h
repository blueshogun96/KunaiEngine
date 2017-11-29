#ifndef _BINSORT_H_
#define _BINSORT_H_

// Sorter to assemble index lists by ID
//  useful for material binning

#include <assert.h>

#pragma pack( push, 1 )

template < class T, int theGrowthFactor = 2, int theNumberOfSlots = 2048 >
class Bin
{
	private :
	
		T* mpTs;

		unsigned int	mSlotsUsed;
		unsigned int	mSlotsAllocated;


		Bin& operator=( const Bin& rhs ); // Intentionally Unimplemented
		Bin( const Bin& rhs ); // Intentionally Unimplemented
	public :

		Bin()
		{
			mSlotsAllocated = theNumberOfSlots;
			mpTs = new T[ mSlotsAllocated ];
			mSlotsUsed = 0;
			assert( theGrowthFactor >= 2 );
		}
		unsigned int size() const { return mSlotsUsed; }

		void clear() 
		{
			mSlotsUsed = 0;
		}

		void AddNode( const T& aNode )
		{
			if ( mSlotsUsed == mSlotsAllocated )
			{
				mSlotsAllocated *= theGrowthFactor;
				T* pNewTs = new T[ mSlotsAllocated ];

				for ( int i = 0; i < mSlotsUsed; ++i )
				{
					pNewTs[ i ] = mpTs[ i ];
				}
				delete []mpTs, mpTs = pNewTs;
			}
			mpTs[ mSlotsUsed ] = aNode;
			++mSlotsUsed;
		}
		T& operator[]( const unsigned int& i )
		{
			assert( i < mSlotsUsed );
			return mpTs[ i ];
		}

		const T& operator[]( const unsigned int& i ) const
		{
			assert( i < mSlotsUsed );
			return mpTs[ i ];
		}
		virtual ~Bin()
		{
			delete []mpTs, mpTs = 0;
		}
	

};


template < class T, int theNumberOfBins = 96, int theGrowthFactor = 2, int theNumberOfSlots = 2048 >
class BinSorter
{
	private :

		Bin< T, theGrowthFactor, theNumberOfSlots > mBins[ theNumberOfBins ];

		BinSorter& operator=( const BinSorter& rhs ); // Intentionally Unimplemented
		BinSorter( const BinSorter& rhs );			// Intentionally Unimplemented
	public :
		void AddTri( unsigned int& anID, const T& t0, const T& t1, const T& t2 )
		{
			unsigned int theID = anID % theNumberOfBins;

			mBins[ theID ].AddNode( t0 );
			mBins[ theID ].AddNode( t1 );
			mBins[ theID ].AddNode( t2 );
		}
		void AddVertex( unsigned int& anID, const T& t0 )
		{
			unsigned int theID = anID % theNumberOfBins;
			mBins[ theID ].AddNode( t0 );
		}

		void clear()
		{
			for ( unsigned int i = 0; i < theNumberOfBins; ++i )
			{
				mBins[ 0 ].clear();
			}
		}
		unsigned int GetSlotCount() const { return theNumberOfBins; }

		const Bin< T, theGrowthFactor, theNumberOfSlots >& operator[]( const unsigned int& i ) const 
		{
			assert( i < theNumberOfBins );
			return mBins[ i ];
		}
		Bin< T, theGrowthFactor, theNumberOfSlots >& operator[]( const unsigned int& i ) 
		{
			assert( i < theNumberOfBins );
			return mBins[ i ];
		}

		const Bin< T, theGrowthFactor, theNumberOfSlots >& GetBin( const unsigned int& i ) const 
		{
			assert( i < theNumberOfBins );
			return mBins[ i ];
		}
		Bin< T, theGrowthFactor, theNumberOfSlots >& GetBin( const unsigned int& i ) 
		{
			assert( i < theNumberOfBins );
			return mBins[ i ];
		}

};


#pragma pack( pop )

#endif  _BINSORT_H_
