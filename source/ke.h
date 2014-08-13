//
//  ke.h
//
//  Created by Shogun3D on 7/11/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke__
#define __ke__

#include "ke_platform.h"
#include "ke_system.h"
#include "ke_renderdevice.h"
#include "ke_audiodevice.h"

/* Debug output level */
#ifndef KE_DBG_LEVEL
#define KE_DBG_LEVEL 1
#endif

/*
 * Kunai engine basics
 */
bool ke_initialize();
void ke_uninitialize();
bool ke_create_window_and_device( ke_renderdevice_desc_t* device_desc, ke_renderdevice_t** device );
void ke_destroy_window_and_device( ke_renderdevice_t* device );
bool ke_create_audio_device( ke_audiodevice_desc_t* device_desc, ke_audiodevice_t** device );
void ke_destroy_audio_device( ke_audiodevice_t* device );

#endif /* defined(__ke__) */
