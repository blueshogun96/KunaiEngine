//
//  KeOSXUtil.cpp
//
//  Created by Shogun3D on 8/21/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KePlatform.h"
#include "KeOSXUtil.h"

#if 0   /* Linux headers */
#include <sys/statvfs.h>
#include <sys/statfs.h>
#include <sys/vfs.h>
#endif

#include <sys/param.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <semaphore.h>

#include "KeSemaphore.h"

KeSemaphore semaphore;


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
    
    return 1;
}

/*
 * Name: KeIsOnlyInstance
 * Desc: Returns true if this is the only instance of the application running.
 * NOTE: This is generally only relevant on desktop OSes.
 */
bool KeIsOnlyInstance( const char* title )
{
    /* TODO: Support MacOSX? */
#if 0
    HANDLE handle = CreateMutex( NULL, TRUE, title );
    
    if( GetLastError() != ERROR_SUCCESS )
    {
        HWND hwnd = FindWindow( title, NULL );
        if( hwnd )
        {
            ShowWindow( hwnd, SW_SHOW );
            SetFocus( hwnd );
            SetForegroundWindow( hwnd );
            SetActiveWindow( hwnd );
            return false;
        }
    }
#endif
    
    return true;
}

/*
 * Name: KeGetDiskSpace
 * Desc: Returns the amount of space free on the current disk drive.
 */
uint64_t KeGetDiskSpace()
{
    /* TODO: This needs to be tested */
    
    char wd[2048];
    uint64_t free_bytes = 0;
    
    if( getcwd( wd, 2048 ) != NULL )
    {
        struct statfs64 chdfs;
        
        statfs64( (const char*) wd, &chdfs );
        
        free_bytes = uint64_t(chdfs.f_bsize) * chdfs.f_bfree;
    }
    
    return free_bytes;
}

/*
 * Name: KeGetCpuSpeed
 * Desc: Returns this machine's current CPU speed.
 */
uint64_t KeGetCpuSpeed()
{
    uint64_t speed = 0;
    size_t len = sizeof( uint64_t );
    
    sysctlbyname( "hw.cpufrequency_max", &speed, &len, NULL, 0 );
    
    return speed;
}


/*
 * Name: KeGetCpuCount()
 * Desc: Returns the number of physical CPU cores on this machine.
 */
int KeGetCpuCount()
{
    /* Source: http://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine */
    
    int nm[2];
    size_t len = 4;
    uint32_t count;
    
    nm[0] = CTL_HW;
    nm[1] = HW_AVAILCPU;
    sysctl( nm, 2, &count, &len, NULL, 0 );
    
    if( count < 1 )
    {
        nm[1] = HW_NCPU;
        sysctl( nm, 2, &count, &len, NULL, 0 );
        if( count < 1 )
        {
            count = 1;
        }
    }
    
    return count;
}

/*
 * Name: KeGetPhysicalMemoryStatus
 * Desc: Returns the amount of physical memory installed and the amount that is currently used
 */
int KeGetPhysicalMemoryStatus( uint64_t* total, uint64_t* free )
{
    return 0;
}

/*
 * Name: KeGetVirtualMemoryStatus
 * Desc: Returns the amount of virtual memory installed and the amount that is currently used
 */
int KeGetVirtualMemoryStatus( uint64_t* total, uint64_t* free )
{
    return 0;
}

