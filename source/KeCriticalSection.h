//
//  ke_critical_section.h
//
//  Created by Shogun3D on 12/8/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#pragma once

#include "Ke.h"
#include <pthread.h>

/*
 * Critical section structure
 */
struct KeCriticalSection
{
public:
	KeCriticalSection();
	virtual ~KeCriticalSection();

	void Enter();
	void Leave();
	bool TryEnter();

	pthread_mutex_t mutex;
};
