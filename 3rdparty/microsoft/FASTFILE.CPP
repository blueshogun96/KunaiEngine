/*==========================================================================
 *
 *  Copyright (C) 1995-1997 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       fastfile.c
 *  Content:	Fast file I/O for large numbers of files.
 *		Uses a single file built using FFCREATE.EXE; this file
 *		contains a directory + all the files.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTBILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 ***************************************************************************/
#ifdef _WIN32
 #include <windows.h>
 #include <io.h>
 #include <malloc.h>
#else
 #include <unistd.h>
 #include <sys/mman.h>
 #include <sys/types.h>
 #include <sys/stat.h>
#endif
#include <fcntl.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fastfile.hpp"

#ifdef DEBUG
#define ODS( a ) OutputDebugString( a )
#else
#define ODS( a )
#endif

#if !defined(_WIN32)
#define _stricmp strcmp
#endif

/*
 * Compare 
 *
 * bsearch comparison routine
 */
int Compare( LPFILEENTRY p1, LPFILEENTRY p2 )
{
    return( _stricmp( (p1)->name,(p2)->name ) );

} /* Compare */

/*
 * CFastFile::Init
 *
 * Initialize for fast file access. The master file and maximum number
 * of open "files" are specified.
 */
BOOL CFastFile::Init( LPSTR fname, int max_handles )
{
    LockCount = 0;
    this->Fini();

    /*
     * get a file handle array
     */
#ifdef _WIN32
    lpFH = LocalAlloc( LPTR, max_handles * sizeof( FILEHANDLE ) );
#else
    lpFH = (LPFILEHANDLE) malloc( max_handles * sizeof( FILEHANDLE ) );
#endif
    if( lpFH == NULL ) {
	return FALSE;
    }
    dwFHCnt = max_handles;

    /*
     * create a memory mapped file for the master file
     */
#ifdef _WIN32
    hFile = CreateFile( fname, GENERIC_READ, FILE_SHARE_READ, NULL,
            OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, 0 );

    if( hFile == NULL || hFile == (HANDLE)HFILE_ERROR )
	{
        ODS( "CFastFile::Init: CreateFile(" ); ODS( fname ); ODS( ")\r\n" );
        hFile = NULL;
        CFastFile::Fini();
        return FALSE;
    }
    hFileMapping = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, 0, NULL );
    if( hFileMapping == NULL ) {
        ODS( "CFastFile::Init: CreateFileMapping Failed\r\n" );
        FastFileFini();
        return FALSE;
    }
    pBase = MapViewOfFile( hFileMapping, FILE_MAP_READ, 0, 0, 0 );
    if( pBase == NULL ) {
        ODS( "CFastFile::Init: MapViewOfFile Failed\r\n" );
        FastFileFini();
        return FALSE;
    }
#else
    hFile = open( fname, O_RDONLY );
    if( hFile < 0 ) {
        this->Fini();
        return FALSE;
    }
    lFileSize = lseek( hFile, 0, SEEK_END );
    lseek( hFile, 0, SEEK_SET );
    
    pBase = (LPBYTE) mmap( NULL, lFileSize, PROT_READ, MAP_PRIVATE, hFile, 0 );
#endif

    /*
     * get initial data from the memory mapped file
     */
    dwFECnt = *((DWORD *) pBase);
    pFE = (LPFILEENTRY) (pBase + 4);
    lFileEnd = pFE[dwFECnt-1].offset;

    return TRUE;

} /* CFastFile::Init */

/*
 * CFastFile::Fini
 *
 * Clean up resources
 */
void CFastFile::Fini( void )
{
    //
    //  dont unmap things if we have locks out standing
    //
    if (LockCount != 0)
        return;

#ifdef _WIN32
    if( hFileMapping != NULL && pBase ) {
        UnmapViewOfFile( pBase );
    }
    if( hFileMapping != NULL ) {
        CloseHandle( hFileMapping );
        hFileMapping = NULL;
    }
    if( hFile != NULL ) {
        CloseHandle( hFile );
        hFile = NULL;
    }
    if( lpFH != NULL ) {
        LocalFree( lpFH );
        lpFH = NULL;
    }
#else
    if( pBase )
        munmap( pBase, lFileSize );
    if( hFile < -1 ) {
        close( hFile );
        hFile = 0;
    }
#endif
    
    dwFHCnt = 0;
    pBase = NULL;
    dwFECnt = 0;
    pFE = NULL;

} /* CFastFile::Fini */

/*
 * CFastFile::Open
 *
 * Search the directory for the file, and return a file handle if found.
 */
HFASTFILE CFastFile::Open( LPSTR name )
{
    FILEENTRY	fe;
    LPFILEENTRY	pfe;

    if( pFE == NULL ) {
	ODS( "CFastFile::Open: not initialized\r\n" );
	return NULL;
    }
    if( name == NULL || name[0] == 0 ) {
	ODS( "CFastFile::Open: invalid name\r\n" );
	return NULL;
    }

    strcpy( fe.name, name );
    pfe = (LPFILEENTRY) bsearch( &fe, pFE, dwFECnt, sizeof( FILEENTRY ),
#ifdef _WIN32
                  (LPVOID) Compare
#else
                    (int(*)(const void*, const void*)) Compare
#endif
    );
    if( pfe != NULL ) {
	DWORD	i;
	for( i=0;i<dwFHCnt;i++ ) {
	    if( !lpFH[i].inuse ) {
		lpFH[i].inuse = TRUE;
		lpFH[i].pos = pfe->offset;
		lpFH[i].size = (pfe+1)->offset - pfe->offset;
		lpFH[i].pfe = pfe;
		return &lpFH[i];
	    }
	}
	ODS( "CFastFile::Open: Out of file handles\r\n" );
    } else {
	ODS( "CFastFile::Open: File \"" ); ODS( name ); ODS( "\" not found\r\n" );
    }

    return NULL;

} /* CFastFile::Open */

/*
 * CFastFile::Close
 *
 * Mark a fast file handle as closed
 */
BOOL CFastFile::Close( LPFILEHANDLE pfh )
{
    if( pfh == NULL || pfh->inuse != TRUE ) {
	ODS( "CFastFile::Close: invalid handle\r\n" );
	return FALSE;
    }
    pfh->inuse = FALSE;
    return TRUE;

} /* CFastFile::Close */

/*
 * CFastFile::Lock
 *
 * return a memory pointer into a fast file
 */
LPVOID CFastFile::Lock( LPFILEHANDLE pfh, int pos, int size )
{
    if( pfh == NULL || pfh->inuse != TRUE ) {
        ODS( "CFastFile::Lock: invalid handle\r\n" );
        return NULL;
    }
    if( size < 0 ) {
        ODS( "CFastFile::Lock: invalid size\r\n" );
        return NULL;
    }
    if( (pos + size) > ((pfh->pfe)+1)->offset ) {
        ODS( "CFastFile::Lock: read past end of file\r\n" );
        return NULL;
    }
    LockCount++;
    return pBase + pfh->pos + pos;

} /* CFastFile::Lock */

/*
 * CFastFile::Unlock
 *
 */
BOOL CFastFile::Unlock( LPFILEHANDLE pfh, int pos, int size )
{
    if( pfh == NULL || pfh->inuse != TRUE ) {
        ODS( "CFastFile::Unlock: invalid handle\r\n" );
        return FALSE;
    }
    if( size < 0 ) {
        ODS( "CFastFile::Unlock: invalid size\r\n" );
        return FALSE;
    }
    if( (pos + size) > ((pfh->pfe)+1)->offset ) {
        ODS( "CFastFile::Unlock: read past end of file\r\n" );
        return FALSE;
    }

    LockCount--;
    return TRUE;

} /* CFastFile::Unlock */

/*
 * CFastFile::Read
 *
 * read from a fast file (memcpy!)
 */
BOOL CFastFile::Read( LPFILEHANDLE pfh, LPVOID ptr, int size )
{
    if( pfh == NULL || pfh->inuse != TRUE ) {
	ODS( "CFastFile::Read: invalid handle\r\n" );
	return FALSE;
    }
    if( size < 0 ) {
	ODS( "CFastFile::Read: invalid size\r\n" );
	return FALSE;
    }
    if( (pfh->pos + size) > ((pfh->pfe)+1)->offset ) {
	ODS( "CFastFile::Read: read past end of file\r\n" );
	return FALSE;
    }
    memcpy( ptr, pBase + pfh->pos, size );
    pfh->pos += size;
    return TRUE;

} /* CFastFile::Read */

/*
 * CFastFile::Seek
 *
 * Set to a new position in a fast file.  Uses standard SEEK_SET, SEEK_CUR,
 * SEEK_END definitions.
 */
BOOL CFastFile::Seek( LPFILEHANDLE pfh, int off, int how )
{
    LPFILEENTRY	pfe;

    if( pfh == NULL || pfh->inuse != TRUE ) {
	ODS( "CFastFile::Seek: invalid handle\r\n" );
	return FALSE;
    }
    pfe = pfh->pfe;
    if( how == SEEK_SET ) {
	if( off < 0 || off >= pfh->size ) {
	    ODS( "CFastFile::Seek: Invalid offset\r\n" );
	    return FALSE;
	}
	off += pfe->offset;
    } else if( how == SEEK_END ) {
	if( off < 0 || off >= pfh->size ) {
	    ODS( "CFastFile::Seek: Invalid offset\r\n" );
	    return FALSE;
	}
	off = (pfe+1)->offset - off;
    } else if( how == SEEK_CUR ) {
	off = pfh->pos + off;
	if( off < pfe->offset || off >= (pfe+1)->offset ) {
	    ODS( "CFastFile::Seek: Invalid offset\r\n" );
	    return FALSE;
	}
    } else {
	ODS( "CFastFile::Seek: Invalid 'how' field\r\n" );
	return FALSE;
    }
    pfh->pos = off;
    return TRUE;

} /* CFastFile::Seek */

/*
 * CFastFile::Tell
 *
 * Get the current position in a fast file
 */
long CFastFile::Tell( LPFILEHANDLE pfh )
{
    if( pfh == NULL || pfh->inuse != TRUE ) {
	ODS( "CFastFile::Tell: invalid handle\r\n" );
	return -1;
    }
    return pfh->pos - pfh->pfe->offset;

} /* CFastFile::Tell */
