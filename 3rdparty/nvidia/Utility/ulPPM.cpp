/******************************************************************************
ulPPM.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:



******************************************************************************/
#include <ul.h>


#if 0
//-----------------------------------------------------------------------
// Name: PPMGetByte()
// Desc: Function to read in a byte from the texture map .ppm file
//-----------------------------------------------------------------------
int ulPPM::bread()
{
  WORD val;
  
  if( EOF == (val=getc(m.fp)) )    // Get a byte, and dump comments
    return 0;
  
  if( '#' == val )                     // Skip comments
  {
    while( ('\n'!=val) && ('\r'!=val) )  // Read until just after EOL
    {
      if( EOF == ( val=getc(m.fp) ) )
        return 0;
    }
  }
  return val;
}









//-----------------------------------------------------------------------
// Name: PPMGetWord()
// Desc: Function to read in a word from the texture map .ppm file
//-----------------------------------------------------------------------
int ulPPM::wread()
{
  WORD val = 0;
  BYTE ch  = 0;
  
  while( !isdigit(ch) )              // Skip bytes until ASCII 0-9
  {
    ch = bread();
  }
  
  while( isdigit(ch) )
  {
    val = (val * 10) + (ch - '0'); // Compose digits
    ch = bread();
  }
  
  return val;
}






//-----------------------------------------------------------------------
// Name: LoadPPMHeader()
// Desc: Reads in values from the header of a texture map's .ppm file
//-----------------------------------------------------------------------
DWORD * ulPPM::ReadFile(char * strFilename, DWORD flags)
{

  m.fp = _tfopen( strFilename, "rb" );
  if( NULL == m.fp)
    return 0;

  
  SAFE_ARRAY_DELETE(m.data);
 


  BYTE ch = ' ', magic[2] = "";
  
  fread( magic, 2, 1, m.fp );       // Read and check the magic bytes
  if( ('P'!=magic[0]) || ('6'!=magic[1]) )
  {
    fclose(m.fp);
    return 0;
  }
  
  // Read in width, height, and maxgrey

  m.dwWidth = wread();
  m.dwHeight = wread();
  int dwMaxgrey = wread();

  // Slurp up rest of white space so we get to actual data
  while( (' '==ch) || ('\t'==ch) || ('\n'==ch) || ('\r'==ch) )
  {
    ch = bread();
  }   
  
  // Back up to start of data and return successfully
  fseek( m.fp, -1, SEEK_CUR );
  

  m.data = (unsigned char *)new DWORD[m.dwHeight * m.dwWidth];
  unsigned char * data = m.data;

  for(DWORD y=0; y<m.dwHeight; y++)
    for(DWORD x=0; x<m.dwWidth; x++)
    {
      *data++ = bread();
      *data++ = bread();
      *data++ = bread();
      *data++ = 0;
    }

  fclose(m.fp);
  return (unsigned long *)m.data;

}


#endif


