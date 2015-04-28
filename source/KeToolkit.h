//
//  KeToolkit.h
//
//  Created by Shogun3D on 4/27/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#ifndef __KeToolkit__
#define __KeToolkit__

#include "Ke.h"
#include "KePlatform.h"


/*
 * Image palette entry (XRGB)
 */
struct KePaletteEntry
{
    uint8_t x, r, g, b;
};

/*
 * Image data structure
 */
struct KeImage
{
    uint32_t width;
    uint32_t height;
    uint32_t bpp;
    
    void* pixels;
    KePaletteEntry palette[256];
};


/*
 * Name: KeImageInitialize
 * Desc: Initialize image loading library via SDL.
 */
bool KeImageInitialize();

/*
 * Name: KeImageUninitialize
 * Desc: 
 */
void KeImageUninitialize();
bool KeImageRead( char* image_path, KeImage* image_out );
bool KeImageReadFromMemory( void* image_file_ptr, KeImage* image_out );
bool KeImageSavePNG( int width, int height, void* pixels, char* image_path );
bool KeImageSaveJPG( int width, int height, void* pixels, char* image_path );
bool KeImageSaveBMP( int width, int height, void* pixels, char* image_path );

#endif /* defined(__Aquatic__KeToolkit__) */
