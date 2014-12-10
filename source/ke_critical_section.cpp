//
//  ke_critical_section.h
//
//  Created by Shogun3D on 12/8/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#pragma warning( disable:4800 )	/* Disable casting warning */

#include "ke_critical_section.h"


/*
 * Name: ke_critical_section_t::ke_critical_section_t
 * Desc: Initializes a critical section object using POSIX. This is the most portable method
 *       I could find.
 */
ke_critical_section_t::ke_critical_section_t()
{
    /* Initialize pthread critical section */
	mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
}

/*
 * Name: ke_critical_section_t::~ke_critical_section_t
 * Desc: Nothing to do
 */
ke_critical_section_t::~ke_critical_section_t()
{
	
}

/*
 * Name: ke_critical_section_t::enter
 * Desc: Simply locks the mutex representing the critical section.
 */
void ke_critical_section_t::enter()
{
    /* Lock this critical section, and lock other threads out. */
    pthread_mutex_lock( &mutex );
}

/*
 * Name: ke_critical_section_t::leave
 * Desc: Unlocks the mutex representing the critical section.
 */
void ke_critical_section_t::leave()
{
    /* Unlock this critical section */
    pthread_mutex_unlock( &mutex );
}

/*
 * Name: ke_critical_section_t::try_enter
 * Desc: Attempts to lock the mutex representing the critical section, and returns the result.
 */
bool ke_critical_section_t::try_enter()
{
    /* Attempt to lock this critical section. Return the result. */
    return (bool) pthread_mutex_trylock( &mutex );
}
