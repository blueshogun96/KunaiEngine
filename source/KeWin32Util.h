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

#endif /* defined(__ke_win32_util__) */
