/******************************************************************************
ulSinCos.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:



******************************************************************************/
#include <ul.h>


/* 1/64th of degree */
const int Nth = 6;





#define tindex(x)    ((long)((x) * power_table[Nth]))

#ifdef FAST_MATH

static real sin_table[360 << Nth];
static real cos_table[360 << Nth];
static real tan_table[360 << Nth];

#endif

class ULMath
{
public:
  ULMath();
  ~ULMath();
};

ULMath ulm;
ULMath::~ULMath(void)
{
}

ULMath::ULMath(void)
{
  
#ifdef FAST_MATH
  real dangle;
  Int angle;
  
  for (angle = 0; angle < (360 << Nth); angle++)
  {
    dangle = (real) angle / (real) (1 << Nth);
    sin_table[angle] = (real) sin(ulDeg2Rad(dangle));
    cos_table[angle] = (real) cos(ulDeg2Rad(dangle));
    if (dangle == 90.0 || dangle == 270.0)
      tan_table[angle] = UL_MAX_FLOAT;
    else
      tan_table[angle] = (real) tan(ulDeg2Rad(dangle));
  }
#endif
}


/*****************************************************************************

*****************************************************************************/
real
ulcos(real angle)
{
  angle = ulAnglify(angle);
#ifdef FAST_MATH
  return cos_table[tindex(angle)];
#else
  return cos(ulDeg2Rad(angle));
#endif
  
}
/*****************************************************************************

*****************************************************************************/

real
ulsin(real angle)
{
  angle = ulAnglify(angle);
#ifdef FAST_MATH
  return sin_table[tindex(angle)];
#else
  return sin(ulDeg2Rad(angle));
#endif
}
/*****************************************************************************

*****************************************************************************/

real
ultan(real angle)
{
  angle = ulAnglify(angle);
#ifdef FAST_MATH
  return tan_table[tindex(angle)];
#else
  return tan(ulDeg2Rad(angle));
#endif
  
}


/*****************************************************************************

*****************************************************************************/
real ulGetAngle(real x, real y)
{
  real angle;
  if (x != 0.0)
    angle = ulatan2(y, x);
  else if (y > 0.0)
    angle = 90.0;
  else
    angle = 270.0;
  
  return angle;
}

/*****************************************************************************

*****************************************************************************/


DWORD power_table[32] =
{
  0x00000001,                   /* 0 */
  0x00000002,                   /* 1 */
  0x00000004,                   /* 2 */
  0x00000008,                   /* 3 */
  0x00000010,                   /* 4 */
  0x00000020,                   /* 5 */
  0x00000040,                   /* 6 */
  0x00000080,                   /* 7 */
  0x00000100,                   /* 8  */
  0x00000200,                   /* 9 */
  0x00000400,                   /* 10 */
  0x00000800,                   /* 11 */
  0x00001000,                   /* 12 */
  0x00002000,                   /* 13 */
  0x00004000,                   /* 14 */
  0x00008000,                   /* 15 */
  0x00010000,                   /* 16 */
  0x00020000,                   /* 17 */
  0x00040000,                   /* 18  */
  0x00080000,                   /* 19 */
  0x00100000,                   /* 20 */
  0x00200000,                   /* 21 */
  0x00400000,                   /* 22 */
  0x00800000,                   /* 23 */
  0x01000000,                   /* 24 */
  0x02000000,                   /* 25 */
  0x04000000,                   /* 26 */
  0x08000000,                   /* 27 */
  0x10000000,                   /* 28  */
  0x20000000,                   /* 29 */
  0x40000000,                   /*     30 */
  0x80000000                    /*     31 */
};

/*****************************************************************************

*****************************************************************************/

real ulAnglify(real x)
{
  while (x < 0.0)
    x += 360.0;
  
  while (x >= 360.0)
    x -= 360.0;
  
  return x;
}

/*****************************************************************************

*****************************************************************************/

real ulAnglify180(real x)
{
  while (x <= -180.0)
    x += 360.0;
  
  while (x > 180.0)
    x -= 360.0;
  
  return x;
}



real ulJustifyAngle(real angle, real min_angle, real max_angle)
{
  real the_angle;
  
  if (angle < min_angle)
    the_angle = angle + 360.0f;
  else if (angle >= max_angle)
    the_angle = angle - 360.0f;
  else
    the_angle = angle;
  return (the_angle);
  
}





real ulatan2(real y, real x)
{
  real angle;
  
  if ((ulAbs(x) < UL_EPS) && (ulAbs(y) < UL_EPS))
    angle = 0.0;
  else
    angle = (real) ulRad2Deg(atan2(y, x));
  
  return (angle);
}

 
real ulacos ( real angle )
{
  return( (real) ulRad2Deg(acos(angle)) );
}








