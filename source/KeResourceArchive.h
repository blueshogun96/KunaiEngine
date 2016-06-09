//
//  KeResourceArchive.h
//
//  Created by Shogun3D on 12/3/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#pragma once

#include "Ke.h"
#include "KeToolKit.h"


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
	virtual bool	ReadString( std::string filename, void** ptr, size_t* size ) PURE;
    virtual bool    ReadTexture( std::string filename, uint32_t desired_target, IKeTexture** texture ) PURE;
    virtual bool    ReadSoundBuffer( std::string filename, IKeSoundBuffer** soundbuffer ) PURE;

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
    virtual bool    ReadTexture( std::string filename, uint32_t desired_target, IKeTexture** texture );
    virtual bool    ReadSoundBuffer( std::string filename, IKeSoundBuffer** soundbuffer );
    
protected:
	void*	archive;
};

/*
 * Feel free to add more!
 */