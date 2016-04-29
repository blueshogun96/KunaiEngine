//
//  KeResourceArchive.cpp
//
//  Created by Shogun3D on 12/3/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

//#define MINIZ_HEADER_INCLUDED

#include "KeResourceArchive.h"
#include <miniz.c>


KeZipResourceArchive::KeZipResourceArchive() : archive(nullptr)
{
	opened = No;
}

KeZipResourceArchive::KeZipResourceArchive( std::string filename ) : archive(nullptr)
{
	archive = new mz_zip_archive;

	/* Open the desired archive */
	ZeroMemory( archive, sizeof( mz_zip_archive ) );
	opened = (bool) mz_zip_reader_init_file( (mz_zip_archive*) archive, filename.c_str(), 0 );
}

KeZipResourceArchive::~KeZipResourceArchive()
{
	Close();
}

bool KeZipResourceArchive::IsOpen()
{
	return opened;
}

void KeZipResourceArchive::Close()
{
    if( archive )
    {
        /* Close the fastfile, if it is open. */
        mz_zip_reader_end( (mz_zip_archive*) archive );
        
        /* Delete the archive pointer */
        delete static_cast<mz_zip_archive*>(archive);
        archive = nullptr;
    }
}

bool KeZipResourceArchive::Read( std::string filename, void** ptr, size_t* size )
{
	*ptr = mz_zip_reader_extract_file_to_heap( (mz_zip_archive*) archive, filename.c_str(), size, 0 );
	if( !(*ptr ) )
		return false;
    
	return true;
}


bool KeZipResourceArchive::ReadString( std::string filename, void** ptr, size_t* size )
{
	*ptr = mz_zip_reader_extract_file_to_heap( (mz_zip_archive*)archive, filename.c_str(), size, 0 );
	if (!(*ptr))
		return false;

	/* Add one more byte to this allocation */
	*ptr = realloc( *ptr, (*size) + 1 );
	/* Set that extra byte to 0 to terminate the string */
	((char*)(*ptr))[*size] = 0;
	(*size)++;

	return true;
}

