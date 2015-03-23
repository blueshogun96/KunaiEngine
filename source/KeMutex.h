//
//  KeMutex.h
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
struct KeMutex
{
	KeMutex();
	virtual ~KeMutex();

	void Enter();
	void Leave();
	bool TryEnter();

	pthread_mutex_t mutex;
};