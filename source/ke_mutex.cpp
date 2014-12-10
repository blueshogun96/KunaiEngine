//
//  ke_mutex.h
//
//  Created by Shogun3D on 12/8/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#pragma warning( disable:4800 )	/* Disable casting warning */

#include "ke_mutex.h"


/*
 * Name: ke_mutex_t::ke_mutex_t
 * Desc: Initializes a mutex object using POSIX. 
 */
ke_mutex_t::ke_mutex_t()
{
	pthread_mutex_init( &mutex, NULL );
}

/*
 * Name: ke_mutex_t::~ke_mutex_t
 * Desc: Destroy the mutex.
 */
ke_mutex_t::~ke_mutex_t()
{
	pthread_mutex_destroy( &mutex );
}

/*
 * Name: ke_mutex_t::enter
 * Desc: Simply locks the mutex.
 */
void ke_mutex_t::enter()
{
    /* Lock this mutex, and lock other threads out. */
    pthread_mutex_lock( &mutex );
}

/*
 * Name: ke_mutex_t::leave
 * Desc: Unlocks the mutex.
 */
void ke_mutex_t::leave()
{
    /* Unlock this mutex */
    pthread_mutex_unlock( &mutex );
}

/*
 * Name: ke_mutex_t::try_enter
 * Desc: Attempts to lock the mutex, and returns the result.
 */
bool ke_mutex_t::try_enter()
{
    /* Attempt to lock this mutex. Return the result. */
    return (bool) pthread_mutex_trylock( &mutex );
}
