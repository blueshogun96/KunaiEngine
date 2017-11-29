/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulMatrixVec.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:
Included by UL.h to define INLINE operators for vectors and matrices

******************************************************************************/




#ifndef __NVIDIA_ULMATRIXVECTOR_OPERATORS_H__
#define __NVIDIA_ULMATRIXVECTOR_OPERATORS_H__

#include "ulCommon.h"
#include "ulVec3.h"
#include "ulVec4.h"
#include "ulMat4.h"
#include "ulMat3.h"
#include "ulMatR.h"


INLINE 
Vec3 operator * (const Mat3& a, const Vec3& v)
{
    Vec3 ret;
    
    ret.x = a(X,X) * v.x + a(Y, X) * v.y + a(Z, X) * v.z;
    ret.y = a(X,Y) * v.x + a(Y, Y) * v.y + a(Z, Y) * v.z;
    ret.z = a(X,Z) * v.x + a(Y, Z) * v.y + a(Z, Z) * v.z;
    
    return ret;
}




INLINE 
Vec3 operator * (const Mat4& a, const Vec3& v)
{
    Vec3 ret;
    
    ret.x = a(X, X) * v.x + a(Y, X) * v.y + a(Z, X) * v.z + a(W, X);
    ret.y = a(X, Y) * v.x + a(Y, Y) * v.y + a(Z, Y) * v.z + a(W, Y);
    ret.z = a(X, Z) * v.x + a(Y, Z) * v.y + a(Z, Z) * v.z + a(W, Z);
    
    return ret;
}



INLINE 
Vec3 operator * (const MatR& a, const Vec3& v)
{
    Vec3 ret;
    a.xformPoint(v, ret);
    return ret;
}


INLINE 
Vec3 operator * (const Vec3& v, Mat4& a)
{ 
    Mat4 t;
    a.GetTranspose(t);
    return t * v; 
}


INLINE 
Vec3 operator * (const Vec3& v, Mat3& a)
{ 
    Mat3 t;
    a.GetTranspose(t);
    return t * v; 
}






INLINE Vec4 operator * (const Mat4& a, const Vec4& v) 
{
  Vec4 ret;
  
  ret[X] = a(X, X) * v.x + a(Y, X) * v.y + a(Z, X) * v.z + a(W, X) * v.w;
  ret[Y] = a(X, Y) * v.x + a(Y, Y) * v.y + a(Z, Y) * v.z + a(W, Y) * v.w;
  ret[Z] = a(X, Z) * v.x + a(Y, Z) * v.y + a(Z, Z) * v.z + a(W, Z) * v.w;
  ret[W] = a(X, W) * v.x + a(Y, W) * v.y + a(Z, W) * v.z + a(W, W) * v.w;

  return ret;
}




INLINE Vec4 operator * (const Vec4& v, const Mat4& a)
{ 
    Mat4 t;
    a.GetTranspose(t);
    return t * v; 
}



INLINE
Quat operator * (const Mat4& a, const Quat& v) 
{
  Quat ret;
  
  ret.i = a(X, X) * v.i + a(Y, X)*v.j + a(Z, X)*v.k + a(W, X)*v.s;
  ret.j = a(X, Y) * v.i + a(Y, Y)*v.j + a(Z, Y)*v.k + a(W, Y)*v.s;
  ret.k = a(X, Z) * v.i + a(Y, Z)*v.j + a(Z, Z)*v.k + a(W, Z)*v.s;
  ret.s = a(X, W) * v.i + a(Y, W)*v.j + a(Z, W)*v.k + a(W, W)*v.s;

  return ret;
}





INLINE
void Quat::GetColMatrix(Mat4 & destMatrix)
{
  real u, _xs, _ys, _zs, _wx, _wy, _wz, _xx, _xy, _xz, _yy, _yz, _zz;

  /* For unit srcQuat, just set s = 2.0; or set xs = i + 
  *  i, etc. 
  *****************************************************************************/
  u = 2.0 / (i * i + j * j + k * k + s * s);

  _xs = i * u;
  _ys = j * u;
  _zs = k * u;
  _wx = s * _xs;
  _wy = s * _ys;
  _wz = s * _zs;
  _xx = i * _xs;
  _xy = i * _ys;
  _xz = i * _zs;
  _yy = j * _ys;
  _yz = j * _zs;
  _zz = k * _zs;

  /* set up 4x4 destMatrixrix */
  destMatrix(X, X) = 1.0 - (_yy + _zz);
  destMatrix(X, Y) = _xy - _wz;
  destMatrix(X, Z) = _xz + _wy;
  destMatrix(X, W) = 0.0;

  destMatrix(Y, X) = _xy + _wz;
  destMatrix(Y, Y) = 1.0 - (_xx + _zz);
  destMatrix(Y, Z) = _yz - _wx;
  destMatrix(Y, W) = 0.0;

  destMatrix(Z, X) = _xz - _wy;
  destMatrix(Z, Y) = _yz + _wx;
  destMatrix(Z, Z) = 1.0 - (_xx + _yy);
  destMatrix(Z, W) = 0.0;

  destMatrix(W, X) = 0.0;
  destMatrix(W, Y) = 0.0;
  destMatrix(W, Z) = 0.0;
  destMatrix(W, W) = 1.0;

}                              

/*****************************************************************************
 * GetColMatrix: Convert quaternion to 3x3 column-major rotation matrix.
 *                  Quaternion need not be unit magnitude.
 *****************************************************************************/

INLINE
void Quat::GetColMatrix(Mat3 & destMatrix)

{
  real u, _xs, _ys, _zs, _wx, _wy, _wz, _xx, _xy, _xz, _yy, _yz, _zz;

  /* For unit srcQuat, just set s = 2.0; or set xs = i + 
  *  i, etc. 
  *****************************************************************************/
  u = 2.0 / (i * i + j * j + k * k + s * s);

  _xs = i * u;
  _ys = j * u;
  _zs = k * u;
  _wx = s * _xs;
  _wy = s * _ys;
  _wz = s * _zs;
  _xx = i * _xs;
  _xy = i * _ys;
  _xz = i * _zs;
  _yy = j * _ys;
  _yz = j * _zs;
  _zz = k * _zs;

  destMatrix(X, X) = 1.0 - (_yy + _zz);
  destMatrix(X, Y) = _xy - _wz;
  destMatrix(X, Z) = _xz + _wy;
                 
  destMatrix(Y, X) = _xy + _wz;
  destMatrix(Y, Y) = 1.0 - (_xx + _zz);
  destMatrix(Y, Z) = _yz - _wx;
                 
  destMatrix(Z, X) = _xz - _wy;
  destMatrix(Z, Y) = _yz + _wx;
  destMatrix(Z, Z) = 1.0 - (_xx + _yy);
                 

}               








#endif   // #ifndef __NVIDIA_ULMATRIXVECTOR_OPERATORS_H__
