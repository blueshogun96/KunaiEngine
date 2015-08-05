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
 * Leap Motion callback IDs/types
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

/* NULL Callback function */
void KeLeapMotionCallbackNull( const Leap::Controller& controller ) {}

/* Callback functions; set them all to NULL initially. */
KeLeapMotionCallback pfnKeLeapCallbacks[10] =
{
    KeLeapMotionCallbackNull, KeLeapMotionCallbackNull, KeLeapMotionCallbackNull, KeLeapMotionCallbackNull,
    KeLeapMotionCallbackNull, KeLeapMotionCallbackNull, KeLeapMotionCallbackNull, KeLeapMotionCallbackNull,
    KeLeapMotionCallbackNull, KeLeapMotionCallbackNull
};

/* Leap motion controller */
Leap::Controller LeapMotionController;

/* Has the listener been added? */
bool ListenerInitialized = No;


/*
 * Leap Motion Listener class
 */
class KeLeapMotionListener : public Leap::Listener
{
    void onInit( const Leap::Controller& controller )
    {
        DISPDBG( KE_DBGLVL(0), "...\n" );
        pfnKeLeapCallbacks[KE_LEAP_CALLBACK_INIT]( controller );
    }

    void onConnect( const Leap::Controller& controller )
    {
        DISPDBG( KE_DBGLVL(0), "...\n" );
        pfnKeLeapCallbacks[KE_LEAP_CALLBACK_CONNECT]( controller );
    }

    void onDisconnect( const Leap::Controller& controller )
    {
        DISPDBG( KE_DBGLVL(0), "...\n" );
        pfnKeLeapCallbacks[KE_LEAP_CALLBACK_DISCONNECT]( controller );
    }

    void onExit( const Leap::Controller& controller )
    {
        DISPDBG( KE_DBGLVL(0), "...\n" );
        pfnKeLeapCallbacks[KE_LEAP_CALLBACK_EXIT]( controller );
    }

    void onFrame( const Leap::Controller& controller )
    {
        //DISPDBG( KE_DBGLVL(0), "..." );
        pfnKeLeapCallbacks[KE_LEAP_CALLBACK_FRAME]( controller );
    }

    void onFocusGained( const Leap::Controller& controller )
    {
        DISPDBG( KE_DBGLVL(0), "...\n" );
        pfnKeLeapCallbacks[KE_LEAP_CALLBACK_FOCUSGAINED]( controller );
    }

    void onFocusLost( const Leap::Controller& controller )
    {
        DISPDBG( KE_DBGLVL(0), "...\n" );
        pfnKeLeapCallbacks[KE_LEAP_CALLBACK_FOCUSLOST]( controller );
    }
    
    void onDeviceChange( const Leap::Controller& controller )
    {
        DISPDBG( KE_DBGLVL(0), "...\n" );
        pfnKeLeapCallbacks[KE_LEAP_CALLBACK_DEVICECHANGE]( controller );
    }
    
    void onServiceConnect( const Leap::Controller& controller )
    {
        DISPDBG( KE_DBGLVL(0), "...\n" );
        pfnKeLeapCallbacks[KE_LEAP_CALLBACK_SERVICECONNECT]( controller );
    }
    
    void onServiceDisconnect( const Leap::Controller& controller )
    {
        DISPDBG( KE_DBGLVL(0), "...\n" );
        pfnKeLeapCallbacks[KE_LEAP_CALLBACK_SERVICEDISCONNECT]( controller );
    }
} LeapMotionListener;


/* 
 * Leap Motion callback set functions
 */

void KeLeapMotionSetInitCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_INIT] = ( pfn == NULL || pfn == nullptr ) ? KeLeapMotionCallbackNull : pfn;
}

void KeLeapMotionSetExitCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_EXIT] = ( pfn == NULL || pfn == nullptr ) ? KeLeapMotionCallbackNull : pfn;
}

void KeLeapMotionSetConnectCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_CONNECT] = ( pfn == NULL || pfn == nullptr ) ? KeLeapMotionCallbackNull : pfn;
}

void KeLeapMotionSetDisconnectCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_DISCONNECT] = ( pfn == NULL || pfn == nullptr ) ? KeLeapMotionCallbackNull : pfn;
}

void KeLeapMotionSetFrameCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_FRAME] = ( pfn == NULL || pfn == nullptr ) ? KeLeapMotionCallbackNull : pfn;
}

void KeLeapMotionSetFocusGainedCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_FOCUSGAINED] = ( pfn == NULL || pfn == nullptr ) ? KeLeapMotionCallbackNull : pfn;
}

void KeLeapMotionSetFocusLostCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_FOCUSLOST] = ( pfn == NULL || pfn == nullptr ) ? KeLeapMotionCallbackNull : pfn;
}

void KeLeapMotionSetDeviceChangeCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_DEVICECHANGE] = ( pfn == NULL || pfn == nullptr ) ? KeLeapMotionCallbackNull : pfn;
}

void KeLeapMotionSetServiceConnectCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_SERVICECONNECT] = ( pfn == NULL || pfn == nullptr ) ? KeLeapMotionCallbackNull : pfn;
}

void KeLeapMotionSetServiceDisconnectCallback( KeLeapMotionCallback pfn )
{
    pfnKeLeapCallbacks[KE_LEAP_CALLBACK_DISCONNECT] = ( pfn == NULL || pfn == nullptr ) ? KeLeapMotionCallbackNull : pfn;
}


/*
 * Name: KeLeapMotionInitializeListener
 * Desc: Adds an instance of the listener class above to the leap motion controller.
 * NOTE: You only need to call this function once during your game's lifetime...
 */
bool KeLeapMotionInitializeListener( bool ProcessBackgroundFrames )
{
    /* Return false if the listener was already added */
    if( ListenerInitialized )
    {
        DISPDBG( KE_WARNING, "Listener has already been initialized!" );
        return false;
    }
    
    /* Add the engine defined listener class instance to the queue */
    if( !LeapMotionController.addListener( LeapMotionListener ) )
    {
        DISPDBG( KE_ERROR, "Failed to add listener to the controller!" );
        return false;
    }
    
    /* Process background frames if desired */
    if( ProcessBackgroundFrames )
        LeapMotionController.setPolicyFlags( Leap::Controller::POLICY_BACKGROUND_FRAMES );
    
    ListenerInitialized = Yes;
    
    return true;
}

/*
 * Name: KeLeapMotionUnitializeListener
 * Desc: Removes the listener from the leap motion controller that was previously set.
 */
void KeLeapMotionUninitializeListener()
{
    /* Remove our engine defined listener from the queue */
    LeapMotionController.removeListener( LeapMotionListener );
    
    ListenerInitialized = No;
}

/*
 * Name: KeLeapMotionDeviceConnected
 * Desc: Returns true if a leap motion device is attached and detected.
 */
bool KeLeapMotionDeviceConnected()
{
    return LeapMotionController.isConnected();
}

/*
 * Name: KeLeapMotionGetController
 * Desc: Returns a reference to the leap motion controller handled by the engine.
 */
Leap::Controller& KeLeapMotionGetController()
{
    return LeapMotionController;
}

