//
//  ke_capture.cpp
//
//  Created by Shogun3D on 8/21/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "ke_capture.h"
#include "jo_jpeg.h"



void flip_image( unsigned char* data, unsigned int rowsize,unsigned int height)
{
	unsigned int mid = height/2,i,j;
	unsigned char *temprow = (unsigned char*) malloc ( rowsize );
	assert( temprow );
    
	for( i=0, j=(height-1); i<mid; ++i,--j )
	{
		/* copy low row to temp */
		memcpy( temprow           , &data[ i*rowsize ], rowsize );
        
		/* copy high row to low row */
		memcpy( &data[ i*rowsize ], &data[ j*rowsize ], rowsize );
        
		/* copy temp to high row */
		memcpy( &data[ j*rowsize ], temprow           , rowsize );
	}
    
	/* delete allocated data */
	free( temprow );
}

/*
 * Name: ke_save_screenshot_jpg
 * Desc: Saves a screenshot to the screenshot folder in JPEG format.
 */
bool ke_save_screenshot_jpg( ke_renderdevice_t* renderdevice )
{
    /* Takes a screenshot like below, but as a .jpg */
    
    FILE* jpgfile;
    char filename[256];
    unsigned int snapshotnr = 0;
    int bpp = 0;
    ke_renderdevice_desc_t rddesc;
    
    renderdevice->get_device_desc( &rddesc );
    
    int w = rddesc.width;
	int h = rddesc.height;
    
    unsigned char* buffer = new unsigned char[w*h*4];
    memset(buffer,0,w*h*4);
    
    renderdevice->get_framebuffer_region( 0, 0, w, h, 0, &bpp, (void**) &buffer );
    flip_image( buffer, w*4, h );
    
	for(;;)
	{
		snapshotnr++;
        
		sprintf( filename, "../screenshots/snapshot%03d.jpg", snapshotnr );
        
		jpgfile = fopen( filename, "rb" );
		if( jpgfile == NULL ) break;
		fclose( jpgfile );
	}
    
    bool res = jo_write_jpg( filename, buffer, w, h, 4, 90 );
    
    if( !res )
        printf( "game_app_t::take_screenshot_jpg(): Unable to save JPG screenshot!\n" );
    
    delete [] buffer;
    
    return true;
}