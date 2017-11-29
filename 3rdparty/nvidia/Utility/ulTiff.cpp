/******************************************************************************
ulTIFF.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:



******************************************************************************/
//#include <ul.h>
#include "ulGraphicsFile.h"
#include <tiffio.h>
#include <fcntl.h>


DWORD * ulTiff::ReadFile(char * filename, DWORD flags)
{
  TIFF * tif;

  SAFE_ARRAY_DELETE(m.grfile_data);

  m.grfile_data = 0;

  tif = TIFFOpen(filename, "rb");
  if (tif == 0)
    return 0;


	uint32* raster;


  TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &m.dwWidth);
	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &m.dwHeight);
  
  m.hxw = m.dwWidth * m.dwHeight;


	raster = (uint32*) _TIFFmalloc(m.hxw * sizeof (uint32));
	if (raster == 0) 
    return 0;

  m.grfile_data = (unsigned char *) new unsigned long[m.hxw];
  if (m.grfile_data == 0)
    return 0;
  
  if (TIFFReadRGBAImage(tif, m.dwWidth, m.dwHeight, raster, 0)) 
  {
    memcpy(m.grfile_data, raster, m.hxw * sizeof (uint32)); 
  }
  _TIFFfree(raster);


  TIFFClose(tif);

  unsigned long * swap = (unsigned long *)m.grfile_data;
  if (flags & UL_GFX_SWAP_RGB)
  {
    int i;
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


  return (unsigned long *)m.grfile_data;

}

