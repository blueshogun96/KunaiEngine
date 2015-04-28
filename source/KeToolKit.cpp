//
//  KeToolkit.cpp
//
//  Created by Shogun3D on 4/27/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "KeToolkit.h"

#include <SDL2_image/SDL_image.h>
#include "jo_jpeg.h"



/*
 *
 */
bool KeImageInitialize()
{
    return IMG_Init( IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP );
}

void KeImageUninitialize()
{
    IMG_Quit();
}

bool KeImageRead( char* image_path, KeImage* image_out )
{
    SDL_Surface* surface = NULL;
    
    
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