//
//  ke_semaphore.h
//
//  Created by Shogun3D on 12/8/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KeThread.h"



/*
 * Name: KeThread::KeThread
 * Desc: Constructor creating a new POSIX thread
 */
KeThread::KeThread( KeThreadPfn pfn, void* context = NULL, bool suspended = No )
{
#ifndef _WIN32
	/* Create a thread attribute and mutex */
	pthread_attr_init( &thread_attr );
	pthread_mutex_init( &mutex );

	/* Create suspended thread if desired */
	if( suspended ) 
#ifdef __APPLE__
		pthread_create_suspended_np( &thread_attr );
#endif
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
#endif
} 


/*
 * Name: KeThread::REsume
 * Desc: Resumes the thread.
 */
void KeThread::Resume()
{
#ifdef _WIN32
	last_error = ResumeThread( thread );
#elif defined(__APPLE__)
	pthread_mutex_lock( mutex );
	thread_resume( pthread_mach_thread_np( thread ) );
	pthread_mutex_unlock( mutex );
#else
#endif
}
