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
 * Thread function pointer
 */
#ifndef _WIN32
typedef void (*KeThreadPfn)( void* );
#else
typedef uint32_t (__stdcall *KeThreadPfn)( void* );
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

#ifndef _WIN32
	pthread_attr_t thread_attr;
	pthread_t thread;
#else
	HANDLE thread;
	uint32_t thread_id;
#endif
	uint32_t last_error;
};
