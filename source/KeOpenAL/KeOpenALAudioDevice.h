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
#include "KeAudioDevice.h"



/* 
 * Sound buffer structure
 */
struct IKeOpenALSoundBuffer : public IKeSoundBuffer
{
    virtual void Destroy();
    
    virtual bool SetBufferData( void* buffer_data, uint32_t buffer_size );
    virtual bool Play( bool looping );
    virtual void Stop();
    virtual void Pause();
    
    virtual void SetPosition( float* position );
    virtual void SetVelocity( float* velocity );
    virtual void SetVolume( float volume );
    virtual void SetPitch( float pitch );

    uint32_t    buffer;
    uint32_t    source;
	float       volume;
	float       pitch;
    float       position[3];
    float       velocity[3];
    KeAudioFormat audio_format;
};

/*
 * Sound stream structure
 */
struct IKeOpenALSoundStream : public IKeSoundStream
{
    virtual void Destroy();
    
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
struct IKeOpenALAudioEffect : public IKeAudioEffect
{
    virtual void Destroy();
    
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
class IKeOpenALAudioDevice : public IKeAudioDevice
{
public:
    IKeOpenALAudioDevice();
    IKeOpenALAudioDevice( KeAudioDeviceDesc* audiodevice_desc );
    virtual ~IKeOpenALAudioDevice();
    
public:
    virtual bool ConfirmDevice();
    virtual bool CreateSoundBuffer( KeAudioFormat* audio_format, IKeSoundBuffer** sound_buffer );
    virtual void ReleaseSoundBuffer( IKeSoundBuffer* sound_buffer );
    virtual bool SetBufferData( IKeSoundBuffer* sound_buffer, void* buffer_data, uint32_t buffer_size );
    virtual bool PlaySoundBuffer( IKeSoundBuffer* sound_buffer, bool looping );
    virtual void StopSoundBuffer( IKeSoundBuffer* sound_buffer );
    virtual void PauseSoundBuffer( IKeSoundBuffer* sound_buffer );
    
    virtual void SetListenerPosition( float* position );
    virtual void SetListenerVelocity( float* velocity );
    virtual void SetListenerOrientation( float* at, float* up );
    
    void SetBufferPosition( IKeSoundBuffer* sound_buffer, float* position );
    void SetBufferVelocity( IKeSoundBuffer* sound_buffer, float* velocity );
    void SetBufferVolume( IKeSoundBuffer* sound_buffer, float volume );
    void SetBufferPitch( IKeSoundBuffer* sound_buffer, float pitch );
    
private:
    ALCcontext* context;
    ALCdevice*  device;
    ALint       max_auxiliary_sends;
};

#endif /* defined(__ke_al_audiodevice__) */
