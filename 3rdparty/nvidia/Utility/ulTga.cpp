/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulTga.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/




#include <ul.h>
#include "ulGraphicsFile.h"



int ulTarga::bread() 
{
    return fgetc(m.fp);
}

int ulTarga::wread()
{
    byte h, l;
    
    l = (byte) bread();
    h = (byte) bread();
    return (h << 8) | l;
}

/*

  Use this to read the whole tga file, allocate memory and return a pointer to the memory,
  
    reads 24/32 tga -> 32 bpp
    does not support 8 bpp palette
    
      flags
      swap: 1 = swaps rgb position
      pad = pads 24 bpp to 32 bpp (sets alpha to 0xFF, pads to 0 otherwise)
      
        
          
*/


ulTarga::ulTarga()
 : ulGraphicsFile()
{

	m_id_length = 0;
	m_color_map_type = 0;   
	m_type = 0;		
	m_cm_index = 0;        
	m_cm_length = 0;      
	m_cm_entry_size = 0;    
	m_x_org = 0;		
	m_y_org = 0;
	m_desc = 0;
	
	for(int i=0; i < 256; i++ )
	{
		m_id[i] = 0;
		m_color_palette[i*3   ] = 0;
		m_color_palette[i*3 +1] = 0;
		m_color_palette[i*3 +2] = 0;
	}
}

ulTarga::~ulTarga()
{
    SAFE_ARRAY_DELETE(m.grfile_data);
    
}

void ulTarga::Release()
{
    SAFE_ARRAY_DELETE(m.grfile_data);
    
}



unsigned long * ulTarga::ReadFile(char * filename, DWORD flags)
{
	// opens and reads file to 32 bit internal format!!!
	// file can be 16, 24, or 32 bits


    m.fp = fopen( filename, "rb" );
    
    if( NULL == m.fp )
        return 0;
    
    int i, rle;
    int a, r, g, b;
    int right, top;
    unsigned short bits16;
    
    unsigned char *data;
    
    SAFE_ARRAY_DELETE(m.grfile_data)
     
    
    m_id_length = bread();
    m_color_map_type = bread();
    m_type = bread();
    
    if (!(m_type == 2 || m_type == 10 || m_type == 3))
    {
		// These are probably palettized formats - 
		// No code here to convert them, so fail

        return 0;
    }

    m_cm_index = wread();
    m_cm_length = wread();
    
    m_cm_entry_size = bread();
    
    m_x_org = wread();
    m_y_org = wread();
    m.dwWidth = wread();
    m.dwHeight = wread();
    
    SetBitsPerPixel(bread());
    
    m_desc = bread();
    /*
    #if VERBOSE
    
      printf("id_length %d\n", id_length);
      printf("color_map_type %d\n", color_map_type);
      printf("image_type %d\n", image_type);
      printf("index %d\n", cm_index);
      printf("length %d\n", cm_length);
      printf("entry_size %d\n", cm_entry_size);
      printf("x_org %d\n", image_x_org);
      printf("y_org %d\n", image_y_org);
      printf("width %d\n", image_width);
      printf("height %d\n", image_height);
      printf("depth %d\n", image_depth);
      printf("desc %02X\n", image_desc);
#endif          */
    
    m.grfile_data = (unsigned char *) new unsigned long[m.dwWidth * m.dwHeight];
    if (m.grfile_data == 0)
        return 0;
    
    
    right = m_desc & UL_TGA_LR;
    top = m_desc & UL_TGA_BT;
    
    fread((char *) m_id, m_id_length, 1, m.fp);
    fread((char *) m_color_palette, m_cm_length, m_cm_entry_size >> 3, m.fp);
    
    data = (unsigned char *)m.grfile_data;
    
    m.hxw = m.dwHeight * m.dwWidth;
    switch (m_type)
    {
    case 3:
        for (i = 0; i < m.hxw; i++)
        {
            a = bread();
            
            *data++ = a;
            *data++ = a;
            *data++ = a;
            *data++ = a;
        }
        break;
        
    case 2:
        switch(m.bits_per_pixel)
        {
        case 16:
            for (i = 0; i < m.hxw; i++)
            {
                bits16 = wread();
                b = bits16 & 0x1F;
                g = (bits16 >> 5) & 0x1F;
                r = (bits16 >> 10) & 0x1F;
                
                b <<= 3;
                g <<= 3;
                r <<= 3;
                if (flags & UL_GFX_SWAP_RGB)
                {
                    *data++ = r;
                    *data++ = g;
                    *data++ = b;
                }
                else
                {
                    *data++ = b;
                    *data++ = g;
                    *data++ = r;
                }
                
                if (flags & UL_GFX_PAD)
                    *data++ = 0xFF;
                else if (flags & UL_GFX_SIGNEDPAD)
                {
                    if (r & 0x80)
                        *data++ = 0xFF;
                    else
                        *data++ = 0;
                }
                else
                    *data++ = 0;

                
            }
            break;
            
        case 24:
            for (i = 0; i < m.hxw; i++)
            {
                b = bread();
                g = bread();
                r = bread();
                if (flags & UL_GFX_SWAP_RGB)
                {
                    *data++ = r;
                    *data++ = g;
                    *data++ = b;
                }
                else
                {
                    *data++ = b;
                    *data++ = g;
                    *data++ = r;
                }
                
                if (flags & UL_GFX_PAD)
                    *data++ = 0xFF;
                else if (flags & UL_GFX_SIGNEDPAD)
                {
                    if (r & 0x80)
                        *data++ = 0xFF;
                    else
                        *data++ = 0;
                }
                else
                    *data++ = 0;
               
            }
            break;
        case 32:
            for (i = 0; i < m.hxw; i++)
            {
                b = bread();
                g = bread();
                r = bread();
                a = bread();
                if (flags & UL_GFX_SWAP_RGB)
                {
                    *data++ = r;
                    *data++ = g;
                    *data++ = b;
                    *data++ = a;
                }
                else
                {
                    *data++ = b;
                    *data++ = g;
                    *data++ = r;
                    *data++ = a;
                }
            }
        }
        break;
    
    case 10:   //RLE TRUE color
        switch(m.bits_per_pixel)
        {
        case 24:
            i = 0;
            while (i < m.hxw)
            {
                
                rle = bread();
                if (rle < 0x80)
                {
                    // Raw data
                    // encoded as n - 1
                    rle++;
                    // # of pixels
                    while (rle)
                    {
                        b = bread();
                        g = bread();
                        r = bread();
                        if (flags & UL_GFX_SWAP_RGB)
                        {
                            *data++ = r;
                            *data++ = g;
                            *data++ = b;
                        }
                        else
                        {
                            *data++ = b;
                            *data++ = g;
                            *data++ = r;
                        }
                        if (flags & UL_GFX_PAD)
                            *data++ = 0xFF;
                        else if (flags & UL_GFX_SIGNEDPAD)
                        {
                            if (r & 0x80)
                                *data++ = 0xFF;
                            else
                                *data++ = 0;
                        }
                        else
                            *data++ = 0;
                            
                        
                        rle--;
                        i++;
                        if (i > m.hxw)
                        {
                            //printf("RLE overflow\n");
                            SAFE_ARRAY_DELETE(m.grfile_data)
                             
                            
                            return 0 ;
                        }
                    }
                }
                else
                {
                    rle -= 127;
                    b = bread();
                    g = bread();
                    r = bread();
                    
                    // # of rle
                    while (rle)
                    {
                        if (flags & UL_GFX_SWAP_RGB)
                        {
                            *data++ = r;
                            *data++ = g;
                            *data++ = b;
                        }
                        else
                        {
                            *data++ = b;
                            *data++ = g;
                            *data++ = r;
                        }
                        if (flags & UL_GFX_PAD)
                            *data++ = 0xFF;
                        else if (flags & UL_GFX_SIGNEDPAD)
                        {
                            if (r & 0x80)
                                *data++ = 0xFF;
                            else
                                *data++ = 0;
                        }
                        else
                            *data++ = 0;

                        
                        
                        i++;
                        if (i > m.hxw)
                        {
                            //printf("RLE overflow\n");
                            SAFE_ARRAY_DELETE(m.grfile_data);
                                
                            
                            return 0;
                        }
                        rle--;
                    }
                }
                
            }
            break;
            
        case 32: // RLE
            i = 0;
            while (i < m.hxw)
            {
                
                rle = bread();
                if (rle < 0x80)
                {
                    rle++;
                    while (rle)
                    {
                        if (flags & UL_GFX_SWAP_RGB)
                        {
                            b = bread();
                            g = bread();
                            r = bread();
                            a = bread();
                            *data++ = r;
                            *data++ = g;
                            *data++ = b;
                            *data++ = a;
                        }
                        else
                        {
                            
                            *data++ = bread();
                            *data++ = bread();
                            *data++ = bread();
                            *data++ = bread();
                        }
                        rle--;
                        i++;
                        if (i > m.hxw)
                        {
                            //printf("RLE overflow\n");
                            //exit(116);
                            SAFE_ARRAY_DELETE(m.grfile_data);
                                
                            
                            return 0;
                        }
                        
                    }
                }
                else 
                {
                    rle -= 127;
                    b = bread();
                    g = bread();
                    r = bread();
                    a = bread();
                    
                    while (rle)
                    {
                        if (flags & UL_GFX_SWAP_RGB)
                        {
                            *data++ = r;
                            *data++ = g;
                            *data++ = b;
                            *data++ = a;
                        }
                        else
                        {
                            *data++ = b;
                            *data++ = g;
                            *data++ = r;
                            *data++ = a;
                        }
                        rle--;
                        i++;
                        if (i > m.hxw)
                        {
                            //printf("RLE overflow\n");
                            SAFE_ARRAY_DELETE(m.grfile_data);
                                
                            return 0;
                        }
                    }
                }
                
            }
            break;
        }
    }
  
    fclose(m.fp);
  

  
  
  if (!top)
  {
      
      unsigned long *swap = (unsigned long *) new unsigned long[m.hxw];
      
      memcpy(swap, m.grfile_data, m.hxw * sizeof(unsigned long));
      
      unsigned long * src, * dest;   
      unsigned long * src_data = (unsigned long *)m.grfile_data;
      
      for (i = 0; i < m.dwHeight; i++)
      {
          src = &swap[(m.dwHeight - i - 1) * m.dwWidth];
          dest = &src_data[i * m.dwWidth];
          
          memcpy(dest, src, m.dwWidth * sizeof(unsigned long) );
      }
      
      SAFE_ARRAY_DELETE(swap);
  }
  
  
  return (unsigned long *)m.grfile_data;
}


/******************************************************************************
ulWriteTGA.cpp

  Copyright (C) 1999, 2000 NVIDIA Corporation
  This file is provided without support, instruction, or implied warranty of any
  kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
  not liable under any circumstances for any damages or loss whatsoever arising
  from the use or inability to use this file or items derived from it.
  
    Comments:
    Writes out a TGA image file (compression supported)
    
      
        
******************************************************************************/
#include <ul.h>



void ulTarga::bwrite(unsigned char data)
{ 
    fputc(data, m.fp);
}

void ulTarga::wwrite(unsigned short data)
{
    unsigned char h, l;
    
    l = data & 0xFF;
    h = data >> 8;
    bwrite(l);
    bwrite(h);
    
}


void ulTarga::GetPixel(int x, int y,
                       Float &a, Float &r, Float&g, Float&b)
{
}

void ulTarga::SetPixel(int x, int y,
                       Float &a, Float &r, Float&g, Float&b)
{
}


void ulTarga::GetPixel(unsigned char * data, DWORD format, int depth,
                       unsigned long &a, unsigned long &r, unsigned long &g, unsigned long &b)
{
    unsigned short color16, h, l;
    
    if (data >= m.endptr)
    {
        a = 0;
        r = 0;
        g = 0;
        b = 0;
        return;
    }
    
    switch(depth)
    {
    case 32:
        if (format & UL_GFX_SWAP_RGB)
        {
            r = *data++;
            g = *data++;
            b = *data++;
            a = *data++;
        }
        else
        {
            b = *data++;
            g = *data++;
            r = *data++;
            a = *data++;
        }
        break;
        
    case 24:
        if (format & UL_GFX_SWAP_RGB)
        {
            r = *data++;
            b = *data++;
            g = *data++;
            a = 0xFF;
        }
        else
        {
            b = *data++;
            g = *data++;
            r = *data++;
            a = 0xFF;
        }
        break;
        
    case 16:
        l = *data++;
        h = *data++;
        
        color16 = (h << 8) | l;
        
        if (format & UL_GFX_SWAP_RGB)
        {
            if (format & UL_GFX_565)
            {
                b = (color16 >> 11) & 0x1F;
                g = (color16 >>  5) & 0x3F;
                r =  color16        & 0x1F;
                b <<= 3;
                g <<= 2;
                r <<= 3;
            }
            else
            {
                b = (color16 >> 10) & 0x1F;
                g = (color16 >>  5) & 0x1F;
                r =  color16        & 0x1F;
                b <<= 3;
                g <<= 3;
                r <<= 3;
            }
        }
        else
        {
            if (format & UL_GFX_565)
            {
                r = (color16 >> 11) & 0x1F;
                g = (color16 >>  5) & 0x3F;
                b =  color16        & 0x1F;
                r <<= 3;
                g <<= 2;
                b <<= 3;
            }
            else
            {
                
                r = (color16 >> 10) & 0x1F;
                g = (color16 >>  5) & 0x1F;
                b =  color16        & 0x1F;
                r <<= 3;
                g <<= 3;
                b <<= 3;
            }
        }        
        
        a = 0xFF;
        break;
    default:
        break;
    }      
}

#define RLE_CUTOFF 4

// n bytes / pixel
int ulTarga::GetRLECount(unsigned char *data)
{
    int i = 0;
    int r = 0;
    unsigned char tgabuff[4];
    
    r = 1;
    
    if (data >= m.endptr)
        return 0;
    
    for (i=0; i<m.bytes_per_pixel; i++)
    {
        tgabuff[i] = *data++; 
    }
    
    while((r < 127) && (data < m.endptr))
    {
        // search for first non pixel match
        for (i=0; i<m.bytes_per_pixel; i++)
        {
            if (data >= m.endptr)
                return 0;
            
            if (tgabuff[i] != *data++)
                return r;
        }
        r++;
        
    }
    
    return r;
    
}

int ulTarga::GetRawPixelCount(unsigned char * data)
{
    int i = 0;
    int rle;
    
    if (data >= m.endptr)
        return 0;
    
    
    while(((rle = GetRLECount(data)) < RLE_CUTOFF) && (data < m.endptr) && (i < 127))
    {
        data += m.bytes_per_pixel;
        i++;
    }
    
    
    
    return i;
}


void ulTarga::WritePixel(int depth, unsigned long a, unsigned long r, unsigned long g,
                         unsigned long b)
{
    DWORD color16;
    
    switch(depth)
    {
    case 32:
        bwrite((byte)b);    // b
        bwrite((byte)g);    // g
        bwrite((byte)r);    // r
        bwrite((byte)a);    // a
        
        break;
        
    case 24:
        bwrite((byte)b);    // b
        bwrite((byte)g);    // g
        bwrite((byte)r);    // r
        break;
        
    case 16:
        r >>= 3;
        g >>= 3;
        b >>= 3;
        
        r &= 0x1F;
        g &= 0x1F;
        b &= 0x1F;
        
        color16 = (r << 10) | (g << 5) | b;
        
        wwrite((ushort)color16);    
        
        break;
        
    } 
}


// format m:n


int ulTarga::WriteFile(char *filename, unsigned char *data, const DWORD &width, const DWORD &height, 
                       int src_format, int dest_depth, DWORD format)
{
    int i;
    unsigned long r,g,b,a;
    int src_depth = src_format & 0xFF;
    
    
    
    if ((m.fp = fopen(filename, "wb")) == NULL)
    {
        //printf("Cannot open file '%s'\n", filename);
        return 0;
    }
    
    //mdesc |= LR;   // left right
    //m_desc |= UL_TGA_BT;   // top
    
    m_id_length = 0;
    m_x_org = 0;
    m_y_org = 0;
    //m_desc = 0;
    
    // 32 bpp
    
    m_cm_index = 0;
    m_cm_length = 0;
    m_cm_entry_size = 0;
    m_color_map_type = 0;
    
    if (format & UL_GFX_COMPRESSED)
        m_type = 10;
    else
        m_type = 2;
    
    //m_depth = dest_depth;
    SetBitsPerPixel(dest_depth);
    
    bwrite(m_id_length);
    bwrite(m_color_map_type);
    bwrite(m_type);
    
    wwrite(m_cm_index);
    wwrite(m_cm_length);
    
    bwrite(m_cm_entry_size);
    
    wwrite(m_x_org);
    wwrite(m_y_org);
    wwrite((unsigned short) width);
    wwrite((unsigned short) height);
    
    bwrite(m.bits_per_pixel);
    bwrite(m_desc);
    
    /*
    #if VERBOSE
    printf("id_length %d\n", id_length);
    printf("color_map_type %d\n", color_map_type);
    printf("mtype %d\n", mtype);
    printf("cm_index %d\n", cm_index);
    printf("cm_length %d\n", cm_length);
    printf("cm_entry_size %d\n", cm_entry_size);
    printf("mx_org %d\n", mx_org);
    printf("my_org %d\n", my_org);
    printf("mwidth %d\n", mwidth);
    printf("mheight %d\n", mheight);
    printf("mdepth %d\n", mdepth);
    printf("mdesc %02X\n", mdesc);
#endif       */
   

    SetBitsPerPixel(src_depth);
    m.hxw = height * width;

	m.dwHeight = height;
	m.dwWidth = width;


    int right = m_desc & UL_TGA_LR;
    int top = m_desc & UL_TGA_BT;


    // use for this data pointer
//    m.grfile_data = data;
/*  
	if( data != m.grfile_data )
	{
		SAFE_ARRAY_DELETE( m.grfile_data );

		m.grfile_data = (unsigned long

		memcpy( 

	}
*/
	DWORD * temp_dp = (DWORD*) data;		// data = input pointer
	
    DWORD * swap = 0;


    if( !top )
    {
		// This code used to be very bad.  It is now clean
		//   courtesy of GJ.

		// Swap vertical lines of a temporary image data buffer
		//   This buffer is killed on exit, so original image
		//   data is not changed.
		// Effect is to flip the stored image verticaly

        swap = (DWORD *) new DWORD[m.hxw];

		// copy whole image data to swap buffer
        memcpy(swap, temp_dp, m.hxw * sizeof(DWORD));

        DWORD * src, * dest;   

        for (i = 0; i < m.dwHeight; i++)
        {
			// copy lines from old into new buffer

			src = & temp_dp[ ( m.dwHeight - i - 1) * m.dwWidth ];

			dest = & swap[ i * m.dwWidth ];

            memcpy(dest, src, m.dwWidth * sizeof(DWORD) );
        }
        
        // use the swapped area in further processing & to write out the data
        data = (unsigned char *)swap;
    }

    
    m.size_in_bytes = m.hxw * m.bytes_per_pixel;
    m.endptr = data + m.size_in_bytes;
    
    if (format & UL_GFX_COMPRESSED)
    {
        
        int raw, rle;
        
        while(data < m.endptr)
        {
            rle = GetRLECount(data);
            
            if (rle < RLE_CUTOFF)
            {
                raw = GetRawPixelCount(data);
                if (raw == 0)
                    break;
                bwrite(raw-1);
                while(raw)
                {
                    GetPixel(data, format, src_depth, a, r, g, b);
                    WritePixel(dest_depth, a, r, g, b);
                    data += m.bytes_per_pixel;
                    raw--;
                }
            }
            else
            {
                // rle
                bwrite((rle-1) | 0x80);
                GetPixel(data, format, src_depth, a, r, g, b);
                WritePixel(dest_depth, a, r, g, b);
                
                data += m.bytes_per_pixel * rle;
                
            }
        }
        
    }  
    
    
    /*else if (src_depth == 32 && dest_depth == 32 && format == TGA_SWAPPED)
    {
    for (i = 0; i < height * width; i++)
    {
    
      bwrite(*data++);    // b
      bwrite(*data++);    // g
      bwrite(*data++);    // r
      bwrite(*data++);    // a
      } 
  } */
    
    
    else if (src_depth == dest_depth && format == 0)
    {
        fwrite(data, m.hxw, m.bytes_per_pixel, m.fp);
    }
    else
    {
        for (i = 0; i < m.hxw; i++)
        {
            GetPixel(data, format, src_depth, a, r, g, b);
            WritePixel(dest_depth, a, r, g, b);  
            data += m.bytes_per_pixel;
        }
    }
    
    fclose(m.fp);

    
    SAFE_ARRAY_DELETE(swap);
    
    return 1;
}
