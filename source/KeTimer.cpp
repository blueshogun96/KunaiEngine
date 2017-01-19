//
//  KeTimer.cpp
//
//  Created by Shogun3D on 1/16/17.
//  Copyright © 2017 Shogun3D. All rights reserved.
//

#include <Ke.h>
#include <KeSystem.h>
#include <KeTimer.h>

#ifndef _WIN32  /* Non-Windows based includes */
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#endif


/*
 * Timer handle
 */
struct KeTimer
{
#ifdef _WIN32
#endif
    
#ifdef __APPLE__
#else
    timer_t timerid;
#endif
    
    uint32_t flags;
};

bool KeInitializeTimer( const char* timer_name, uint32_t flags, KeTimer* timer )
{
#ifdef _WIN32
#endif
    
#ifdef __APPLE__
#else
    /* Is this a periodic timer? */
    if( flags & KE_TIMER_PERIODIC )
    {
        pthread_attr_t attr;
        pthread_attr_init( &attr );
        
        struct sched_param param;
        param.sched_priority = 255;
        pthread_attr_setschedparam( &attr, &param );
        
        struct sigevent sig;
        sig.sigev_notify = SIGEV_THREAD;
        sig.sigev_notify_function = callback;   /* TODO */
        sig.sigev_value.sival_ptr = value;
    }
#endif
    
    return true;
}
