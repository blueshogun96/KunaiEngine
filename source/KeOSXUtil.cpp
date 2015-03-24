//
//  ke_win32_util.cpp
//
//  Created by Shogun3D on 8/21/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KePlatform.h"
#include "KeOSXUtil.h"


/*
 * Name: KeSetCurrentPathToResourceDirectory
 * Desc: Sets the current path to the resource directory of the app.
 */
int KeSetCurrentPathToResourceDirectory()
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


/*
 * Name: KeGetCurrentPathToResourceDirectory
 * Desc: Returns the path to this application's resource directory.
 */
int KeGetCurrentPathToResourceDirectory( char* resource_path )
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
}