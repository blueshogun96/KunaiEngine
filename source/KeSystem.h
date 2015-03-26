//
//  ke_system.h
//
//  Created by Shogun3D on 6/2/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke_system__
#define __ke_system__

#include "KePlatform.h"
#if defined(__APPLE__) && !defined(__MOBILE_OS__)
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include <pthread.h>

#include "KeDebug.h"
#include "Framerate.h"



/*
 * Input timestamp structure
 */
struct KeTimeStamp
{
    float start_time;   /* Starting time when this input started */
    float elapsed_time; /* Number of milliseconds since the input */
    int frames;         /* The number of frames since the input */
};

/*
 * Keyboard and gamepad button structure
 */
struct KeButton
{
    bool            pressed;
    KeTimeStamp  timestamp;
};

/*
 * Mouse state structure
 */
struct KeMouse
{
    int x, y;       /* Mouse position */
    int button[3];  /* Mouse buttons 1=Down, 0=Up */
};


/*
 * Application lifetime
 */
void KeProcessEvents();
bool KeQuitRequested();
void KeSetInitializeCallback(void (*callback)(void*));
void KeSetUnitializeCallback(void (*callback)(void*));
void KeSetKeyboardCallback(void (*callback)(void*, void*));
void KeSetMouseCallBack(void (*callback)(void*, void*));
void KeSetGamepadCallback(void (*callback)(void*, void*));

void KeSetContextPointer( void* context_pointer );
void* KeGetContextPointer();
void KeOnInitialize( void* context );
void KeOnUnintialize( void* context );
void KeOnKeyboard( void* context, void* input_context );
void KeOnMouse( void* context, void* input_context );
void KeOnGamepad( void* context, void* input_context );

void KeResetKeys();
void KeResetMouse();
void KeGetKeyState( KeButton* _keys );
bool KeyGotAnyKey();
void KeGetMouseState( KeMouse* _mouse );


/*
 * Timing
 */
void KeDelay( uint32_t milliseconds );
uint64_t KeGetPerformanceCounter();
uint64_t KeGetPerformanceFrequency();
uint32_t KeGetTickCount();


/*
 * Misc functionality
 */
void KeMessageBox( const char* message, const char* title, uint32_t flags );
void KeDisplayLastMessage( const char* title, uint32_t flags );

#endif /* defined(__ke_system__) */
