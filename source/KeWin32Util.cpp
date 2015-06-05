//
//  ke_win32_util.cpp
//
//  Created by Shogun3D on 8/21/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KePlatform.h"
#include "KeWin32Util.h"
#include <ShlObj.h>
#include <tchar.h>


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
	MEMORYSTATUSEX status;

	if( !GlobalMemoryStatusEx( &status ) )
		return 0;

	*total = status.ullTotalPhys;
	*free = status.ullAvailPhys;

    return 1;
}

/*
 * Name: KeGetVirtualMemoryStatus
 * Desc: Returns the amount of virtual memory installed and the amount that is currently used
 */
int KeGetVirtualMemoryStatus( uint64_t* total, uint64_t* free )
{
	MEMORYSTATUSEX status;

	if( !GlobalMemoryStatusEx( &status ) )
		return 0;

	*total = status.ullTotalVirtual;
	*free = status.ullAvailVirtual;

    return 1;
}

const char* KeGetSaveGameDirectory( const char* game_app_directory )
{
	HRESULT hr;
	char user_data_path[MAX_PATH];
	char save_game_directory[MAX_PATH];

	/* Build our game save directory */
	hr = SHGetSpecialFolderPath( GetActiveWindow(), user_data_path, CSIDL_APPDATA, Yes );

	strcpy( save_game_directory, user_data_path );
	strcat( save_game_directory, "\\" );
	strcat( save_game_directory, game_app_directory );

	/* Does this directory already exist? */
	if( 0xFFFFFFFF == GetFileAttributesA( save_game_directory ) )
	{
		/* If not, attempt to create it*/
		if( SHCreateDirectoryEx( GetActiveWindow(), save_game_directory, NULL ) != ERROR_SUCCESS )
			return NULL;
	}

	/* Return the directory */
	strcat( save_game_directory, "\\" );

	return save_game_directory;
}

void KeRequestUserAttention()
{
	HWND hwnd = GetActiveWindow();
	if( !hwnd )
		return;

	/* Blink until the app is no longer minimized */
	if( IsIconic( hwnd ) )
	{
		DWORD now = timeGetTime();
		DWORD then = now;
		MSG msg;

		FlashWindow( hwnd, Yes );

		while( Yes )
		{
			if( PeekMessage( &msg, NULL, 0, 0, 0 ) )
			{
				if( msg.message != WM_SYSCOMMAND && msg.wParam != SC_CLOSE )
				{
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}

				/* Are we done yet? */
				if( !IsIconic( hwnd ) )
				{
					FlashWindow( hwnd, No );
					break;
				}
			}
			else
			{
				now = timeGetTime();
				DWORD time_span = now > then ? ( now - then ) : ( then  - now );
				if( time_span > 1000 )
				{
					then = now;
					FlashWindow( hwnd, Yes );
				}
			}
		}
	}
}


