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

extern BOOL FastFileInit( LPSTR fname, int max_handles );
extern void FastFileFini( void );
extern HFASTFILE FastFileOpen( LPSTR name );
extern BOOL FastFileClose( HFASTFILE pfe );
extern BOOL FastFileRead( HFASTFILE pfh, LPVOID ptr, int size );
extern BOOL FastFileSeek( HFASTFILE pfe, int off, int how );
extern long FastFileTell( HFASTFILE pfe );
extern LPVOID FastFileLock( HFASTFILE pfe, int off, int len );
extern BOOL FastFileUnlock( HFASTFILE pfe, int off, int len );
