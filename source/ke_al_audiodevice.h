//
//  ke_al_audiodevice.h
//
//  Created by Shogun3D on 5/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke_al_audiodevice__
#define __ke_al_audiodevice__

#ifdef __APPLE__
#ifndef __MOBILE_OS__
#include <OpenAL-Soft/al.h>
#include <OpenAL-Soft/alc.h>
#include <OpenAL-Soft/alext.h>
#else
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <OpenAL/oalMacOSX_OALExtensions.h>
#include <OpenAL/oalStaticBufferExtension.h>
#endif
#else
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#endif
#include "ke_audiodevice.h"



/* 
 * Sound buffer structure
 */
struct ke_al_soundbuffer_t : public ke_soundbuffer_t
{
    uint32_t    buffer;
    uint32_t    source;
	float       volume;
	float       pitch;
    float       position[3];
    float       velocity[3];
    ke_audioformat_t audio_format;
};

/*
 * Sound stream structure
 */
struct ke_al_soundstream_t : public ke_soundstream_t
{
	uint32_t    source;
	uint32_t*   buffers;
    uint32_t    buffer_count;
	uint32_t    frequency;
	uint32_t    format;
    float       volume;
    uint32_t    wave_format;
	int         channels;
	int         bits;
	bool        is_playing;
	unsigned char* buf;
	unsigned char* buf2;
	FILE*       file;
	void*       thread;
};

/*
 * Audio effect structure
 */
struct ke_al_audioeffect_t : public ke_audioeffect_t
{
    uint32_t    environment;
    float       environment_size;
    float       environment_diffusion;
    long        room;
    long        room_hf;
    long        room_lf;
    float       decay_time;
    float       decay_hr_ratio;
    float       decay_lf_ratio;
    long        reflections;
    float       reflections_delay;
    float       reflections_pan[3];
    long        reverb;
    float       reverb_delay;
    float       reverb_pan[3];
    float       echo_time;
    float       echo_depth;
    float       modulation_time;
    float       modulation_depth;
    float       air_absorption_hf;
    float       hf_reference;
    float       lf_reference;
    float       room_rollof_factor;
    uint32_t    flags;
};

/*
 * Audio device base class
 */
class ke_al_audiodevice_t : public ke_audiodevice_t
{
public:
    ke_al_audiodevice_t();
    ke_al_audiodevice_t( ke_audiodevice_desc_t* audiodevice_desc );
    virtual ~ke_al_audiodevice_t();
    
public:
    virtual bool confirm_device();
    virtual bool create_sound_buffer( ke_audioformat_t* audio_format, ke_soundbuffer_t** sound_buffer );
    virtual void release_sound_buffer( ke_soundbuffer_t* sound_buffer );
    virtual bool set_buffer_data( ke_soundbuffer_t* sound_buffer, void* buffer_data, uint32_t buffer_size );
    virtual bool play_sound_buffer( ke_soundbuffer_t* sound_buffer, bool looping );
    virtual void stop_sound_buffer( ke_soundbuffer_t* sound_buffer );
    virtual void pause_sound_buffer( ke_soundbuffer_t* sound_buffer );
    
    virtual void set_listener_position( float* position );
    virtual void set_listener_velocity( float* velocity );
    virtual void set_listener_orientation( float* at, float* up );
    
    void set_buffer_position( ke_soundbuffer_t* sound_buffer, float* position );
    void set_buffer_velocity( ke_soundbuffer_t* sound_buffer, float* velocity );
    void set_buffer_volume( ke_soundbuffer_t* sound_buffer, float volume );
    void set_buffer_pitch( ke_soundbuffer_t* sound_buffer, float pitch );
    
private:
    ALCcontext* context;
    ALCdevice*  device;
    ALint       max_auxiliary_sends;
};

#endif /* defined(__ke_al_audiodevice__) */
