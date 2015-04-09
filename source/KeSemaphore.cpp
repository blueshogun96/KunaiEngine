//
//  KeSemaphore.cpp
//
//  Created by Shogun3D on 12/8/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KeSemaphore.h"


KeSemaphore::KeSemaphore() :
#ifndef _WIN32
	semaphore_ptr(NULL), semaphore(-1), named(No), valid(No), last_error(EINVAL)
#else
	semaphore(NULL), named(No), valid(No), last_error(0)
#endif
{
    
}

KeSemaphore::KeSemaphore( bool shared, uint32_t value ) : 
#ifndef _WIN32
	semaphore_ptr(NULL), semaphore(-1), named(No), valid(No), last_error(EINVAL)
#else
	semaphore(NULL), named(No), valid(No), last_error(0)
#endif
{
#ifndef _WIN32
    int ret = sem_init( &semaphore, shared ? Yes : No, value );
    if( ret != -1 )
        valid = Yes;
    
    last_error = errno;
#else
	/*semaphore = CreateSemaphore( NULL, value, value, NULL );
	if( semaphore != NULL )
		valid = Yes;

	last_error = GetLastError();*/
#endif
}

KeSemaphore::KeSemaphore( const char* name, uint32_t value ) : 
#ifndef _WIN32
	semaphore_ptr(NULL), semaphore(-1), named(No), valid(No), last_error(EINVAL)
#else
	semaphore(NULL), named(No), valid(No), last_error(0)
#endif
{
#ifndef _WIN32
    semaphore_ptr = sem_open( name, O_CREAT | O_EXCL, 0644, value );
    if( semaphore_ptr != SEM_FAILED )
    {
        valid = Yes;
        memcpy( semaphore_name, name, strlen( name ) );
    }
    
    last_error = errno;
#else
#endif
}

bool KeSemaphore::Create( bool shared, uint32_t value )
{
#ifndef _WIN32
    int ret = sem_init( &semaphore, shared ? Yes : No, value );
    if( ret != -1 )
        valid = Yes;
    
    last_error = errno;
#else
#endif
    
    return valid;
}

bool KeSemaphore::Open( const char* name, uint32_t value )
{
#ifndef _WIN32
    semaphore_ptr = sem_open( name, O_CREAT | O_EXCL, 0644, value );
    if( semaphore_ptr != SEM_FAILED )
    {
        valid = Yes;
        memcpy( semaphore_name, name, strlen( name ) );
    }
    
    last_error = errno;
#else
#endif

    return valid;
}

bool KeSemaphore::Wait()
{
#ifndef _WIN32
    if( semaphore_ptr != SEM_FAILED )
        sem_wait( semaphore_ptr );
    
    if( semaphore != -1 )
        sem_wait( &semaphore );
#else
#endif
    
    return true;
}

bool KeSemaphore::TryWait()
{
#ifndef _WIN32
    if( semaphore_ptr != SEM_FAILED )
        sem_trywait( semaphore_ptr );
    
    if( semaphore != -1 )
        sem_trywait( &semaphore );
#else
#endif
    
    return true;
}

bool KeSemaphore::GetValue( int* value )
{
#ifndef _WIN32
    if( semaphore_ptr != SEM_FAILED )
        sem_getvalue( semaphore_ptr, value );
    
    if( semaphore != -1 )
        sem_getvalue( &semaphore, value );
#else
#endif
    
    return true;
}

KeSemaphore::~KeSemaphore()
{
#ifndef _WIN32
    if( semaphore != -1 )
        sem_destroy( &semaphore );
    
    if( semaphore_ptr != SEM_FAILED )
    {
        sem_unlink( semaphore_name );
        sem_close( semaphore_ptr );
    }
#else
#endif
    
    valid = No;
}

bool KeSemaphore::IsValid()
{
    return valid;
}