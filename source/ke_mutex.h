//
//  ke_mutex.h
//
//  Created by Shogun3D on 12/8/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#pragma once

#include "ke.h"
#include <pthread.h>


/* 
 * Mutex structure
 */
struct ke_mutex_t
{
	ke_mutex_t();
	virtual ~ke_mutex_t();

	void enter();
	void leave();
	bool try_enter();

	pthread_mutex_t mutex;
};