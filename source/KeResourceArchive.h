//
//  KeResourceArchive.h
//
//  Created by Shogun3D on 12/3/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#pragma once

#include "Ke.h"


/*
 * Resource archive base class
 */
class KeResourceArchive
{
public:
	KeResourceArchive() {};
	virtual ~KeResourceArchive() {};

public:
	virtual bool	IsOpen() PURE;
	virtual void	Close() PURE;
	virtual bool	Read( std::string filename, void** ptr, size_t* size ) PURE;

protected:
	bool	opened;
};


/*
 * Zip file resource archive
 */
class KeZipResourceArchive : public KeResourceArchive
{
public:
	KeZipResourceArchive();
	KeZipResourceArchive( std::string filename );
    virtual ~KeZipResourceArchive();

public:
	virtual bool	IsOpen();
	virtual void	Close();
	virtual bool	Read( std::string filename, void** ptr, size_t* size );
	virtual bool	ReadString( std::string filename, void** ptr, size_t* size );
    
protected:
	void*	archive;
};

/*
 * Feel free to add more!
 */