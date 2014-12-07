//
//  ke_resource_archive.h
//
//  Created by Shogun3D on 12/3/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#pragma once

#include "ke.h"


/*
 * Resource archive base class
 */
class ke_resource_archive_t
{
public:
	ke_resource_archive_t() {};
	virtual ~ke_resource_archive_t() {};

public:
	virtual bool	is_open() PURE;
	virtual void	close() PURE;
	virtual bool	read( std::string filename, void** ptr, size_t* size ) PURE;
	virtual bool	read( char* filename, void** ptr, size_t* size ) PURE;

protected:
	bool	opened;
};


/*
 * Zip file resource archive
 */
class ke_zip_resource_archive_t : public ke_resource_archive_t
{
public:
	ke_zip_resource_archive_t();
	ke_zip_resource_archive_t( std::string filename );
	ke_zip_resource_archive_t( char* filename );
	virtual ~ke_zip_resource_archive_t();

public:
	virtual bool	is_open();
	virtual void	close();
	virtual bool	read( std::string filename, void** ptr, size_t* size );
	virtual bool	read( char* filename, void** ptr, size_t* size );

protected:
	void*	archive;
};

/*
 * Feel free to add more!
 */