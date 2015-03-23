//
//  ke_ios_util.h
//
//  Created by Brandon Fleming on 10/15/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke_ios_util__
#define __ke_ios_util__

#include <CoreFoundation/CoreFoundation.h>

/*
 * Name: ke_set_current_path_to_resource_directory
 * Desc: Sets the current path to the resource directory of the app.
 */
int ke_set_current_path_to_resource_directory();
int ke_get_path_to_resource_directory( char* resource_path );

#endif /* defined(__ke_ios_util__) */
