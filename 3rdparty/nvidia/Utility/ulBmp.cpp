/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulBmp.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/

//#include <ul.h>
#include "ulGraphicsFile.h"



#define BOOLEAN_DEFINED
#include "gbm.h"

DWORD * ulGraphicsFile::ReadFile(const char * filename, DWORD flags)
{
  char  *opt_src = "";
  int fd, ft_src;
  GBM_ERR rc;
  GBM gbm;
  GBMRGB  palette[0x100]; // palette



  int stride, bytes;
  unsigned char * image_data;
  unsigned char * data;

  
  if ( gbm_guess_filetype(filename, &ft_src) != GBM_ERR_OK )
    return 0;
  
  
  fd = gbm_io_open(filename, O_RDONLY|O_BINARY);
  
  if (fd == -1)
    return 0;
  
  gbm_init();
  
  
  
  if ( (rc = gbm_read_header(filename, fd, ft_src, &gbm, opt_src)) != GBM_ERR_OK )
  {
    gbm_deinit();
    gbm_io_close(fd);
    return 0;
  }

  if ( (rc = gbm_read_palette(fd, ft_src, &gbm, palette)) != GBM_ERR_OK )
  {
    gbm_deinit();
    gbm_io_close(fd);
    return 0;
  }
  m.dwWidth = gbm.w;
  m.dwHeight = gbm.h;
  m.hxw = m.dwWidth * m.dwHeight;

  SetBitsPerPixel(gbm.bpp);
 
  stride = ( ((gbm.w * gbm.bpp + 31)/32) * 4 );
  bytes = stride * gbm.h;
  
  image_data = (unsigned char *)malloc(bytes);
  
  if (image_data == NULL )
  {
    gbm_deinit();
    gbm_io_close(fd);
    return 0;
  }
  
  if ( (rc = gbm_read_data(fd, ft_src, &gbm, image_data)) != GBM_ERR_OK )
  {
    gbm_deinit();
    gbm_io_close(fd);
    return 0;
  }
  
  m.grfile_data = new unsigned char[ m.dwWidth * m.dwHeight * 4];

  DWORD x,y;

  data = m.grfile_data;
  unsigned char * ptr;
  unsigned char * line = image_data;


  switch(m.bits_per_pixel)
  {
  case 8:
    for(y=0; y<m.dwHeight; y++)
    {
      ptr = line;
      for(x=0; x<m.dwWidth; x++)
      {
        *data++ = palette[*ptr].b;
        *data++ = palette[*ptr].g;
        *data++ = palette[*ptr].r;
        *data++ = 0;
        ptr++;
      }
      line += stride;
    }
    break;
  case 24:
    for(y=0; y<m.dwHeight; y++)
    {
      ptr = line;
      for(x=0; x<m.dwWidth; x++)
      {
        *data++ =*ptr++;//b
        *data++ =*ptr++;//g
        *data++ =*ptr++;//r
        *data++ = 0;
      }
      line += stride;
    }
    break;
    
  case 32:
    for(y=0; y<m.dwHeight; y++)
    {
      ptr = line;
      for(x=0; x<m.dwWidth; x++)
      {
        *data++ =*ptr++;
        *data++ =*ptr++;
        *data++ =*ptr++;
        *data++ =*ptr++;
      }
      line += stride;
    }
    break;
  }
  
  free(image_data);
  gbm_io_close(fd);
  
  gbm_deinit();
  


  
  unsigned long * swap = (unsigned long *)m.grfile_data;
  if (flags & UL_GFX_SWAP_RGB)
  {
    DWORD i;
    int a,r,b,g,c;
    for(i=0; i<m.dwHeight * m.dwWidth; i++)
    {
      c = *swap;
      a = (c >> 24) & 0xFF;
      r = (c >> 16) & 0xFF;
      g = (c >> 8) & 0xFF;
      b = (c >> 0) & 0xFF;
      
      *swap++ = (a << 24) | (b << 16) | (g << 8) | r;
    }
  }
  

  
  swap = (unsigned long *) new unsigned long[m.hxw];
    
  memcpy(swap, m.grfile_data, m.hxw * sizeof(unsigned long));
    
  unsigned long * src, * dest;   
  unsigned long * src_data = (unsigned long *)m.grfile_data;

  for (DWORD i = 0; i < m.dwHeight; i++)
  {
      src = &swap[(m.dwHeight - i - 1) * m.dwWidth];
      dest = &src_data[i * m.dwWidth];

      memcpy(dest, src, m.dwWidth * sizeof(unsigned long) );
  }
  
  SAFE_ARRAY_DELETE(swap);
  
  return (unsigned long *)m.grfile_data;
  
}


int ulGraphicsFile::WriteFile(const char *filename, unsigned char *data, const DWORD &width, const DWORD &height, 
                          int src_format, int dest_depth, DWORD format)
{
  
  if (data == 0)
    return -1;

  int ft_dst;
  DWORD flag = 0;
  char * opt_dst = "";
  GBM gbm;

  GBMRGB  palette[0x100];
  GBMFT gbmft;
  int fd;
  unsigned char * image_data;
  unsigned char * line;
  unsigned char * ptr;
  DWORD x,y;
  HRESULT hr;

  gbm_init();


  if ( gbm_guess_filetype(filename, &ft_dst) != GBM_ERR_OK )
    return -1;

  gbm.w = width; 
  gbm.h = height;
  gbm.bpp = 24;


  gbm_query_filetype(ft_dst, &gbmft);
  switch ( gbm.bpp )
  {
  case 24:  
    flag = GBM_FT_W24;  
    break;
  case 8:   
    flag = GBM_FT_W8; 
    break;
  case 4:   
    flag = GBM_FT_W4; 
    break;
  case 1:   
    flag = GBM_FT_W1; 
    break;
  }
  if ( (gbmft.flags & flag) == 0 )
  {
    //gbm_io_close(fd);
    return -1;
  }
  
  int stride = ((gbm.w * 3 + 3) & ~3);
  //int stride = ( ((gbm.w * gbm.bpp + 31)/32) * 4 );
  int bytes = stride * gbm.h;
  
  image_data = (unsigned char *)malloc(bytes);
  line = image_data;

  // should convert src_format here
  // convert to 24 bpp
  if (format & UL_GFX_SWAP_RGB)
  {
    for(y=0; y<height; y++)
    {
      ptr = line;
      for(x=0; x<width; x++)
      {
        *ptr++ = data[2];
        *ptr++ = data[1];
        *ptr++ = data[0];
        data+=4; // skip alpha
      }
      line += stride;
    }
  }
  else
  {
    for(y=0; y<height; y++)
    {
      ptr = line;
      for(x=0; x<width; x++)
      {
        *ptr++ = *data++;
        *ptr++ = *data++;
        *ptr++ = *data++;
        data++; // skip alpha
      }
      line += stride;
    }
  }
    

  if ( (fd = gbm_io_create(filename, O_WRONLY|O_BINARY)) == -1 )
    return -1;
  
  if ( format & UL_GFX_INVERT_VERT )
    gbm_ref_vert(&gbm, image_data);
  
  if ( format & UL_GFX_INVERT_HORIZ )
    gbm_ref_horz(&gbm, image_data);




  if ( (hr = gbm_write(filename, fd, ft_dst, &gbm, palette, image_data, opt_dst)) != GBM_ERR_OK )
  {
    gbm_io_close(fd);
    remove(filename);
    return -1;
  }
  
  free(image_data);
  gbm_io_close(fd);

  gbm_deinit();


  
  return 0;
}

