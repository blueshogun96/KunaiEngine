//
//  ke_semaphore.h
//
//  Created by Shogun3D on 12/8/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#pragma once

#include "ke.h"
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>


/*
 * Semaphore structure
 */
struct ke_semaphore_t
{
	ke_semaphore_t( uint32_t value );
	virtual ~ke_semaphore_t();

	sem_t semaphore;
};