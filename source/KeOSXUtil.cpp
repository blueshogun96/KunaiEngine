//
//  KeOSXUtil.cpp
//
//  Created by Shogun3D on 8/21/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KePlatform.h"
#include "KeOSXUtil.h"
#include "KeDebug.h"

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
    
    /* Attempt to open a semaphore with this title */
    if( !semaphore.Open( title, 1 ) )
    {
        /* If it failed, check the error to see if it already exists. */
        if( semaphore.GetLastError() == EEXIST )
        {
            DISPDBG( KE_WARNING, "Another instance of \"" << title << "\" appears to be running already!" );
            return false;   /* There is another instance running */
        }
        else
        {
            /* Another error occurred */
            DISPDBG( KE_WARNING, "KeSemaphore::Create() has returned " << semaphore.GetLastError() );
        }
    }
    
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

/*
 * Name: KeQuerySystemMemoryStatus
 * Desc: Returns various details about the state of the target machine's virtual and physical
 *       memory totals, usage, etc.
 */
bool KeQuerySystemMemoryStatus( KeSystemMemoryStatus* memory_status )
{
    bool error = false;
    
    /* Sanity check */
    if( !memory_status )
        return false;
    
    /* Source: http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process */
    
    /* 
     * From Apple's developer documentation:
     *
     * Unlike most Unix-based operating systems, Mac OS X does not use a preallocated swap partition for 
     * virtual memory. Instead, it uses all of the available space on the machine’s boot partition.
     *
     * That being said, there is no total virtual memory for us to query here, so this value will always
     * return 0 for the time being.
     */
    memory_status->total_virtual_memory = 0;
    DISPDBG( KE_DBGLVL(0), "Total virtual memory status cannot be queried on MacOSX" );
    
    /* Query the root directory for the total amount of available virtual memory */
    struct statfs stats;
    int ret = 0;
    
    ret = statfs( "/", &stats );
    if( ret == 0 )
    {
        memory_status->free_virtual_memory = (uint64_t)stats.f_bsize * stats.f_bfree;
    }
    else
    {
        /* TODO: Enumerate error code */
        DISPDBG( KE_WARNING, "Unable to query free virtual memory via statfs() (Error code: " << ret << ")" );
        
        error = true;
        memory_status->free_virtual_memory = 0;
    }
    
    /* Calculate the total amount of virtual memory in use by the OS altogether */
    xsw_usage vmusage = {0};
    size_t size = sizeof( vmusage );
    
    ret = sysctlbyname( "vm.swapusage", &vmusage, &size, NULL, 0 );
    if( ret == 0 )
    {
        memory_status->system_used_virtual_memory = (uint64_t) size;
    }
    else
    {
        DISPDBG( KE_WARNING, "Unable to get swap/virtual memory usage by calling sysctlbyname(\"vm.swapusage\",...) (Error code: " << ret << ")" );
        
        error = true;
        memory_status->system_used_virtual_memory = 0;
    }
    
    /* Query information about the current task, and return the amount of virtual memory this program is actually using */
    struct task_basic_info t_info;
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
    
    ret = task_info( mach_task_self(), TASK_BASIC_INFO, (task_info_t) &t_info, &t_info_count );
    if( KERN_SUCCESS != ret )
    {
        DISPDBG( KE_WARNING, "Unable to get the current virtual memory usage of this app via task_info() (Error code: " << ret << " )" );
        
        error = true;
        memory_status->program_used_virtual_memory = 0;
    }
    else
    {
        memory_status->program_used_virtual_memory = t_info.virtual_size;
    }
    
    /* Query the amount of pysical RAM installed on this Mac */
    int mib[2];
    int64_t physical_memory;
    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    size_t length = sizeof(int64_t);
    sysctl( mib, 0x2, (void*) &physical_memory, &length, NULL, 0 );
    
    memory_status->total_physical_memory = physical_memory;
    
    /* Query the amount of physical memory used by the OS */
    vm_size_t page_size;
    mach_port_t mach_port;
    mach_msg_type_number_t count;
    vm_statistics64_data_t vm_stats;
    
    mach_port = mach_host_self();
    count = sizeof(vm_stats) / sizeof(natural_t);
    ret = host_page_size( mach_port, &page_size );
    if( KERN_SUCCESS == ret )
    {
        ret = host_statistics64( mach_port, HOST_VM_INFO, (host_info64_t) &vm_stats, &count );
        
        if( ret == KERN_SUCCESS )
        {
            long long free_memory = (int64_t)vm_stats.free_count * (int64_t)page_size;
        
            long long used_memory = ((int64_t)vm_stats.active_count +
                                     (int64_t)vm_stats.inactive_count +
                                     (int64_t)vm_stats.wire_count) *  (int64_t)page_size;
            //printf("free memory: %lld\nused memory: %lld\n", free_memory, used_memory);
            
            memory_status->system_used_physical_memory = used_memory;
        }
        else
        {
            DISPDBG( KE_WARNING, "Unable to query physical memory usage (host_statistics64() failed) (Error code: " << ret << " )" );
            
            error = true;
            memory_status->system_used_physical_memory = 0;
        }
    }
    else
    {
        DISPDBG( KE_WARNING, "Unable to query physical memory usage (host_page_size() failed) (Error code: " << ret << " )" );
        
        error = true;
        memory_status->system_used_physical_memory = 0;
    }
    
    /* Query the amount of physical memory used by this app */
    struct rusage usage;
    
    ret = getrusage( RUSAGE_SELF, &usage );
    if( ret == 0 )
    {
        memory_status->program_used_physical_memory = usage.ru_maxrss;
    }
    else
    {
        DISPDBG( KE_WARNING, "Unable to query physical memory usage (getrusage() failed) (Error code: " << ret << " )" );
        
        error = true;
        memory_status->program_used_physical_memory = 0;
    }
    
    return !error;
}
