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
	/* Create a thread attribute */
	pthread_attr_init( &thread_attr );

	/* Create suspended thread if desired */
	if( suspended ) 
		pthread_attr_setcreatesuspended_np( &thread_attr );
#else
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
	/* Kill this thread */
	pthread_attr_destroy( &thread_attr );
#else
	CloseHandle( thread );
#endif
}


/*
 * Name: 
 * Desc: 
 */
bool KeThread::Wait( uint32_t timeout )
{
	/* Wait for the desired amount of time for this thread to finish. 
	   If successful, return true.  If the wait was cancelled or failed,
	   return false. */

	last_error = WaitForSingleObject( thread, timeout );
	if( last_error != WAIT_OBJECT_0 )
		return false;

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
