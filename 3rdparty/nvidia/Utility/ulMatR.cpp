/******************************************************************************
ulMatR.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:

	3 x 3 Rotation + Translation vector 

******************************************************************************/
  
#include "ul.h"   
#include "ulCommon.h"
#include "ulMatR.h"
#include "ulMatQ.h"
#include "ulQuat.h"
#include "ulMat4.h"
#include "ulVec3.h"


void ulInvertMatrix(const MatR & a, MatR & b)
{
  // invert the rotation part by transposing it
  b.R.XX = a.R.XX;
  b.R.XY = a.R.YX;
  b.R.XZ = a.R.ZX;
  b.R.YX = a.R.XY;
  b.R.YY = a.R.YY;
  b.R.YZ = a.R.ZY;
  b.R.ZX = a.R.XZ;
  b.R.ZY = a.R.YZ;
  b.R.ZZ = a.R.ZZ;

  // new displacement vector given by:  d' = -(R^-1) * d 
  b.d.x = - (a.R.XX * a.d.x + b.R.XY * a.d.y + b.R.XZ * a.d.z);
  b.d.y = - (a.R.YX * a.d.x + b.R.YY * a.d.y + b.R.YZ * a.d.z);
  b.d.z = - (a.R.ZX * a.d.x + b.R.ZY * a.d.y + b.R.ZZ * a.d.z);
}


void MatR::GetInverse(MatR &M) const
{
  ulInvertMatrix(*this, M);
}


void MatR::SetInverse(const MatR &M) 
{
  ulInvertMatrix(M, *this);
}

void MatR::Invert()
{
  Float tmp, odx, ody;

  // invert the rotation part by transposing it
  tmp  = R.XY;
  R.XY = R.YX;
  R.YX = tmp;

  tmp  = R.YZ;
  R.YZ = R.ZY;
  R.ZY = tmp;

  tmp  = R.ZX;
  R.ZX = R.XZ;
  R.XZ = tmp;

  // new displacement vector given by:  d' = -(R^T) * d 
  odx = d.x; ody = d.y;
  d.x = - (R.XX * odx + R.XY * ody + R.XZ * d.z);
  d.y = - (R.YX * odx + R.YY * ody + R.YZ * d.z);
  d.z = - (R.ZX * odx + R.ZY * ody + R.ZZ * d.z);
}



void MatR::set(const MatQ &T)
{
  R.set(T.q); 
  d = T.d;
}

void MatR::set(const Quat & q, const Vec3 & v)
{
  R.set(q); 
  d = v;
}



void MatR::xformVect(const Vec3 &v, Vec3 &xv) const
{
  //R.xform(v, xv);
  xv = R * v;
}

  
void MatR::xformVect(Vec3 &v) const
{
  //R.xform(v);
  v = R * v;
}

  
void MatR::xformPoint(const Vec3 &p, Vec3 &xp) const
{
  R.xform(p, xp);
  //xp.add(d);
  xp += d;
}


void MatR::xformPoint(Vec3 &p) const
{
  R.xform(p);
  //p.add(d);
  p += d;
}


void MatR::invXformVect(const Vec3 &v, Vec3 &xv) const
{
  R.invXform(v, xv);
}

  
void MatR::invXformVect(Vec3 &v) const
{
  R.invXform(v);
}

  
void MatR::invXformPoint(const Vec3 &p, Vec3 &xp) const
{
  //xp.sub(p, d);
  xp = p - d;
  R.invXform(xp);
}


void MatR::invXformPoint(Vec3 &p) const
{
  //p.sub(d);
  p -= d;
  R.invXform(p);
}




void MatR::mult(const MatR &M, const MatR &N)
{
  // multiply rotation matrices
  R.XX = M.R.XX * N.R.XX + M.R.XY * N.R.YX + M.R.XZ * N.R.ZX;
  R.XY = M.R.XX * N.R.XY + M.R.XY * N.R.YY + M.R.XZ * N.R.ZY;
  R.XZ = M.R.XX * N.R.XZ + M.R.XY * N.R.YZ + M.R.XZ * N.R.ZZ;
  R.YX = M.R.YX * N.R.XX + M.R.YY * N.R.YX + M.R.YZ * N.R.ZX;
  R.YY = M.R.YX * N.R.XY + M.R.YY * N.R.YY + M.R.YZ * N.R.ZY;
  R.YZ = M.R.YX * N.R.XZ + M.R.YY * N.R.YZ + M.R.YZ * N.R.ZZ;
  R.ZX = M.R.ZX * N.R.XX + M.R.ZY * N.R.YX + M.R.ZZ * N.R.ZX;
  R.ZY = M.R.ZX * N.R.XY + M.R.ZY * N.R.YY + M.R.ZZ * N.R.ZY;
  R.ZZ = M.R.ZX * N.R.XZ + M.R.ZY * N.R.YZ + M.R.ZZ * N.R.ZZ;

  // d = M.R * N.d + M.d
  d.x = M.R.XX * N.d.x + M.R.XY * N.d.y + M.R.XZ * N.d.z + M.d.x;
  d.y = M.R.YX * N.d.x + M.R.YY * N.d.y + M.R.YZ * N.d.z + M.d.y;
  d.z = M.R.ZX * N.d.x + M.R.ZY * N.d.y + M.R.ZZ * N.d.z + M.d.z;
}

MatR operator * (const MatR& a, const MatR& b) 
{
  MatR c;

  c.mult(a, b);
      
  return c;
}


void MatR::mult(const MatR &M, bool post)
{

  Float oxy, oyz, ozx, oyx, ozy, oxz, odx, ody;

  if (post == false)
  {
    // multiply rotation matrices
    oxy = R.XY; oyx = R.YX; oyz = R.YZ; ozy = R.ZY; ozx = R.ZX; oxz = R.XZ;
    
    R.XY = M.R.XX * oxy  + M.R.XY * R.YY + M.R.XZ * ozy;
    R.XZ = M.R.XX * oxz  + M.R.XY * oyz  + M.R.XZ * R.ZZ;
    R.YX = M.R.YX * R.XX + M.R.YY * oyx  + M.R.YZ * ozx;
    R.YZ = M.R.YX * oxz  + M.R.YY * oyz  + M.R.YZ * R.ZZ;
    R.ZX = M.R.ZX * R.XX + M.R.ZY * oyx  + M.R.ZZ * ozx;
    R.ZY = M.R.ZX * oxy  + M.R.ZY * R.YY + M.R.ZZ * ozy;
    
    R.XX = M.R.XX * R.XX + M.R.XY * oyx  + M.R.XZ * ozx;
    R.YY = M.R.YX * oxy  + M.R.YY * R.YY + M.R.YZ * ozy;
    R.ZZ = M.R.ZX * oxz  + M.R.ZY * oyz  + M.R.ZZ * R.ZZ;
    
    // d = M.R * d + M.d
    odx = d.x; ody = d.y;
    d.x = M.R.XX * odx + M.R.XY * ody + M.R.XZ * d.z + M.d.x;
    d.y = M.R.YX * odx + M.R.YY * ody + M.R.YZ * d.z + M.d.y;
    d.z = M.R.ZX * odx + M.R.ZY * ody + M.R.ZZ * d.z + M.d.z;
  }
  else
  {

  //Float oxy, oyz, ozx, oyx, ozy, oxz;
    Vec3 v;
    
    //d = R * M.d + d
    d.x += R.XX * M.d.x + R.XY * M.d.y + R.XZ * M.d.z;
    d.y += R.YX * M.d.x + R.YY * M.d.y + R.YZ * M.d.z;
    d.z += R.ZX * M.d.x + R.ZY * M.d.y + R.ZZ * M.d.z;
    
    // multiply rotation matrices
    oxy = R.XY; oyx = R.YX; oyz = R.YZ; ozy = R.ZY; ozx = R.ZX; oxz = R.XZ;
    R.XY = R.XX * M.R.XY + oxy  * M.R.YY + oxz  * M.R.ZY;
    R.XZ = R.XX * M.R.XZ + oxy  * M.R.YZ + oxz  * M.R.ZZ;
    R.YX = oyx  * M.R.XX + R.YY * M.R.YX + oyz  * M.R.ZX;
    R.YZ = oyx  * M.R.XZ + R.YY * M.R.YZ + oyz  * M.R.ZZ;
    R.ZX = ozx  * M.R.XX + ozy  * M.R.YX + R.ZZ * M.R.ZX;
    R.ZY = ozx  * M.R.XY + ozy  * M.R.YY + R.ZZ * M.R.ZY;
    
    R.XX = R.XX * M.R.XX + oxy  * M.R.YX + oxz  * M.R.ZX;
    R.YY = oyx  * M.R.XY + R.YY * M.R.YY + oyz  * M.R.ZY;
    R.ZZ = ozx  * M.R.XZ + ozy  * M.R.YZ + R.ZZ * M.R.ZZ;
  }
}
  


HRESULT MatR::CalcRotationMatrixFromNU(const Vec3 &vpn, const Vec3 &vup)
{
  d.set(0,0,0);
  return R.CalcRotationMatrixFromNU(vpn, vup);
}

HRESULT MatR::CalcRotationMatrixFromNU(const Vec3 & vpn, const Vec3 & vup, const Vec3 & p)
{
  int res = R.CalcRotationMatrixFromNU(vpn, vup);
 
  d[UL_N] = -(R(X, UL_N) * p.x + R(Y, UL_N) * p.y + R(Z, UL_N) * p.z);
  d[UL_U] = -(R(X, UL_U) * p.x + R(Y, UL_U) * p.y + R(Z, UL_U) * p.z);
  d[UL_R] = -(R(X, UL_R) * p.x + R(Y, UL_R) * p.y + R(Z, UL_R) * p.z);

  return res;
}



HRESULT MatR::CalcRotationMatrixFromRN(const Vec3 &vr, const Vec3 &vpn)
{
  d.set(0,0,0);
  return R.CalcRotationMatrixFromRN(vr, vpn);
}

HRESULT MatR::CalcRotationMatrixFromRN(const Vec3 & vr, const Vec3 & vpn, const Vec3 & p)
{
  int res = R.CalcRotationMatrixFromRN(vr, vpn);
 
  d[UL_N] = -(R(X, UL_N) * p.x + R(Y, UL_N) * p.y + R(Z, UL_N) * p.z);
  d[UL_U] = -(R(X, UL_U) * p.x + R(Y, UL_U) * p.y + R(Z, UL_U) * p.z);
  d[UL_R] = -(R(X, UL_R) * p.x + R(Y, UL_R) * p.y + R(Z, UL_R) * p.z);

  return res;
}

HRESULT MatR::CalcRotationMatrixFromUR(const Vec3 &vup, const Vec3 &vr)
{
  d.set(0,0,0);
  return R.CalcRotationMatrixFromUR(vup, vr);
}

HRESULT MatR::CalcRotationMatrixFromUR(const Vec3 &vup, const Vec3 &vr, const Vec3 & p)
{
  HRESULT res = R.CalcRotationMatrixFromUR(vup, vr);

  d[UL_N] = -(R(X, UL_N) * p.x + R(Y, UL_N) * p.y + R(Z, UL_N) * p.z);
  d[UL_U] = -(R(X, UL_U) * p.x + R(Y, UL_U) * p.y + R(Z, UL_U) * p.z);
  d[UL_R] = -(R(X, UL_R) * p.x + R(Y, UL_R) * p.y + R(Z, UL_R) * p.z);

  return res;

}



ostream& MatR::print(ostream &os) const
{
  return os << R << d << endl;
}

void MatR::SetTranslate(const Vec3 & v)
{
  d = v;
}

void MatR::SetTranslate(Float x, Float y, Float z)
{
  d.x = x;
  d.y = y;
  d.z = z;
}

void MatR::AddTranslate(const Vec3 & v)
{
  d += v;
}

void MatR::AddTranslate(Float x, Float y, Float z)
{
  d.x += x;
  d.y += y;
  d.z += z;
}



Float MatR::xformAxis(const Vec3 &v, int axis) const
{
  return R(X, axis) * v.x + R(Y, axis) * v.y + R(Z, axis) * v.z;
}


void MatR::SetIdentity()
{
  R.SetIdentity();
  d.set(0,0,0);
}



Mat4 MatR::GetMat4()
{
  Mat4 m;
  Vec3 vr;
  Vec3 vpn;
  Vec3 vup;

  vup = R.GetViewUp();
  vpn = R.GetViewNormal();
  vr = R.GetViewRight();

  m(X, UL_R) = vr[X];
  m(Y, UL_R) = vr[Y];
  m(Z, UL_R) = vr[Z];
  m(W, UL_R) = -(vr DOT d);

  m(X, UL_U) = vup[X];
  m(Y, UL_U) = vup[Y];
  m(Z, UL_U) = vup[Z];
  m(W, UL_U) = -(vup DOT d);

  m(X, UL_N) = vpn[X];
  m(Y, UL_N) = vpn[Y];
  m(Z, UL_N) = vpn[Z];
  m(W, UL_N) = -(vpn DOT d);

  m(X, W) = 0;
  m(Y, W) = 0;
  m(Z, W) = 0;
  m(W, W) = 1;

  return m;
}