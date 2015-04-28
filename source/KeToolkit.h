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
 * Desc: Initialize image loading library via SDL_image.
 */
bool KeImageInitialize();

/*
 * Name: KeImageUninitialize
 * Desc: Uninitialize SDL_image or whatever applicable image loading component.
 */
void KeImageUninitialize();

/*
 * Name: KeImageRead
 * Desc: Opens an image from disc
 */
bool KeImageRead( char* image_path, KeImage* image_out );

/*
 * Name: KeImageReadFromMemory
 * Desc: Opens an image from a file in memory.
 */
bool KeImageReadFromMemory( void* image_file_ptr, KeImage* image_out );

/*
 * Name: KeImageClose
 * Desc: Closes a previously opened image.
 */
void KeImageClose( KeImage* image );

/*
 * Name: KeImageSavePNG
 * Desc: Saves a pixel buffer as a .png file
 */
bool KeImageSavePNG( int width, int height, void* pixels, char* image_path );

/*
 * Name: KeImageSaveJPG
 * Desc: Saves a pixel buffer as a .jpg file
 */
bool KeImageSaveJPG( int width, int height, void* pixels, char* image_path );

/*
 * Name: KeImageSaveBMP
 * Desc: Saves a pixel buffer as a .bmp file
 */
bool KeImageSaveBMP( int width, int height, void* pixels, char* image_path );

#endif /* defined(__Aquatic__KeToolkit__) */
