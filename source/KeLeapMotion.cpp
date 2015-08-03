//
//  KeLeapMotion.cpp
//  rigidbodies
//
//  Created by Shogun3D on 8/2/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeLeapMotion.h"


/* 
 * Leap Motion callback IDs 
 */
#define KE_LEAP_CALLBACK_INIT               0
#define KE_LEAP_CALLBACK_EXIT               1
#define KE_LEAP_CALLBACK_CONNECT            2
#define KE_LEAP_CALLBACK_DISCONNECT         3
#define KE_LEAP_CALLBACK_FRAME              4
#define KE_LEAP_CALLBACK_FOCUSGAINED        5
#define KE_LEAP_CALLBACK_FOCUSLOST          6
#define KE_LEAP_CALLBACK_DEVICECHANGE       7
#define KE_LEAP_CALLBACK_SERVICECONNECT     8
#define KE_LEAP_CALLBACK_SERVICEDISCONNECT  9

void KeLeapMotionCallbackNull( const Leap::Controller& controller ) {}

/* Callback functions */
KeLeapMotionCallback pfnKeLeapCallbacks[10] = { KeLeapMotionCallbackNull };

/* Leap motion controller */
Leap::Controller LeapMotionController;


class KeLeapMotionListener : public Leap::Listener
{
    void onInit( const Leap::Controller& controller )
    {
        std::cerr << "onInit()...\n";
        pfnKeLeapCallbacks[KE_LEAP_CALLBACK_INIT]( controller );
    }

    void onConnect( const Leap::Controller& controller )
    {
        std::cerr << "onConnect()...\n";
        pfnKeLeapCallbacks[KE_LEAP_CALLBACK_CONNECT]( controller );
    }

    void onDisconnect( const Leap::Controller& controller )
    {
        std::cerr << "onDisconnect()...\n";
        pfnKeLeapCallbacks[KE_LEAP_CALLBACK_DISCONNECT]( controller );
    }

    void onExit( const Leap::Controller& controller )
    {
        std::cerr << "onExit()...\n";
        pfnKeLeapCallbacks[KE_LEAP_CALLBACK_EXIT]( controller );
    }

    void onFrame( const Leap::Controller& controller )
    {
        //   std::cerr << "onFrame()...\n";
        pfnKeLeapCallbacks[KE_LEAP_CALLBACK_FRAME]( controller );
    }

    void onFocusGained( const Leap::Controller& controller )
    {
        pfnKeLeapCallbacks[KE_LEAP_CALLBACK_FOCUSGAINED]( controller );
    }

    void onFocusLost( const Leap::Controller& controller )
    {
        pfnKeLeapCallbacks[KE_LEAP_CALLBACK_FOCUSLOST]( controller );
    }
} LeapMotionListener;


void KeLeapMotionSetInitCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_INIT] = pfn;
}

void KeLeapMotionSetExitCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_EXIT] = pfn;
}

void KeLeapMotionSetConnectCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_CONNECT] = pfn;
}

void KeLeapMotionSetDisconnectCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_DISCONNECT] = pfn;
}

void KeLeapMotionSetFrameCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_FRAME] = pfn;
}

void KeLeapMotionSetFocusGainedCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_FOCUSGAINED] = pfn;
}

void KeLeapMotionSetFocusLostCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_FOCUSLOST] = pfn;
}

void KeLeapMotionSetDeviceChangeCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_DEVICECHANGE] = pfn;
}

void KeLeapMotionSetServiceConnectCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_SERVICECONNECT] = pfn;
}

void KeLeapMotionSetServiceDisconnectCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_DISCONNECT] = pfn;
}


bool KeLeapMotionInitializeListener( bool ProcessBackgroundFrames )
{
    LeapMotionController.addListener( LeapMotionListener );
    
    return true;
}

void KeLeapMotionUninitializeListener()
{
    LeapMotionController.removeListener( LeapMotionListener );
}

