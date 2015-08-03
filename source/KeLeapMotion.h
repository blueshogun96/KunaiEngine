//
//  KeLeapMotion.h
//  rigidbodies
//
//  Created by Shogun3D on 8/2/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#ifndef __rigidbodies__KeLeapMotion__
#define __rigidbodies__KeLeapMotion__


#include <iostream>
#include "Leap.h"
#include "LeapMath.h"


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


bool KeLeapMotionInitializeListener( bool ProcessBackgroundFrames );
void KeLeapMotionUninitializeListener();
bool KeLeapMotionDeviceConnected();
bool KeLeapMotionHandInRange();

#if 0
/*
 * Leap Motion listener class
 */

class leap_motion_listener_t : public Leap::Listener
{
public:
    virtual void onInit( const Leap::Controller& );
    virtual void onConnect( const Leap::Controller& );
    virtual void onDisconnect( const Leap::Controller& );
    virtual void onExit( const Leap::Controller& );
    virtual void onFrame( const Leap::Controller& );
    virtual void onFocusGained( const Leap::Controller& );
    virtual void onFocusLost( const Leap::Controller& );
    /*virtual void onDeviceChange( const Leap::Controller& );
     virtual void onServiceConnect( const Leap::Controller& );
     virtual void onServiceDisconnect( const Leap::Controller& );*/
};

/* Return the position of the index finger of the desired hand */
struct vec3_t<float> leap_motion_get_finger_position( int hand, struct vec2_t<float> window_res );
struct vec3_t<float> leap_motion_get_finger_position_averaged( int hand, struct vec2_t<float> window_res );
/* Set a new listener instance of leap_motion_listener_t */
void leap_motion_initialize_listener( bool background_frames );
/* Remove the current listener */
void leap_motion_uninitialize_listener();
/* Returns true if a leap motion device is connected */
bool leap_motion_device_connected();
/* Returns true if there are any hands in focus */
bool leap_motion_hand_in_range();
#endif

#endif /* defined(__rigidbodies__KeLeapMotion__) */
