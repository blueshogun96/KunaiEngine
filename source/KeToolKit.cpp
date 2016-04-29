//
//  KeToolkit.cpp
//
//  Created by Shogun3D on 4/27/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "KeToolkit.h"

/* SDL2_image */
#if defined(__APPLE__) && !defined(__MOBILE_OS__)
#include <SDL2_image/SDL_image.h>
#else
#include <SDL_image.h>
#endif

/* JoJpeg library for saving JPEGs */
#include "jo_jpeg.h"



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
    int ret = IMG_Init( IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP );
	if( !ret )
		DISPDBG( KE_ERROR, "Error initializing SDL_Image library.\nReason: " << IMG_GetError() << std::endl );

	return ret;
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
    
    memmove( image_out->pixels, surface->pixels, surface->w*surface->h*(surface->format->BytesPerPixel) );
    
    SDL_FreeSurface( surface );
    
    return true;
}

bool KeImageReadFromMemory( void* image_file_ptr, uint32_t size, KeImageData* image_out )
{
    SDL_RWops* rwop = NULL;
    SDL_Surface* surface = NULL;
    
    /* Create a RW op from our memory pointer */
    rwop = SDL_RWFromConstMem( image_file_ptr, size );
    if( rwop )
    {
        /* Get the surface from our RW op */
        surface = IMG_Load_RW( rwop, No );
        SDL_FreeRW( rwop );
        
        if( !surface )
		{
			DISPDBG( KE_ERROR, "Error processing this image buffer.\nReason: " << IMG_GetError() );
            return false;
		}
        
        image_out->pixels = new uint8_t[surface->w*surface->h*(surface->format->BytesPerPixel)];
        image_out->width = surface->w;
        image_out->height = surface->h;
        image_out->bpp = surface->format->BitsPerPixel;
        //image_out->palette = surface->format->palette; TODO
        
        memmove( image_out->pixels, surface->pixels, surface->w*surface->h*(surface->format->BytesPerPixel) );
        
        SDL_FreeSurface( surface );
        
        return true;
    }
    
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
    if( !jo_write_jpg( image_path, pixels, width, height, 4, 4 ) )
        DISPDBG( KE_WARNING, "Failed to save JPG image!" );
    
    return true;
}

bool KeImageSaveBMP( int width, int height, void* pixels, char* image_path )
{
    FILE* bmpfile;
    unsigned char header[0x36];
    long size;
    unsigned char line[1024*2*3];
    int w = width, h = height;
    short i,j;
    unsigned char empty[2] = {0,0};
    unsigned int color;
    
    size = w * h * 3 + 0x38;
    
    memset( header, 0, 0x36 );
    header[0] = 'B';
    header[1] = 'M';
    header[2] = size & 0xff;
    header[3] = ( size >> 8 ) & 0xff;
    header[4] = ( size >> 16 ) & 0xff;
    header[5] = ( size >> 24 ) & 0xff;
    header[0x0a] = 0x36;
    header[0x0e] = 0x28;
    header[0x12] = w % 256;
    header[0x13] = w / 256;
    header[0x16] = h % 256;
    header[0x17] = h / 256;
    header[0x1a] = 0x01;
    header[0x1c] = 0x18;
    header[0x26] = 0x12;
    header[0x27] = 0x0b;
    header[0x2a] = 0x12;
    
    if( ( bmpfile = fopen( image_path, "wb" ) ) == NULL )
    {
        DISPDBG( KE_WARNING, "Could not open directory to save BMP image!" );
        return false;
    }
    
    fwrite( header, 0x36, 1, bmpfile );
    for( i = 0; i < h; i++ )
    {
        for( j = 0; j < w; j++ )
        {
            color = ((uint8_t*)pixels)[j+(i*w)];
            line[j*3+2] = ( color ) & 0xff;
            line[j*3+1] = ( color >> 8 ) & 0xff;
            line[j*3+0] = ( color >> 16 ) & 0xff;
        }
        fwrite( line, w * 3, 1, bmpfile );
    }
    
    fwrite( empty, 0x2, 1, bmpfile );
    fclose( bmpfile );
    
    return true;
}

/*
 * Name: KeSoundReadWAV
 * Desc: Reads a .wav file from disk and returns the sound bytes along with the
 *       necessary attributes.
 */
bool KeSoundReadWAV( char* wav_path, KeSoundData* sound_data )
{
    FILE* fp = fopen( wav_path, "rb" );

    if( !fp )
    {
        return false;
    }
    
    fseek( fp, 0, SEEK_END );
    size_t fsize = ftell(fp);
    
    std::unique_ptr<uint8_t> riff( new uint8_t[fsize] );
    
    fseek( fp, 0, SEEK_SET );
    fread( sound_data->ptr, 1, fsize, fp );
    
    WAVEFORMATEX* wfx;
    uint8_t* chunk_data = NULL;
    if( UnpackWAVData( riff.get(), &wfx, (uint8_t**) &chunk_data, &sound_data->bytes ) )
    {
        sound_data->ptr = malloc( sound_data->bytes );
        sound_data->frequency = wfx->nAvgBytesPerSec;
        sound_data->bit_rate = wfx->wBitsPerSample;
        
        return true;
    }
    
    return false;
}

/*
 * Name: KeSoundReadWAVFromMemory
 * Desc: Same as above, but reading a .wav file from memory
 */
bool KeSoundReadWAVFromMemory( void* wav_file_ptr, KeSoundData* sound_data )
{
    WAVEFORMATEX* wfx;
    uint8_t* chunk_data = NULL;
    if( UnpackWAVData( wav_file_ptr, &wfx, (uint8_t**) &chunk_data, &sound_data->bytes ) )
    {
        sound_data->ptr = malloc( sound_data->bytes );
        sound_data->frequency = wfx->nAvgBytesPerSec;
        sound_data->bit_rate = wfx->wBitsPerSample;
        
        return true;
    }
    
    return false;
}

/*
 * Name: KeSoundClose
 * Desc: Closes a sound file opened by an above function.
 */
void KeSoundClose( KeSoundData* sound_data )
{
    if( sound_data )
        if( sound_data->ptr )
            free( sound_data->ptr );
}
