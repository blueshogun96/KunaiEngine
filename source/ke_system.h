//
//  ke_system.h
//
//  Created by Shogun3D on 6/2/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke_system__
#define __ke_system__

#include "ke_platform.h"
#if defined(__APPLE__) && !defined(__MOBILE_OS__)
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include <pthread.h>

#include "ke_debug.h"
#include "Framerate.h"


/*
 * System objects
 */
struct ke_thread_t;
struct ke_mutex_t;
struct ke_semaphore_t;
struct ke_critical_section_t;

/*
 * Thread function pointer
 */
typedef int (*ke_thread_pfn)( void* );

/*
 * Input timestamp structure
 */
struct ke_timestamp_t
{
    float start_time;   /* Starting time when this input started */
    float elapsed_time; /* Number of milliseconds since the input */
    int frames;         /* The number of frames since the input */
};

/*
 * Keyboard and gamepad button structure
 */
struct ke_button_t
{
    bool            pressed;
    ke_timestamp_t  timestamp;
};

/*
 * Mouse state structure
 */
struct ke_mouse_t
{
    int x, y;       /* Mouse position */
    int button[3];  /* Mouse buttons 1=Down, 0=Up */
};


/*
 * Application lifetime
 */
void ke_process_events();
bool ke_quit_requested();
void ke_set_initialize_callback(void (*callback)(void*));
void ke_set_uninitialize_callback(void (*callback)(void*));
void ke_set_keyboard_callback(void (*callback)(void*, void*));
void ke_set_mouse_callback(void (*callback)(void*, void*));
void ke_set_gamepad_callback(void (*callback)(void*, void*));

void ke_set_context_pointer( void* context_pointer );
void* ke_get_context_pointer();
void ke_on_initialize( void* context );
void ke_on_uninitialize( void* context );
void ke_on_keyboard( void* context, void* input_context );
void ke_on_mouse( void* context, void* input_context );
void ke_on_gamepad( void* context, void* input_context );

void ke_reset_keys();
void ke_reset_mouse();
void ke_get_key_state( ke_button_t* _keys );
bool ke_got_any_key();
void ke_get_mouse_state( ke_mouse_t* _mouse );


/*
 * Timing
 */
void ke_delay( uint32_t milliseconds );
uint64_t ke_get_performance_counter();
uint64_t ke_get_performance_frequency();
uint32_t ke_get_tick_count();


/*
 * Critical section
 */
bool ke_initialize_critical_section( struct ke_critical_section_t** critical_section );
void ke_uninitialize_critical_section( struct ke_critical_section_t* critical_section );
void ke_enter_critical_section( struct ke_critical_section_t* critical_section );
void ke_leave_critical_section( struct ke_critical_section_t* critical_section );
bool ke_try_enter_critical_section( struct ke_critical_section_t* critical_section );


/*
 * Mutex
 */
bool ke_create_mutex( struct ke_mutex_t** mutex );
void ke_destroy_mutex( struct ke_mutex_t* mutex );
void ke_lock_mutex( struct ke_mutex_t* mutex );
void ke_unlock_mutex( struct ke_mutex_t* mutex );
bool ke_try_lock_mutex( struct ke_mutext* mutex );


/*
 * Semaphore
 */
bool ke_create_semaphore( struct ke_semaphore_t** semaphore );
void ke_destroy_semaphore( struct ke_semaphore_t* semaphore );

/*
 * Thread
 */
bool ke_create_thread( struct ke_thread_t** thread, ke_thread_pfn pfn, const char* thread_name );

/*
 * Misc functionality
 */
void ke_message_box( const char* message, const char* title, uint32_t flags );
void ke_display_last_message( const char* title, uint32_t flags );

#endif /* defined(__ke_system__) */
