//
//  KeMutex.h
//
//  Created by Shogun3D on 12/8/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#pragma warning( disable:4800 )	/* Disable casting warning */

#include "KeMutex.h"


/*
 * Name: KeMutex::KeMutex
 * Desc: Initializes a mutex object using POSIX. 
 */
KeMutex::KeMutex()
{
	pthread_mutex_init( &mutex, NULL );
}

/*
 * Name: KeMutex::~KeMutex
 * Desc: Destroy the mutex.
 */
KeMutex::~KeMutex()
{
	pthread_mutex_destroy( &mutex );
}

/*
 * Name: KeMutex::Enter
 * Desc: Simply locks the mutex.
 */
void KeMutex::Enter()
{
    /* Lock this mutex, and lock other threads out. */
    pthread_mutex_lock( &mutex );
}

/*
 * Name: KeMutex::leave
 * Desc: Unlocks the mutex.
 */
void KeMutex::Leave()
{
    /* Unlock this mutex */
    pthread_mutex_unlock( &mutex );
}

/*
 * Name: KeMutex::try_enter
 * Desc: Attempts to lock the mutex, and returns the result.
 */
bool KeMutex::TryEnter()
{
    /* Attempt to lock this mutex. Return the result. */
    return (bool) pthread_mutex_trylock( &mutex );
}
