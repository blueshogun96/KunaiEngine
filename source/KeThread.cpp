//
//  ke_semaphore.h
//
//  Created by Shogun3D on 12/8/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KeThread.h"

#ifdef __APPLE__
#include <mach/thread_act.h>
#endif


/*
 * Name: KeThread::KeThread
 * Desc: Constructor creating a new POSIX thread
 */
KeThread::KeThread( KeThreadPfn pfn, void* context, bool suspended )
{
#ifndef _WIN32
	/* Create a thread attribute and mutex */
	pthread_attr_init( &thread_attr );
	pthread_mutex_init( &mutex, NULL );

    /* Create suspended thread if desired */
    /* TODO: Support suspended threads for Linux, not just Win32 and OSX. */
#ifdef __APPLE__
	if( suspended )
		pthread_create_suspended_np( &thread, NULL, (void *(*)(void *)) pfn, context );
    else
#endif
        pthread_create( &thread, NULL, (void *(*)(void *)) pfn, context );
#else
	/* Create mutex */
	mutex = CreateMutex( NULL, TRUE, NULL );

	/* Create a thread */
	thread = (HANDLE) _beginthreadex( NULL, 0, pfn, context, suspended ? CREATE_SUSPENDED : 0, &thread_id );
#endif
}


/*
 * Name: KeThread::~KeThread
 * Desc: 
 */
KeThread::~KeThread()
{
#ifndef _WIN32
	/* Kill this thread and mutex */
	pthread_attr_destroy( &thread_attr );
	pthread_mutex_destroy( &mutex );
	pthread_kill( thread, 0 );
	pthread_join( thread, NULL );
#else
	/* Close the thread and mutex */
	CloseHandle( thread );
	CloseHandle( mutex );
#endif
}


/*
 * Name: KeThread::Wait
 * Desc: Wait for the desired amount of time for this thread to finish. 
 *		 If successful, return true.  If the wait was cancelled or failed,
 *	     return false.
 */
bool KeThread::Wait( uint32_t timeout )
{
#ifdef _WIN32
	last_error = WaitForSingleObject( thread, timeout );
	if( last_error != WAIT_OBJECT_0 )
		return false;
#else
    if( timeout == static_cast<uint32_t>(-1) )  /* Block the current thread until this thread is finished */
        pthread_join( thread, NULL );
    else if( timeout == 0 )
    {
        /* Test the thread by sending no signal */
        int ret = pthread_kill( thread, 0 );
        last_error = ret;
        
        /* ESRCH means no valid thread; which means either the thread was not initialized or it completed */
        if( ret == ESRCH )
            return true;
        else if( ret != 0 ) /* Anything else besides 0 is not an expected error */
            return false;
    }
    else
    {
        struct timeval now;
        
        gettimeofday( &now, NULL );
        uint32_t start = now.tv_usec / 1000;
        uint32_t stop = start + timeout;
        
        do
        {
            /* Test the thread by sending no signal */
            int ret = pthread_kill( thread, 0 );
            last_error = ret;
            
            /* ESRCH means no valid thread; which means either the thread was not initialized or it completed */
            if( ret == ESRCH )
                return true;
            else if( ret != 0 ) /* Anything else besides 0 is not an expected error */
                return false;
            
        } while( ( now.tv_usec / 1000 ) < stop );
    }
#endif

	return true;
}


/*
 * Name: KeThread::GetLastError
 * Desc: Return the last error returned by any internal functions.
 */
uint32_t KeThread::GetLastError()
{
	return last_error;
}


/*
 * Name: KeThread::Suspend
 * Desc: Suspends the thread.
 */
void KeThread::Suspend()
{
#ifdef _WIN32
	last_error = SuspendThread( thread );
#elif defined(__APPLE__)
	thread_suspend( pthread_mach_thread_np( thread ) );
#else
    /* TODO: Linux */
    /* http://boinc.berkeley.edu/android-boinc/boinc/lib/susp.cpp */
#endif
} 


/*
 * Name: KeThread::Resume
 * Desc: Resumes the thread.
 */
void KeThread::Resume()
{
#ifdef _WIN32
	last_error = ResumeThread( thread );
#elif defined(__APPLE__)
	pthread_mutex_lock( &mutex );
	thread_resume( pthread_mach_thread_np( thread ) );
	pthread_mutex_unlock( &mutex );
#else
    /* TODO: Linux */
#endif
}
