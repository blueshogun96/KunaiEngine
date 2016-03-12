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
#include <CoreGraphics/CGDisplayConfiguration.h>
#include <IOKit/IOKitLib.h>

#include <mach/mach_traps.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/vmmeter.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <mach/mach_port.h>
#include <mach/mach_traps.h>
#include <mach/task_info.h>
#include <mach/thread_info.h>
#include <mach/thread_act.h>
#include <mach/vm_region.h>
#include <mach/vm_map.h>
#include <mach/task.h>
#include <mach/shared_memory_server.h>

#include <vector>

/*
 * System memory status
 */
struct KeSystemMemoryStatus
{
    uint64_t total_virtual_memory;          /* Total virtual memory allocated for this machine */
    uint64_t free_virtual_memory;           /* Free virtual memory */
    uint64_t system_used_virtual_memory;    /* The amount of virtual memory used by the OS */
    uint64_t program_used_virtual_memory;   /* The amount of virtual memory used by the program */
    uint64_t total_physical_memory;         /* Total physical memory installed on this machine */
    uint64_t free_physical_memory;          /* The amount of physical memory that is free on this machine */
    uint64_t system_used_physical_memory;   /* The amount of physical memory used by the OS */
    uint64_t program_used_physical_memory;  /* The amount of physical memory used by the program */
};

/*
 * Video card info
 */
struct KeVideoCardInfo
{
    uint32_t device_id;
    uint32_t vendor_id;
};


/*
 * Name: KeSetCurrentPathToResourceDirectory
 * Desc: Sets the current path to the resource directory of the app.
 */
int KeSetCurrentPathToResourceDirectory();

/*
 * Name: KeGetCurrentPathToResourceDirectory
 * Desc: Returns the path to this application's resource directory.
 */
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
 * Name: KeGetPhysicalMemoryStatus
 * Desc: Returns the amount of physical memory installed and the amount that is currently used
 */
int KeGetPhysicalMemoryStatus( uint64_t* total, uint64_t* free );

/*
 * Name: KeGetVirtualMemoryStatus
 * Desc: Returns the amount of virtual memory installed and the amount that is currently used
 */
int KeGetVirtualMemoryStatus( uint64_t* total, uint64_t* free );

/*
 * Name: KeQuerySystemMemoryStatus
 * Desc: Returns various details about the state of the target machine's virtual and physical
 *       memory totals, usage, etc.
 */
bool KeQuerySystemMemoryStatus( KeSystemMemoryStatus* memory_status );

/*
 * Name: KeGetVideoCardInfo
 * Desc: Gets the device/vendor ID for the currently active video card.
 */
bool KeGetVideoCardInfo( KeVideoCardInfo* video_card_info );

/*
 * Name: KeGetVideoCardInfoList
 * Desc: Returns a list of device/vendor IDs for all available video cards
 */
bool KeGetVideoCardInfoList( std::vector<KeVideoCardInfo> video_card_info_list );


#endif /* defined(__ke_osx_util__) */
