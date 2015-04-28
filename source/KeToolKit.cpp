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
bool KeImageRead( char* image_path, KeImage* image_out )
{
    SDL_Surface* surface = NULL;
    
	/* Open this image and save the details */
	surface = IMG_Load( image_path );    
	if( !surface )
		return false;


}

/*
 * Name: KeImageClose
 * Desc: Closes a previously opened image.
 */
void KeImageClose( KeImage* image )
{
}

bool KeImageReadFromMemory( void* image_file_ptr, KeImage* image_out )
{
    
}

bool KeImageSavePNG( int width, int height, void* pixels, char* image_path )
{
    
}

bool KeImageSaveJPG( int width, int height, void* pixels, char* image_path )
{
    
}

bool KeImageSaveBMP( int width, int height, void* pixels, char* image_path )
{
    /* TODO */
    return false;
}