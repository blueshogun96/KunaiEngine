/******************************************************************************
ulNormal.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:



******************************************************************************/
#include <ul.h>

#define ONE_AS_INTEGER ((DWORD)(0x3F800000))
float __fastcall InvSqrt(const float & x)
{
  DWORD   tmp = ((ONE_AS_INTEGER << 1) + ONE_AS_INTEGER - *(DWORD*)&x) >> 1;   
  float y = *(float*)&tmp;                                             
  return y * (1.47f - 0.47f * x * y * y);
}
