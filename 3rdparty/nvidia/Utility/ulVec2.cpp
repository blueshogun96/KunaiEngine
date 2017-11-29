/******************************************************************************
ulVec2.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:



******************************************************************************/
//#include <ul.h>
#include "ulCommon.h"
#include "ulVec2.h"
#include <ctype.h>




/****************************************************************
*								                                                *
*		    Vec2 Member functions			                              *
*								                                                *
****************************************************************/





/*
ostream& operator << (ostream& s, Vec2& v)
{ return s << "| " << v.n[X] << ' ' << v.n[Y] << " |"; }

istream& operator >> (istream& s, Vec2& v) 
{
  Vec2	v_tmp;
  char	c = ' ';
  
  while (isspace(c))
    s >> c;
  // The vectors can be formatted either as x y or | x y |
  if (c == '|') 
  {
    s >> v_tmp[X] >> v_tmp[Y];
    while (s >> c && isspace(c)) ;
    //if (c != '|')
    //  s.set(_bad);  DHR
  }
  else 
  {
    s.putback(c);
    s >> v_tmp[X] >> v_tmp[Y];
  }
  if (s)
    v = v_tmp;
  return s;
}



  */