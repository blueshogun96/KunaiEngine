//
//  ke_win32_util.h
//
//  Created by Shogun3D on 8/21/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke_win32_util__
#define __ke_win32_util__

#include <windows.h>
#include <mmsystem.h>

/*
 * Name: KeSetCurrentPathToResourceDirectory
 * Desc: Sets the current path to the resource directory of the app.
 */
int KeSetCurrentPathToResourceDirectory();

int KeGetCurrentPathToResourceDirectory( char* resource_path );

/*
 * Name: KeIsOnlyInstance
 * Desc: Returns true if this is the only instance of the application running.
 * NOTE: This is generally only relevant on desktop OSes.
 */
bool KeIsOnlyInstance( const char* title );

/*
 * Name: KeGetDiskSpace
 * Desc: Returns the amount of space free on the current disk drive.
 */
uint64_t KeGetDiskSpace();

#endif /* defined(__ke_win32_util__) */
