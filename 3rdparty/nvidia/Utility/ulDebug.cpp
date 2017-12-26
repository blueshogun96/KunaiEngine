/******************************************************************************
ulDebug.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:



******************************************************************************/
#include <ul.h>

void dprintf( LPSTR fmt, ... )
{
#ifdef _DEBUG
  char buff[1024];
  
  va_list va;
  
  va_start( va, fmt );
  vsprintf( buff, fmt, va );
  va_end( va ); 
  OutputDebugStringA(buff);

#endif
}

