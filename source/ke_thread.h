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
typedef int (*ke_thread_pfn)( void* );


/*
 * Thread structure
 */
struct ke_thread_t
{
	ke_thread_t( ke_thread_pfn pfn );
	virtual ~ke_thread_t();


	pthread_t thread;
};