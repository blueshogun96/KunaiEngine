//
//  ke_audiodevice.h
//
//  Created by Shogun3D on 5/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke_audiodevice__
#define __ke_audiodevice__

#include "ke_platform.h"


/*
 * Audio device types
 */
#define KE_AUDIODEVICE_OPENAL   0
#define KE_AUDIODEVICE_XAUDIO2  1
#define KE_AUDIODEVICE_DSOUND   2


/*
 eaxbathroom.ulEnvironment:=3;
 eaxbathroom.flEnvironmentSize:=1.4;
 eaxbathroom.flEnvironmentDiffusion:=1.000;
 eaxbathroom.lRoom:=-1000;
 eaxbathroom.lRoomHF:=-1200;
 eaxbathroom.lRoomLF:=0;
 eaxbathroom.flDecayTime:=1.49;
 eaxbathroom.flDecayHFRatio:=0.54;
 eaxbathroom.flDecayLFRatio:=1.00;
 eaxbathroom.lReflections:=-370;
 eaxbathroom.flReflectionsDelay:=0.007;
 eaxbathroom.vReflectionsPan.x:=0.0;
 eaxbathroom.vReflectionsPan.y:=0.0;
 eaxbathroom.vReflectionsPan.z:=0.0;
 eaxbathroom.lReverb:=1030;
 eaxbathroom.flReverbDelay:=0.011;
 eaxbathroom.vReverbPan.x:=0.00;
 eaxbathroom.vReverbPan.y:=0.00;
 eaxbathroom.vReverbPan.z:=0.00;
 eaxbathroom.flEchoTime:=0.250;
 eaxbathroom.flEchoDepth:=0.000;
 eaxbathroom.flModulationTime:=0.250;
 eaxbathroom.flModulationDepth:=0.000;
 eaxbathroom.flAirAbsorptionHF:=-5.0;
 eaxbathroom.flHFReference:=5000.0;
 eaxbathroom.flLFReference:=250.0;
 eaxbathroom.flRoomRolloffFactor:=0.00;
 eaxbathroom.ulFlags:=$3f;*/

/*
 * Audio device description
 */
struct ke_audiodevice_desc_t
{
    uint32_t    device_type;
    uint32_t    aux_sends;
    uint32_t    flags;
};

/*
 * Audio format structure
 */
struct ke_audioformat_t
{
    uint32_t    wave_format;
    uint16_t    channels;
    uint16_t    format;
    uint32_t    block_size;
    uint32_t    bits_per_second;
    int         frequency;
    uint32_t    data_size;
};

/* 
 * 3D sound effect structure 
 */
struct ke_soundbuffer_t {};


/* 
 * Sound stream structure 
 */
struct ke_soundstream_t {};


/*
 * Audio effect structure
 */
struct ke_audioeffect_t {};


/*
 * Audio device base class
 */
class ke_audiodevice_t
{
public:
    ke_audiodevice_t() {};
    ke_audiodevice_t( ke_audiodevice_desc_t* audiodevice_desc ) {};
    virtual ~ke_audiodevice_t() {};
    
public:
    virtual bool confirm_device() PURE;
    virtual bool create_sound_buffer( ke_audioformat_t* audio_format, ke_soundbuffer_t** sound_buffer ) PURE;
    virtual void release_sound_buffer( ke_soundbuffer_t* sound_buffer ) PURE;
    virtual bool set_buffer_data( ke_soundbuffer_t* sound_buffer, void* buffer_data, uint32_t buffer_size ) PURE;
    virtual bool play_sound_buffer( ke_soundbuffer_t* sound_buffer, bool looping ) PURE;
    virtual void stop_sound_buffer( ke_soundbuffer_t* sound_buffer ) PURE;
    virtual void pause_sound_buffer( ke_soundbuffer_t* sound_buffer ) PURE;
    
    virtual void set_listener_position( float* position ) PURE;
    virtual void set_listener_velocity( float* velocity ) PURE;
    virtual void set_listener_orientation( float* at, float* up ) PURE;
    
    virtual void set_buffer_position( ke_soundbuffer_t* sound_buffer, float* position ) PURE;
    virtual void set_buffer_velocity( ke_soundbuffer_t* sound_buffer, float* velocity ) PURE;
    virtual void set_buffer_volume( ke_soundbuffer_t* sound_buffer, float volume ) PURE;
    virtual void set_buffer_pitch( ke_soundbuffer_t* sound_buffer, float pitch ) PURE;
    
protected:
    bool initialized;
    float listener_position[3];
    float listener_velocity[3];
    float listener_orientation_at[3];
    float listener_orientation_up[3];
};

#endif /* defined(__ke_audiodevice__) */
