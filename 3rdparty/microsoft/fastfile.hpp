/*==========================================================================
 *
 *  Copyright (C) 1995-1997 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       fastfile.h
 *  Content:	Definitions for fastfile access.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTBILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 ***************************************************************************/

#include "ffent.h"

#ifndef _WIN32
#define LPVOID  void*
#define BOOL    int
#define LONG    long
#define LPBYTE  unsigned char*
#define HANDLE  void*
#define DWORD   unsigned long
#define LPSTR   char*
#define FAR
#define OutputDebugString printf
#define TRUE    1
#define FALSE   0
#endif

typedef LPVOID	HFASTFILE;

/*
 * File handle structure 
 */
typedef struct {
    BOOL	inuse;
    LONG	pos;
    LONG	size;
    LPFILEENTRY	pfe;
} FILEHANDLE, FAR *LPFILEHANDLE;

class CFastFile
{
public:
    CFastFile() {}
    virtual ~CFastFile() { Fini(); };

public:
    BOOL        Init( LPSTR fname, int max_handles );
    void        Fini( void );
    HFASTFILE   Open( LPSTR name );
    BOOL        Close( LPFILEHANDLE pfh );
    BOOL        Read( LPFILEHANDLE pfh, LPVOID ptr, int size );
    BOOL        Seek( LPFILEHANDLE pfe, int off, int how );
    long        Tell( LPFILEHANDLE pfe );
    LPVOID      Lock( LPFILEHANDLE pfe, int off, int len );
    BOOL        Unlock( LPFILEHANDLE pfe, int off, int len );
    
protected:
    int              LockCount;
#ifdef _WIN32
    HANDLE           hFile;
#else
    int              hFile;
#endif
    HANDLE           hFileMapping;
    LPFILEENTRY      pFE;
    LPBYTE           pBase;
    DWORD            dwFECnt;
    LPFILEHANDLE     lpFH;
    DWORD            dwFHCnt;
    long             lFileEnd;
    long             lFileSize;
};

