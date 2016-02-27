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

/*
 * Name: KeGetCpuSpeed
 * Desc: Returns this machine's current CPU speed.
 */
uint64_t KeGetCpuSpeed();

/*
 * Name: KeGetCpuCount()
 * Desc: Returns the number of physical CPU cores on this machine.
 */
int KeGetCpuCount();

/*
 * Name: KeGetCpuCapabilities
 * Desc: Checks this CPU's capabilities via CPUID (for x86)
 */
bool KeGetCpuCapabilities();

/*
 * Name: KeGetPhysicalMemoryStatus
 * Desc: Returns the amount of physical memory installed and the amount that is currently used
 */
int KeGetPhysicalMemoryStatus( uint64_t* total, uint64_t* free );

/*
 * Name: KeGetVirtualMemoryStatus
 * Desc: Returns the amount of virtual memory installed and the amount that is currently used
 */
int KeGetVirtualMemoryStatus( uint64_t* total, uint64_t* free );

#endif /* defined(__ke_win32_util__) */
