//
//  KeOSXUtil.h
//
//  Created by Shogun3D on 7/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke_osx_util__
#define __ke_osx_util__

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CGDirectDisplay.h>

/*
 * Name: KeSetCurrentPathToResourceDirectory
 * Desc: Sets the current path to the resource directory of the app.
 */
int KeSetCurrentPathToResourceDirectory();

/*
 * Name: KeGetCurrentPathToResourceDirectory
 * Desc: Returns the path to this application's resource directory.
 */
int KeGetCurrentPathToResourceDirectory(char* resource_path);

#endif /* defined(__ke_osx_util__) */
