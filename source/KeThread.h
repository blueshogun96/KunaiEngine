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
typedef int (*KeThreadPfn)( void* );


/*
 * Thread structure
 */
struct KeThread
{
	KeThread( KeThreadPfn pfn );
	virtual ~KeThread();

	pthread_attr_t thread_attr;
	pthread_t thread;
};
