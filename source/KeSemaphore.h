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
    KeSemaphore();
	KeSemaphore( bool shared, uint32_t value );
    KeSemaphore( const char* name, uint32_t value );
	virtual ~KeSemaphore();

    bool        Create( bool shared, uint32_t value );
    bool        Open( const char* name, uint32_t value );
    bool        IsValid();
    uint32_t    GetLastError() { return last_error; }
    
	sem_t* semaphore_ptr;
    sem_t semaphore;
    bool named, valid;
    uint32_t last_error;
};