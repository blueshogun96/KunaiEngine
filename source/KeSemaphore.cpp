//
//  KeSemaphore.cpp
//
//  Created by Shogun3D on 12/8/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KeSemaphore.h"



KeSemaphore::KeSemaphore() : semaphore_ptr(NULL), semaphore(-1), named(No), valid(No), last_error(EINVAL)
{
    
}

KeSemaphore::KeSemaphore( bool shared, uint32_t value ) : semaphore_ptr(NULL), semaphore(-1), named(No), valid(No), last_error(EINVAL)
{
    int ret = sem_init( &semaphore, shared ? Yes : No, value );
    if( ret != -1 )
        valid = Yes;
    
    last_error = errno;
}

KeSemaphore::KeSemaphore( const char* name, uint32_t value ) : semaphore_ptr(NULL), semaphore(-1), named(Yes), valid(No), last_error(EINVAL)
{
    semaphore_ptr = sem_open( name, O_CREAT | O_EXCL, 0644, value );
    if( semaphore_ptr != SEM_FAILED )
    {
        valid = Yes;
        memcpy( semaphore_name, name, strlen( name ) );
    }
    
    last_error = errno;
}

bool KeSemaphore::Create( bool shared, uint32_t value )
{
    int ret = sem_init( &semaphore, shared ? Yes : No, value );
    if( ret != -1 )
        valid = Yes;
    
    last_error = errno;
    
    return valid;
}

bool KeSemaphore::Open( const char* name, uint32_t value )
{
    semaphore_ptr = sem_open( name, O_CREAT | O_EXCL, 0644, value );
    if( semaphore_ptr != SEM_FAILED )
    {
        valid = Yes;
        memcpy( semaphore_name, name, strlen( name ) );
    }
    
    last_error = errno;
    
    return valid;
}

KeSemaphore::~KeSemaphore()
{
    if( semaphore != -1 )
        sem_destroy( &semaphore );
    
    if( semaphore_ptr != SEM_FAILED )
    {
        sem_unlink( semaphore_name );
        sem_close( semaphore_ptr );
    }
    
    valid = No;
}

bool KeSemaphore::IsValid()
{
    return valid;
}