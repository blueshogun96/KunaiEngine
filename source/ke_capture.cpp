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

bool ke_save_screenshot_bmp( ke_renderdevice_t* renderdevice )
{
    FILE* bmpfile;
	char filename[256];
	unsigned char header[0x36];
	long size;
	unsigned char line[1024*2*3];
	int w,h;
	short i,j;
	unsigned char empty[2] = {0,0};
	unsigned int color;
	unsigned int snapshotnr = 0;
    int bpp = 0;
    
    ke_renderdevice_desc_t rddesc;
    
    renderdevice->get_device_desc( &rddesc );
    
    w = rddesc.width;
	h = rddesc.height;
    
	size = w * h * 3 + 0x38;
    
    unsigned char* buffer = new unsigned char[w*h*4];
    memset(buffer,0,w*h*4);
    
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
    
    renderdevice->get_framebuffer_region( 0, 0, w, h, 0, &bpp, (void**) &buffer );
    
	for(;;)
	{
		snapshotnr++;
        
		sprintf( filename, "data/screenshots/snapshot%03d.bmp", snapshotnr );
        
		bmpfile = fopen( filename, "rb" );
		if( bmpfile == NULL ) break;
		fclose( bmpfile );
	}
    
	if( ( bmpfile = fopen( filename, "wb" ) ) == NULL )
    {
        delete [] buffer;
		return false;
    }
    
	fwrite( header, 0x36, 1, bmpfile );
	for( i = 0; i < h; i++ )
	{
		for( j = 0; j < w; j++ )
		{
			color = buffer[j+(i*w)];
			line[j*3+2] = ( color ) & 0xff;
			line[j*3+1] = ( color >> 8 ) & 0xff;
			line[j*3+0] = ( color >> 16 ) & 0xff;
		}
		fwrite( line, w * 3, 1, bmpfile );
	}
    
	fwrite( empty, 0x2, 1, bmpfile );
	fclose( bmpfile );
    delete [] buffer;
    
	return true;
}