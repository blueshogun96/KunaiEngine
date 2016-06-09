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
  /*#include <AL/al.h>
  #include <AL/alc.h>
  #include <AL/alext.h>*/
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
    
    virtual void SetPosition( nv::vec3f& position );
    virtual void SetVelocity( nv::vec3f& velocity );
    virtual void SetVolume( float volume );
    virtual void SetPitch( float pitch );

    uint32_t    buffer;
    uint32_t    source;
	float       volume;
	float       pitch;
    nv::vec3f   position;
    nv::vec3f   velocity;
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
    virtual bool CreateSoundBuffer( WAVEFORMATEX* wfx, IKeSoundBuffer** sound_buffer );
    
    virtual void SetListenerPosition( nv::vec3f& position );
    virtual void SetListenerVelocity( nv::vec3f& velocity );
    virtual void SetListenerOrientation( nv::vec3f& at, nv::vec3f& up );
    
private:
    ALCcontext* context;
    ALCdevice*  device;
    ALint       max_auxiliary_sends;
};

#endif /* defined(__ke_al_audiodevice__) */
