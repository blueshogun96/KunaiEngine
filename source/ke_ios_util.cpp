//
//  ke_ios_util.cpp
//  rigidbodies
//
//  Created by Brandon Fleming on 10/15/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "ke_platform.h"
#include "ke_ios_util.h"

#include <CoreFoundation/CoreFoundation.h>
#include <AudioToolbox/AudioToolbox.h>


/*
 * Name: ke_set_current_path_to_resource_directory
 * Desc: Sets the current path to the resource directory of the app.
 */
int ke_set_current_path_to_resource_directory()
{
    CFBundleRef main_bundle = CFBundleGetMainBundle();
    CFURLRef resources_url = CFBundleCopyResourcesDirectoryURL(main_bundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resources_url, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
        return 0;
    }
    CFRelease(resources_url);
    
    chdir(path);
    printf( "Current directory changed to: %s\n", path );
    
    return 1;
}

int ke_get_path_to_resource_directory( char* resource_path )
{
    CFBundleRef main_bundle = CFBundleGetMainBundle();
    CFURLRef resources_url = CFBundleCopyResourcesDirectoryURL(main_bundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resources_url, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
        return 0;
    }
    CFRelease(resources_url);
    
    strcpy( resource_path, path );
    
    return 1;
}