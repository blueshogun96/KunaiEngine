//
//  KeSemaphore.h
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
struct KeSemaphore
{
	KeSemaphore( uint32_t value );
	virtual ~KeSemaphore();

	sem_t semaphore;
};