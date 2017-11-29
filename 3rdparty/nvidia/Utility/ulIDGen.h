#ifndef _IDGEN_HPP_
#define _IDGEN_HPP_

#include "assert.h"

typedef unsigned int ID;

class IDGen
{
public:
    IDGen::IDGen( ID startID = 1 ) : mNextID( startID ), mStartID( startID )
	{
		assert( startID > 0 );
	}

    virtual IDGen::~IDGen(){;}    // abstract class
    
    virtual ID GenerateID( void ) = 0;
	// generate an ID AFTER the one passed in
    virtual ID GenerateIDAfter( const ID& anID ) = 0;
	bool	IsValid( const ID& anID )
	{
		return ( ( anID > 0 ) && ( anID < mStartID ) );
	}
    virtual void Reset( void ) = 0;
protected:
	ID mStartID;
    ID mNextID;
};

class SequentialIDGen : public IDGen
{

public:
    SequentialIDGen::SequentialIDGen( ID startID = 1 ) : IDGen( startID ){;}
    virtual SequentialIDGen::~SequentialIDGen(){;}

    virtual ID GenerateID( void )
    {
        return mNextID++;
    }
    virtual ID GenerateIDAfter( const ID& anID )
    {
        mNextID = anID + 2;
        return ( mNextID - 1 );
    }

    virtual void Reset( void )
    {
        mNextID = mStartID;
    }   
};

/*
class BinaryIDGen
{
public:
    BinaryIDGen::BinaryIDGen( ID startID = 1 )
    {
        mCount = 0;
        mPowerOf2 = 0;
    }

    virtual BinaryIDGen::~BinaryIDGen(){;}

    virtual ID GenerateID( void )
    {
        ++mCount;

        ID anID = mNextID;
//        mNextID = 
        return anID;
    }
    virtual ID GenerateIDAfter( const ID& anID )
    {
        mNextID = anID + 2;
        return mNextID - 1;
    }

    virtual void Reset( void )
    {
        mNextID = 0;
    } 
private :
    unsigned int mCount;
    unsigned int mPowerOf2;
};*/

#endif