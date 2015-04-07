//
//  ke_win32_util.cpp
//
//  Created by Shogun3D on 8/21/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KePlatform.h"
#include "KeWin32Util.h"


/*
* Name: KeSetCurrentPathToResourceDirectory
* Desc: Sets the current path to the resource directory of the app.
*/
int KeSetCurrentPathToResourceDirectory()
{
	printf("Resource path redirection not yet supported...\n");

	return 0;
}

int KeGetCurrentPathToResourceDirectory(char* resource_path)
{
	printf("Resource path redirection not yet supported...\n");

	return 0;
}

/*
 * Name: KeIsOnlyInstance
 * Desc: Returns true if this is the only instance of the application running.
 * NOTE: This is generally only relevant on desktop OSes.
 */
bool KeIsOnlyInstance( const char* title )
{
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

	return true;
}

/*
 * Name: KeGetDiskSpace
 * Desc: Returns the amount of space free on the current disk drive.
 */
uint64_t KeGetDiskSpace()
{
	int const drive = _getdrive();
	struct _diskfree_t diskfree;

	_getdiskfree( drive, &diskfree );

	uint64_t free_bytes = uint64_t(diskfree.avail_clusters) * 
		uint64_t(diskfree.bytes_per_sector) * uint64_t(diskfree.sectors_per_cluster);

	return free_bytes;
}

/*
 * Name: KeGetCpuSpeed
 * Desc: Returns this machine's current CPU speed.
 */
uint64_t KeGetCpuSpeed()
{
    uint32_t speed = 0;
	uint32_t buff_size = sizeof( uint32_t );
	uint32_t type = REG_DWORD;
	HKEY key;

	long error = RegOpenKeyExA( HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &key );
	if( error == ERROR_SUCCESS )
	{
		RegQueryValueExA( key, "~MHz", NULL, (DWORD*) &type, (uint8_t*) &speed, (DWORD*) &buff_size );
	}
    
    return (uint64_t) speed;
}


/*
 * Name: KeGetCpuCount()
 * Desc: Returns the number of physical CPU cores on this machine.
 */
int KeGetCpuCount()
{
    /* Source: http://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine */
    
    SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );

	return sysinfo.dwNumberOfProcessors;
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

