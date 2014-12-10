//
//  ke_critical_section.h
//
//  Created by Shogun3D on 12/8/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#pragma once

#include "ke.h"
#include <pthread.h>

/*
 * Critical section structure
 */
struct ke_critical_section_t
{
public:
	ke_critical_section_t();
	virtual ~ke_critical_section_t();

	void enter();
	void leave();
	bool try_enter();

	pthread_mutex_t mutex;
};
