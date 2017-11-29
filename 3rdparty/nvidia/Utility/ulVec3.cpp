/*********************************************************************NVMH2****
Path:  c:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulVec3.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/


#include "ulVec3.h"
#include <ctype.h>
#include "ulMatrixVec.h"


/****************************************************************
*												*
*		    Vec3 Member functions				*
*												*
****************************************************************/

/*
ostream& operator << (ostream& s, Vec3& v)
{ 
  return s << "| " << v.x << ' ' << v.y << ' ' << v.z << " |"; 
}



istream& operator >> (istream& s, Vec3& v) {
  Vec3	v_tmp;
  char	c = ' ';
  
  while (isspace(c))
    s >> c;
  // The vectors can be formatted either as x y z or | x y z |
  if (c == '|') {
    s >> v_tmp[X] >> v_tmp[Y] >> v_tmp[Z];
    while (s >> c && isspace(c)) ;
    //if (c != '|')
     // s.set(_bad); DHR
  }
  else {
    s.putback(c);
    s >> v_tmp[X] >> v_tmp[Y] >> v_tmp[Z];
  }
  if (s)
    v = v_tmp;
  return s;
}


  */



