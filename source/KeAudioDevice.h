//
//  ke_audiodevice.h
//
//  Created by Shogun3D on 5/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke_audiodevice__
#define __ke_audiodevice__

#include "KePlatform.h"
#include "KeUnknown.h"
#include <Nv/NvMath.h>

/*
 * Audio device types
 */
#define KE_AUDIODEVICE_OPENAL   1
#define KE_AUDIODEVICE_XAUDIO2  2
#define KE_AUDIODEVICE_DSOUND   3


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
struct KeAudioDeviceDesc
{
    uint32_t    device_type;
    uint32_t    aux_sends;
    uint32_t    flags;
};

/*
 * Audio format structure
 */
/*struct KeAudioFormat
{
    uint32_t    wave_format;
    uint16_t    channels;
    uint16_t    format;
    uint32_t    block_size;
    uint32_t    bits_per_second;
    int         frequency;
    uint32_t    data_size;
};*/

/* 
 * 3D sound effect structure 
 */
struct IKeSoundBuffer : public IKeUnknown
{
    virtual void Destroy() PURE;
    
    virtual bool SetBufferData( void* buffer_data, uint32_t buffer_size ) PURE;
    virtual bool Play( bool looping ) PURE;
    virtual void Stop() PURE;
    virtual void Pause() PURE;
    
    virtual void SetPosition( nv::vec3f& position ) PURE;
    virtual void SetVelocity( nv::vec3f& velocity ) PURE;
    virtual void SetVolume( float volume ) PURE;
    virtual void SetPitch( float pitch ) PURE;

	WAVEFORMATEX wfx;
};


/* 
 * Sound stream structure 
 */
struct IKeSoundStream : public IKeUnknown
{
    virtual void Destroy() PURE;
};


/*
 * Audio effect structure
 */
struct IKeAudioEffect : public IKeUnknown
{
    virtual void Destroy() PURE;
};


/*
 * Audio device base class
 */
class IKeAudioDevice
{
public:
    IKeAudioDevice() {};
    IKeAudioDevice( KeAudioDeviceDesc* audiodevice_desc ) {};
    virtual ~IKeAudioDevice() {};
    
public:
    virtual bool ConfirmDevice() PURE;
    virtual bool CreateSoundBuffer( WAVEFORMATEX* wfx, IKeSoundBuffer** sound_buffer ) PURE;
    
    virtual void SetListenerPosition( nv::vec3f& position ) PURE;
    virtual void SetListenerVelocity( nv::vec3f& velocity ) PURE;
    virtual void SetListenerOrientation( nv::vec3f& at, nv::vec3f& up ) PURE;

protected:
    bool initialized;
    nv::vec3f listener_position;
    nv::vec3f listener_velocity;
    nv::vec3f listener_orientation_at;
    nv::vec3f listener_orientation_up;
};


IKeAudioDevice* KeCreateAudioDevice( KeAudioDeviceDesc* audiodevice_desc );

#endif /* defined(__ke_audiodevice__) */
