//
//  KeLeapMotion.h
//  rigidbodies
//
//  Created by Shogun3D on 8/2/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#ifndef __rigidbodies__KeLeapMotion__
#define __rigidbodies__KeLeapMotion__


#include "Ke.h"
#include <Leap.h>
#include <LeapMath.h>

#ifdef __MOBILE_OS__
#error Leap Motion is not supported on mobile platforms at this time
#endif


/*
 * Leap Motion event callback function pointer
 */
typedef void (*KeLeapMotionCallback)( const Leap::Controller&  );

void KeLeapMotionSetInitCallback( KeLeapMotionCallback pfn );
void KeLeapMotionSetExitCallback( KeLeapMotionCallback pfn );
void KeLeapMotionSetConnectCallback( KeLeapMotionCallback pfn );
void KeLeapMotionSetDisconnectCallback( KeLeapMotionCallback pfn );
void KeLeapMotionSetFrameCallback( KeLeapMotionCallback pfn );
void KeLeapMotionSetFocusGainedCallback( KeLeapMotionCallback pfn );
void KeLeapMotionSetFocusLostCallback( KeLeapMotionCallback pfn );
void KeLeapMotionSetDeviceChangeCallback( KeLeapMotionCallback pfn );
void KeLeapMotionSetServiceConnectCallback( KeLeapMotionCallback pfn );
void KeLeapMotionSetServiceDisconnectCallback( KeLeapMotionCallback pfn );


/*
 * Leap Motion functionality
 */
bool KeLeapMotionInitializeListener( bool ProcessBackgroundFrames );
void KeLeapMotionUninitializeListener();
bool KeLeapMotionDeviceConnected();
Leap::Controller& KeLeapMotionGetController();


#endif /* defined(__rigidbodies__KeLeapMotion__) */
