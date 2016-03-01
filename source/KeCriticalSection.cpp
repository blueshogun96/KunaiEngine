//
//  ke_critical_section.h
//
//  Created by Shogun3D on 12/8/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifdef _WIN32
#pragma warning( disable:4800 )	/* Disable casting warning */
#endif

#include "KeCriticalSection.h"


/*
 * Name: KeCriticalSection::KeCriticalSection
 * Desc: Initializes a critical section object using POSIX. This is the most portable method
 *       I could find.
 */
KeCriticalSection::KeCriticalSection()
{
    /* Initialize pthread critical section */
	mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
}

/*
 * Name: KeCriticalSection::~KeCriticalSection
 * Desc: Nothing to do
 */
KeCriticalSection::~KeCriticalSection()
{
	
}

/*
 * Name: KeCriticalSection::enter
 * Desc: Simply locks the mutex representing the critical section.
 */
void KeCriticalSection::Enter()
{
    /* Lock this critical section, and lock other threads out. */
    pthread_mutex_lock( &mutex );
}

/*
 * Name: KeCriticalSection::leave
 * Desc: Unlocks the mutex representing the critical section.
 */
void KeCriticalSection::Leave()
{
    /* Unlock this critical section */
    pthread_mutex_unlock( &mutex );
}

/*
 * Name: KeCriticalSection::try_enter
 * Desc: Attempts to lock the mutex representing the critical section, and returns the result.
 */
bool KeCriticalSection::TryEnter()
{
    /* Attempt to lock this critical section. Return the result. */
    return (bool) pthread_mutex_trylock( &mutex );
}
