//
//  KeToolkit.cpp
//
//  Created by Shogun3D on 4/27/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "KeToolkit.h"

/* SDL2_image */
#ifdef __APPLE__
#include <SDL2_image/SDL_image.h>
#else
#include <SDL_image.h>
#endif

/* JoJpeg library for saving JPEGs */
#include "jo_jpeg.h"

/* Ripped this from PC headers */
/* Used to deal with .wav files */
#ifndef _WIN32
typedef struct {
    uint16_t  wFormatTag;
    uint16_t  nChannels;
    uint32_t nSamplesPerSec;
    uint32_t nAvgBytesPerSec;
    uint16_t  nBlockAlign;
} WAVEFORMAT;

typedef struct {
    uint16_t  wFormatTag;
    uint16_t  nChannels;
    uint32_t nSamplesPerSec;
    uint32_t nAvgBytesPerSec;
    uint16_t  nBlockAlign;
    uint16_t  wBitsPerSample;
    uint16_t  cbSize;
} WAVEFORMATEX;

#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
((uint32_t)(uint8_t)(ch0) | ((uint32_t)(uint8_t)(ch1) << 8) |   \
((uint32_t)(uint8_t)(ch2) << 16) | ((uint32_t)(uint8_t)(ch3) << 24 ))
#define mmioFOURCC MAKEFOURCC
#endif


/*
 * Name: UnpackWAVData
 * Desc: A .wav parsing function I wrote years ago based off of digiben's old 
 *       DirectSound code.  Don't judge a function by it's age.  And no, I'm NOT sorry
 *       about the Hungarian notation either!  Szeretem Magyarországot!
 */
int UnpackWAVData( void* pRIFFBytes, WAVEFORMATEX** ppwfmx, uint8_t** ppChunkData, uint32_t* pCkSize )
{
    uint32_t* pdwChunkBitsPtr;	// Current data being referenced
    uint32_t* pdwChunkTailPtr;	// Points to the end of the chunk
    uint32_t dwChunkID;			// 4 byte chunk ID
    uint32_t dwType;				// Form type
    uint32_t dwLength;				// Size of data in the chunk
    
    // Initialize the WAVEFORMATEX structure
    if( ppwfmx )
        *ppwfmx = NULL;
    
    // Initialize the chunk data pointer
    if( ppChunkData )
        *ppChunkData = NULL;
    
    // Initialize the chunk size pointer
    if( pCkSize )
        *pCkSize = 0;
    
    // Reference the WAVE resource buffer
    pdwChunkBitsPtr = (uint32_t*) pRIFFBytes;
    // Unpack the chunk ID
    dwChunkID = *pdwChunkBitsPtr++;
    // Unpack the size field
    dwLength = *pdwChunkBitsPtr++;
    // Unpack the form field
    dwType = *pdwChunkBitsPtr++;
    
    // Read the 4 byte identifier (FOURCC)
    if( dwChunkID != mmioFOURCC( 'R', 'I', 'F', 'F' ) )
        return No;	// Not a RIFF
    
    // Is this a wave file?
    if( dwType != mmioFOURCC( 'W', 'A', 'V', 'E' ) )
        return No;	// Not a WAV
    
    // Set the tail to point to the end of the wav file
    pdwChunkTailPtr = (uint32_t*) ((uint8_t*) pdwChunkBitsPtr + dwLength - 4 );
    
    while( Yes )
    {
        // Unpack the form type
        dwType = *pdwChunkBitsPtr++;
        
        // Unpack the size
        dwLength = *pdwChunkBitsPtr++;
        
        switch( dwType )
        {
            case mmioFOURCC( 'f', 'm', 't', ' ' ):
                if( ppwfmx && !*ppwfmx )
                {
                    if( dwLength < sizeof( WAVEFORMAT ) )
                        return No;	// Still not a WAV
                    
                    *ppwfmx = (WAVEFORMATEX*) pdwChunkBitsPtr;
                    
                    if( (!ppChunkData || *ppChunkData ) && (!pCkSize || *pCkSize ) )
                        return Yes;
                }
                break;
                
            case mmioFOURCC( 'd', 'a', 't', 'a' ):
                if( ( ppChunkData && !*ppChunkData ) || ( pCkSize && !*pCkSize ) )
                {
                    if( ppChunkData )
                        *ppChunkData = (uint8_t*) pdwChunkBitsPtr++;
                    
                    if( pCkSize )
                        *pCkSize = dwLength;
                    
                    if( !ppwfmx || *ppwfmx )
                        return Yes;
                }
                break;
        }
        
        pdwChunkBitsPtr = (uint32_t*) ((uint8_t*) pdwChunkBitsPtr + ((dwLength+1) & ~1) );
        
        if( pdwChunkBitsPtr >= pdwChunkTailPtr )
            break;
    }
    
    return No;
}


/*
 * Name: KeImageInitialize
 * Desc: Initialize image loading library via SDL_image.
 */
bool KeImageInitialize()
{
    return IMG_Init( IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP );
}

/*
 * Name: KeImageUninitialize
 * Desc: Uninitialize SDL_image or whatever applicable image loading component.
 */
void KeImageUninitialize()
{
    IMG_Quit();
}

/*
 * Name: KeImageRead
 * Desc: Opens an image from disc
 */
bool KeImageRead( char* image_path, KeImageData* image_out )
{
    SDL_Surface* surface = NULL;
    
	/* Open this image and save the details */
	surface = IMG_Load( image_path );    
	if( !surface )
		return false;

    image_out->pixels = new uint8_t[surface->w*surface->h*(surface->format->BytesPerPixel)];
    image_out->width = surface->w;
    image_out->height = surface->h;
    image_out->bpp = surface->format->BitsPerPixel;
    //image_out->palette = surface->format->palette; TODO
    
    return true;
}

bool KeImageReadFromMemory( void* image_file_ptr, KeImageData* image_out )
{
    SDL_RWops* rwop = NULL;
    SDL_Surface* surf = NULL;
    
    return false;   /* TODO */
}

/*
 * Name: KeImageClose
 * Desc: Closes a previously opened image.
 */
void KeImageClose( KeImageData* image )
{
    if( image )
        if( image->pixels )
            delete [] (uint8_t*) image->pixels;
}

bool KeImageSavePNG( int width, int height, void* pixels, char* image_path )
{
    return false;   /* TODO */
}

bool KeImageSaveJPG( int width, int height, void* pixels, char* image_path )
{
    return false;   /* TODO */
}

bool KeImageSaveBMP( int width, int height, void* pixels, char* image_path )
{
    /* TODO */
    return false;
}

/*
 * Name: KeSoundReadWAV
 * Desc: Reads a .wav file from disk and returns the sound bytes along with the
 *       necessary attributes.
 */
bool KeSoundReadWAV( char* wav_path, KeSoundData* sound_data )
{
    return false;
}

/*
 * Name: KeSoundReadWAVFromMemory
 * Desc: Same as above, but reading a .wav file from memory
 */
bool KeSoundReadWAVFromMemory( void* wav_file_ptr, KeSoundData* sound_data )
{
    return false;
}

/*
 * Name: KeSoundClose
 * Desc: Closes a sound file opened by an above function.
 */
void KeSoundClose( KeSoundData* sound_data )
{
    
}
