/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulSqrt.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/

#include <ul.h>




static real32 _0_47 = 0.47f;
static real32 _1_47 = 1.47f;


/*
// This may not be any faster than standard sqrt

real32 __fastcall ulrsqrt(real32 x)
{
    DWORD y;
    real32 r;
    _asm
    {
        mov     eax, 07F000000h+03F800000h // (ONE_AS_INTEGER<<1) + ONE_AS_INTEGER
        sub     eax, x
        sar     eax, 1
        
        mov     y, eax                      // y
        fld     _0_47                       // 0.47
        fmul    DWORD PTR x                 // x*0.47
        
        fld     DWORD PTR y
        fld     st(0)                       // y y x*0.47
        fmul    st(0), st(1)                // y*y y x*0.47
        
        fld     _1_47                       // 1.47 y*y y x*0.47
        fxch    st(3)                       // x*0.47 y*y y 1.47
        fmulp   st(1), st(0)                // x*0.47*y*y y 1.47
        fsubp   st(2), st(0)                // y 1.47-x*0.47*y*y
        fmulp   st(1), st(0)                // result
        fstp    y
        and     y, 07FFFFFFFh
            
    }
    r = *(real32 *)&y;
    // optional
    r = (3.0f - x * (r * r)) * r * 0.5f; // remove for low accuracy
    return r;
}
*/

/*
    sqrt(x) = x / sqrt(x)
*/


/*
real32 __fastcall ulsqrt(real32 x)
{
  return x * ulrsqrt(x);

}
*/


/*
  1/(sqrt(a)

real rsqrt(real a, real eps)
{
    real en, yn, yn1;

    // start value
    yn = a * 0.5;

    for(int i=0; i<100; i++)
    {
        en = (0.5 - 0.5 * a * yn * yn);

        yn1 = (yn + en * yn);

        if (fabs(yn1 - yn) < eps)
            return yn1;

        yn = yn1;
    }

    // close enough
    return yn;
}
 */

