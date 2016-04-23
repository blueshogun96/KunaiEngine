//
//  KeXAudio2AudioDevice.h
//
//  Created by Shogun3D on 6/28/15.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke_xa2_audiodevice__
#define __ke_xa2_audiodevice__

#include <xaudio2.h>
#include <xaudio2fx.h>
#include <comip.h>
#include <comdef.h>
#include "KeAudioDevice.h"



/*
* Non-ATL based smart COM pointer types
*/
typedef _com_ptr_t<_com_IIID<IXAudio2, &__uuidof(IXAudio2)>>			CXAudio2;



/* 
 * Sound buffer structure
 */
struct IKeXAudio2SoundBuffer : public IKeSoundBuffer
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

	float					volume;
	float					pitch;
    nv::vec3f				position;
    nv::vec3f				velocity;
	IXAudio2SourceVoice*	subvoice;
	uint8_t*				buffer_data;
	int						buffer_size;
};

/*
 * Sound stream structure
 */
struct IKeXAudio2SoundStream : public IKeSoundStream
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
struct IKeXAudio2AudioEffect : public IKeAudioEffect
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
class IKeXAudio2AudioDevice : public IKeAudioDevice
{
public:
    IKeXAudio2AudioDevice();
    IKeXAudio2AudioDevice( KeAudioDeviceDesc* audiodevice_desc );
    virtual ~IKeXAudio2AudioDevice();
    
public:
    virtual bool ConfirmDevice();
    virtual bool CreateSoundBuffer( WAVEFORMATEX* wfx, IKeSoundBuffer** sound_buffer );

    virtual void SetListenerPosition( nv::vec3f& position );
    virtual void SetListenerVelocity( nv::vec3f& velocity );
    virtual void SetListenerOrientation( nv::vec3f& at, nv::vec3f& up );
    
private:
	bool initialized;

	CXAudio2				xa2;
	IXAudio2MasteringVoice* mv;
};

#endif /* defined(__ke_xa2_audiodevice__) */
