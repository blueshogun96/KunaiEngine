//
//  ke_system.h
//
//  Created by Shogun3D on 6/2/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke_system__
#define __ke_system__

#include "ke_platform.h"
#include <SDL2/SDL.h>
#include <pthread.h>


//typedef void (*callback)(void*, int) ke_input_callback;

/*
 * System objects
 */
struct ke_thread_t;
struct ke_mutex_t;
struct ke_semaphore_t;
struct ke_critical_section_t;


/*
 * Application lifetime
 */
void ke_process_events();
bool ke_quit_requested();
void ke_set_initialize_callback(void (*callback)(void*));
void ke_set_uninitialize_callback(void (*callback)(void*));
void ke_set_keyboard_callback(void (*callback)(void*, int));
void ke_set_mouse_callback(void (*callback)(void*, int));
void ke_set_gamepad_callback(void (*callback)(void*, int));

void ke_on_initialize( void* context );
void ke_on_uninitialize( void* context );
void ke_on_keyboard( void* context, int down );
void ke_on_mouse( void* context, int down );
void ke_on_gamepad( void* context, int down );


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

#endif /* defined(__ke_system__) */