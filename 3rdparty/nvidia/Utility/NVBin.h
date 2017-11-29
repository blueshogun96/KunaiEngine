#include <assert.h>

#pragma pack( push, 1 )

// NvBin - like a stl vector ( growable array ) except for two things :

//	You can specify both the growth factor >=2 and the initial size via optional template parameters
//	You can clear it w/out deleting the objects by calling clear()
//	You can actually delete the objects by calling kill() or deleting the bin

//	Example :

//	NVBin< float, 3, 1024 > myFloatBin; // This creates a bin of floats, with growth factor of 3 ( it will triple in size if it runs out of room ) and initial size 1024

//	myFloatBin.push_back( 3.0f );	// add an element

//	myFloatBin.clear();	// don't call ~float, just pretend its empty again

//	myFloatBin.kill();	// really de-allocate


template < class T, int theGrowthFactor = 2, int theNumberOfSlots = 2048 >
class NVBin
{
	private :
	
		T* mpTs;

		unsigned int	mSlotsUsed;
		unsigned int	mSlotsAllocated;
		unsigned int    mGrowthFactor;

		NVBin& operator=( const NVBin& rhs ); // Intentionally Unimplemented
		NVBin( const NVBin& rhs ); // Intentionally Unimplemented
	public :

		typedef const T* const_iterator;
		typedef T* iterator;

		const_iterator begin() const { return &( mpT[ 0 ] ); }
		      iterator begin() { return &( mpT[ 0 ] ); }

		const_iterator end() const { return &( mpT[ mSlotsUsed ] ); }
		      iterator end() { return &( mpT[ mSlotsUsed ] ); }
		NVBin()
		{
			mGrowthFactor = max( 2, theGrowthFactor );
			mSlotsAllocated = max( 1, theNumberOfSlots );
			mpTs = new T[ mSlotsAllocated ];
			mSlotsUsed = 0;
			assert( theNumberOfSlots >= 1 );
			assert( mGrowthFactor >= 2 );
		}
		unsigned int size() const { return mSlotsUsed; }

		void kill()
		{
			nSlotsUsed = 0;
			mSlotsAllocated = 0;
			delete []mpTs, mpTs = 0;
		}

		void clear() 
		{
			mSlotsUsed = 0;
		}

		unsigned int push_back( const T& aNode )
		{
			if ( mSlotsUsed == mSlotsAllocated )
			{
				mSlotsAllocated *= mGrowthFactor;
				T* pNewTs = new T[ mSlotsAllocated ];

				for ( unsigned int i = 0; i < mSlotsUsed; ++i )
				{
					pNewTs[ i ] = mpTs[ i ];
				}
				delete []mpTs, mpTs = pNewTs;
			}
			mpTs[ mSlotsUsed ] = aNode;
			++mSlotsUsed;
                        return ( mSlotsUsed - 1 );
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
		virtual ~NVBin()
		{
			kill();
		}
	

};
