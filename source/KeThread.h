//
//  ke_thread.h
//
//  Created by Shogun3D on 12/8/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#pragma once

#include "ke.h"
#include <pthread.h>


/*
 * OS specific defines
 */
#ifndef _WIN32
typedef void (*KeThreadPfn)( void* );

#define KeExitThread() pthread_exit(NULL)
#else
typedef uint32_t (__stdcall *KeThreadPfn)( void* );

#define KeExitThread() ExitThread(0)
#endif


/*
 * Thread structure
 */
struct KeThread
{
	KeThread( KeThreadPfn pfn, void* context = NULL, bool suspended = No );
	virtual ~KeThread();

	bool Wait( uint32_t timeout );
	uint32_t GetLastError();
	void Suspend(), Resume();

#ifndef _WIN32
	pthread_attr_t thread_attr;
	pthread_t thread;
	pthread_mutex_t mutex;
#else
	HANDLE thread, mutex;
	uint32_t thread_id;
#endif
	uint32_t last_error;
};
