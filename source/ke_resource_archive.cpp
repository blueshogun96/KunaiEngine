//
//  ke_resource_archive.cpp
//
//  Created by Shogun3D on 12/3/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

//#define MINIZ_HEADER_INCLUDED

#include "ke_resource_archive.h"
#include "miniz.c"



ke_zip_resource_archive_t::ke_zip_resource_archive_t( ) : archive(nullptr)
{
	opened = No;
};

ke_zip_resource_archive_t::ke_zip_resource_archive_t( std::string filename ) : archive(nullptr)
{
	archive = new mz_zip_archive;

	/* Open the desired archive */
	ZeroMemory( archive, sizeof( mz_zip_archive ) );
	opened = (bool) mz_zip_reader_init_file( (mz_zip_archive*) archive, filename.c_str(), 0 );
};

ke_zip_resource_archive_t::ke_zip_resource_archive_t( char* filename )
{
	archive = new mz_zip_archive;

	/* Open the desired archive */
	ZeroMemory( archive, sizeof( mz_zip_archive ) );
	opened = (bool) mz_zip_reader_init_file( (mz_zip_archive*) archive, filename, 0 );
};

ke_zip_resource_archive_t::~ke_zip_resource_archive_t()
{
	close();
}

bool ke_zip_resource_archive_t::is_open()
{
	return opened;
}

void ke_zip_resource_archive_t::close()
{
	/* Close the fastfile, if it is open. */
	mz_zip_reader_end( (mz_zip_archive*) archive );
}

bool ke_zip_resource_archive_t::read( std::string filename, void** ptr, size_t* size )
{
	*ptr = mz_zip_reader_extract_file_to_heap( (mz_zip_archive*) archive, filename.c_str(), size, 0 );
	if( !(*ptr ) )
		return false;
    
	return true;
}

bool ke_zip_resource_archive_t::read( char* filename, void** ptr, size_t* size )
{
	*ptr = mz_zip_reader_extract_file_to_heap( (mz_zip_archive*) archive, filename, size, 0 );
	if( !(*ptr ) )
		return false;
    
	return true;
}
