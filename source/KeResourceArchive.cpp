//
//  KeResourceArchive.cpp
//
//  Created by Shogun3D on 12/3/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

//#define MINIZ_HEADER_INCLUDED

#include "KeResourceArchive.h"
#include <miniz.c>
#include "fastfile.hpp"


/*
 * Zip archive reader
 */

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

bool KeZipResourceArchive::ReadTexture( std::string filename, uint32_t desired_target, IKeTexture** texture )
{
    void* buffer;
    KeImageData img;
    size_t size;
    
    if( Read( filename, &buffer, &size ) )
    {
        if( KeImageReadFromMemory( buffer, size, &img ) )
        {
            /* TODO: Auto-determine texture format */
            KeGetRenderDevice()->CreateTexture2D( desired_target, img.width, img.height, 0, KE_TEXTUREFORMAT_BGRA, KE_UNSIGNED_BYTE, texture, img.pixels );
        
            KeImageClose( &img );
            free(buffer);
        }
        else
            return false;
    }
    else
        return false;
    
    return true;
}

bool KeZipResourceArchive::ReadSoundBuffer( std::string filename, IKeSoundBuffer** soundbuffer )
{
    void* buffer;
    size_t size;
    KeSoundData snd;
    
    if( Read( filename, &buffer, &size ) )
    {
        if( KeSoundReadWAVFromMemory( buffer, &snd ) )
        {
            bool ret = KeGetAudioDevice()->CreateSoundBuffer( &snd.wfx, soundbuffer );
            if( ret )
                (*soundbuffer)->SetBufferData( snd.ptr, snd.bytes );
            
            KeSoundClose( &snd );
            free(buffer);
        }
        else
            return false;
    }
    else
        return false;
    
    return true;
}


/*
 * FastFile archive reader
 */

KeFastFileResourceArchive::KeFastFileResourceArchive() : archive(nullptr)
{
    opened = No;
}

KeFastFileResourceArchive::KeFastFileResourceArchive( std::string filename ) : archive(nullptr)
{
    archive = new CFastFile;
    
    /* Open the desired archive */
    opened = static_cast<CFastFile*>( archive )->Init( (char*) filename.c_str(), 63556 );
}

KeFastFileResourceArchive::~KeFastFileResourceArchive()
{
    Close();
}

bool KeFastFileResourceArchive::IsOpen()
{
    return opened;
}

void KeFastFileResourceArchive::Close()
{
    if( archive )
    {
        CFastFile* ff = static_cast<CFastFile*>( archive );
        
        /* Close the fastfile, if it is open. */
        ff->Fini();
        
        /* Delete the archive pointer */
        delete static_cast<CFastFile*>( archive );
        archive = nullptr;
    }
}

bool KeFastFileResourceArchive::Read( std::string filename, void** ptr, size_t* size )
{
    return true;
}


bool KeFastFileResourceArchive::ReadString( std::string filename, void** ptr, size_t* size )
{
    return true;
}

bool KeFastFileResourceArchive::ReadTexture( std::string filename, uint32_t desired_target, IKeTexture** texture )
{
    return true;
}

bool KeFastFileResourceArchive::ReadSoundBuffer( std::string filename, IKeSoundBuffer** soundbuffer )
{
    return true;
}
