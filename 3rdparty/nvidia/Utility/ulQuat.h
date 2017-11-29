/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulQuat.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/

#ifndef __NVIDIA_ULQUAT_H__
#define __NVIDIA_ULQUAT_H__


#include <ctype.h>
#include "ulCommon.h"
#include "ulVec4.h"

class Quat
{
    
    //protected:
    
public:
    union
    {
        struct
        {
            real i,j,k,s;
        };
        struct
        {
            Vec3 vec3;
            real r;
        };
        struct
        {
            Vec4 vec4;
        };
    };
    
    
public:
    
    // Constructors
    
    Quat() {}
    Quat(const real i_, const real j_, const real k_, const real _s);
    Quat(const real d);
    Quat(const Quat& v);          // copy constructor
    
    Quat(const Vec3& v, const real d);     // cast Vec3 to Quat
    
    // Assignment operators
    
    Quat & operator = ( const Quat& v );      // assignment of a Quat
    Quat & operator += ( const Quat& v );     // incrementation by a Quat
    Quat & operator -= ( const Quat& v );     // decrementation by a Quat
    Quat & operator *= ( const real d );     // multiplication by a constant
    Quat & operator /= ( const real d );     // division by a constant
    
    real& operator [] ( int i) { return vec4.n[i]; };       // indexing
    const real&operator[](int i) const { return vec4.n[i]; }

    // special functions
    
    real GetMagnitude();         // length of a Quat
    real GetMagnitudeSquared();          // squared length of a Quat
    
    Quat& ApplyFunc(ulFuncPtrf fct);       // apply a func. to each component
    
    void SetIdentity();
    void set(const real i_, const real j_, const real k_, const real s_);
    
    HRESULT Normalize();
    HRESULT normalize() { return Normalize(); }
    
    
    // //friends
    void GetColMatrix(Mat4 & m);
    void GetColMatrix(Mat3 & m);
    
    void SetQuatFromEulerDeg( real yaw, real pitch, real roll);
    friend Quat operator - (const Quat& v);         // -v1
    friend Quat operator + (const Quat& a, const Quat& b);      // v1 + v2
    friend Quat operator - (const Quat& a, const Quat& b);      // v1 - v2
    friend Quat operator * (const Quat& a, const real d);      // v1 * d
    friend Quat operator * (const real d, const Quat& a);      // d * v1
    friend Quat operator * (const Mat4& a, const Quat& v);      // M . v
    friend Quat operator * (const Quat& v, Mat4& a);      // v * M
    friend Quat operator * (const Quat& a, const Quat& b);    // mult quat
    friend Quat operator * (const Vec3& a, const Quat& b);    // mult vec *quat
    friend Quat operator * (const Quat& a, const Vec3& b);    // mult quat*vec
    friend Quat operator / (const Quat& a, const real d);      // v1 / d
    friend int operator == (const Quat& a, const Quat& b);      // v1 == v2 ?
    friend int operator != (const Quat& a, const Quat& b);      // v1 != v2 ?
    friend ostream& operator << (ostream& s, Quat& v);      // output to stream
    friend istream& operator >> (istream& s, Quat& v);      // input from strm.
    
    // necessary //friend declarations
    void xform(const Vec3 &u, Vec3 &v) const;    // this (v 0) this^-1 => xv
    void xform(Vec3 &v) const;
    
    void mult(const Quat &p, const Quat &q);            // p * q     [!]
    void mult(const Quat &q, bool post = false);                        // q * this  [!]
    //void postmult(const Quat &q);                       // this * q  [!]
    
    void invXform(const Vec3 &v, Vec3 &xv) const;
    void invXform(Vec3 &v) const;
    
    
    void deriv(const Quat &q, const Vec3 &w);
    
    void set(real angle, const Vec3 &axis, int normalizeAxis);
    void set(const Mat3 &R);
    
    ostream& print(ostream &os) const;
    
    Vec3 axis() const;
    real angle() const;
    
    
    int operator==(const Quat &other)
    {return s == other.s && 
    i == other.i &&
    j == other.j && k == other.k;}
    
    void normalize(const Quat &q);               // q/|q|
    
};


INLINE ostream &operator<<(ostream &os, const Quat &q)  {return q.print(os);}


#include <ul.h>
#include <ctype.h>

//Quaternion class
INLINE
Quat::Quat(const real i_, const real j_, const real k_, const real s_)
{
  i = i_; 
  j = j_; 
  k = k_; 
  s = s_; 
}

INLINE
Quat::Quat(const real d)
{
  i = j = k = s = d; 
}

INLINE
Quat::Quat(const Quat& v)
{ 
  i = v.i; 
  j = v.j; 
  k = v.k; 
  s = v.s; 
}


INLINE
Quat::Quat(const Vec3& v, const real d)
{ 
  i = v.x; 
  j = v.y; 
  k = v.z;  
  s = d; 
}


// ASSIGNMENT OPERATORS

INLINE
Quat & Quat::operator = (const Quat& v)
{ 
  i = v.i; 
  j = v.j; 
  k = v.k; 
  s = v.s;
  return *this; 
}

INLINE
Quat & Quat::operator += ( const Quat& v )
{
  i += v.i; 
  j += v.j; 
  k += v.k; 
  s += v.s;
  return *this; 
}

INLINE
Quat & Quat::operator -= ( const Quat& v )
{ 
  i -= v.i; 
  j -= v.j; 
  k -= v.k; 
  s -= v.s;
  return *this; 
}

INLINE
Quat & Quat::operator *= ( const real d )
{ 
  i *= d; 
  j *= d; 
  k *= d; 
  s *= d; 
  return *this; 
}

INLINE
Quat & Quat::operator /= ( const real d )
{ 
  Float64 d_inv = 1.0/d; 
  i *= d_inv; 
  j *= d_inv; 
  k *= d_inv;
  s *= d_inv; 
  return *this; 
}


// SPECIAL FUNCTIONS

INLINE
real Quat::GetMagnitude()
{ 
  return ulsqrt(GetMagnitudeSquared()); 
}

INLINE
real Quat::GetMagnitudeSquared()
{ 
  return i*i + j*j + k*k + s*s; 
}


// FRIENDS

INLINE
Quat operator - (const Quat& a)
{ 
  return Quat(-a.i, -a.j, -a.k, -a.s); 
}

INLINE
Quat operator + (const Quat& a, const Quat& b)
{ 
  return Quat( a.i + b.i, a.j + b.j, a.k + b.k, a.s + b.s); 
}

INLINE
Quat operator - (const Quat& a, const Quat& b)
{ 
  return Quat(
    a.i - b.i, 
    a.j - b.j, 
    a.k - b.k,
    a.s - b.s); 
}

INLINE
Quat operator * (const Quat& a, const real d)
{ 
  return Quat(d * a.i, d * a.j, d * a.k, d* a.s ); 
}

INLINE
Quat operator * (const real d, const Quat& a)
{
  return  a * d; 
}




INLINE
Quat operator / (const Quat& a, const real d)
{ 
  Float64 d_inv = 1.0 / d; 
  return Quat(a.i*d_inv, a.j*d_inv, a.k*d_inv,  a.s*d_inv); 
}

INLINE
int operator == (const Quat& a, const Quat& b)
{ 
  return (a.i == b.i) && (a.j == b.j) && (a.k == b.k)
  && (a.s == b.s); 
}

INLINE
int operator != (const Quat& a, const Quat& b)
{ 
  return !(a == b); 
}

/*
ostream& operator << (ostream& str, Quat& v)
{ return str << "| " << v.i << ' ' << v.j << ' ' << v.k << ' '
  << v.s << " |"; }

istream& operator >> (istream& str, Quat& v) 
{
  Quat	v_tmp;
  char	c = ' ';
  
  while (isspace(c))
    str >> c;
  // The vectors can be formatted either as x y z w or | x y z w |
  if (c == '|') {
    str >> v_tmp.i >> v_tmp.j >> v_tmp.k >> v_tmp.s;
    while (str >> c && isspace(c)) ;
    //if (c != '|')
    //  s.set(_bad); DHR
  }
  else {
    str.putback(c);
    str >> v_tmp.i >> v_tmp.j >> v_tmp.k >> v_tmp.s;
  }
  if (str)
    v = v_tmp;
  return str;
}


      */



INLINE
Quat operator *(const Quat &qLeft, const Quat &qRight)
{
  Quat temp;

  temp.s = qLeft.s * qRight.s - qLeft.i * qRight.i -
    qLeft.j * qRight.j - qLeft.k * qRight.k;

  temp.i = qLeft.s * qRight.i + qLeft.i * qRight.s +
    qLeft.j * qRight.k - qLeft.k * qRight.j;

  temp.j = qLeft.s * qRight.j + qLeft.j * qRight.s +
    qLeft.k * qRight.i - qLeft.i * qRight.k;

  temp.k = qLeft.s * qRight.k + qLeft.k * qRight.s +
    qLeft.i * qRight.j - qLeft.j * qRight.i;

  return temp;
} 



INLINE
Quat operator *(const Vec3 &v, const Quat &quat)
{
  Quat temp;
  real s = 1.0;

  temp.s = s * quat.s - v.x * quat.i -
    v.y * quat.j - v.z * quat.k;

  temp.i = s * quat.i + v.x * quat.s +
    v.y * quat.k - v.z * quat.j;

  temp.j = s * quat.j + v.y * quat.s +
    v.z * quat.i - v.x * quat.k;

  temp.k = s * quat.k + v.z * quat.s +
    v.x * quat.j - v.y * quat.i;

  return temp;
} 



INLINE
Quat operator *(const Quat &quat, const Vec3 &v)
{
  Quat temp;
  real s = 1.0;

  temp.s = quat.s * s   - quat.i * v.x - quat.j * v.y - quat.k * v.z;
  temp.i = quat.s * v.x + quat.i * s   + quat.j * v.z - quat.k * v.y;
  temp.j = quat.s * v.y + quat.j * s   + quat.k * v.x - quat.i * v.z;
  temp.k = quat.s * v.z + quat.k * s   + quat.i * v.y - quat.j * v.x;

  return temp;
} 








/*****************************************************************************
 *  
    SetQuatFromEulerDeg - converts 3 euler angles (in radians) to a quaternion
     
        angles are in degrees;  Assumes roll is rotation about X, pitch
        is rotation about Y, yaw is about Z.  Assumes order of 
        yaw, pitch, roll applied as follows:
            
            p' = roll( pitch( yaw(p) ) )

******************************************************************************/

INLINE
void Quat::SetQuatFromEulerDeg( real yaw, real pitch, real roll)
{

  real cosYaw, sinYaw, cosPitch, sinPitch, cosRoll, sinRoll;
  real half_roll, half_pitch, half_yaw;

  /* put angles into radians and divide by two, since all angles in formula
     *  are (angle/2)
   */

  half_yaw = yaw / 2.0;
  half_pitch = pitch / 2.0;
  half_roll = roll / 2.0;

  cosYaw = ulcos(half_yaw);
  sinYaw = ulsin(half_yaw);

  cosPitch = ulcos(half_pitch);
  sinPitch = ulsin(half_pitch);

  cosRoll = ulcos(half_roll);
  sinRoll = ulsin(half_roll);

  i = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
  j = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
  k = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;

  s = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;

}



INLINE
HRESULT Quat::Normalize()
{
  return ulNormalize(i,j,k,s);
} 


INLINE
void Quat::deriv(const Quat &q, const Vec3 &w)
{
  s = 0.5 * (-q.i * w.x - q.j * w.y - q.k * w.z);
  i = 0.5 * ( q.s * w.x - q.k * w.y + q.j * w.z);
  j = 0.5 * ( q.k * w.x + q.s * w.y - q.i * w.z);
  k = 0.5 * (-q.j * w.x + q.i * w.y + q.s * w.z);
}






///////////////////////////////////////////////////////////////////////////////
//
//  class Quat
//
///////////////////////////////////////////////////////////////////////////////


INLINE
void Quat::set(real angle, const Vec3 &axis, int normalizeAxis)
{
  Vec3 axis0;
  real theta, sine;

  theta = 0.5 * angle;
  sine = sin(theta);
  s = cos(theta);

  if (normalizeAxis) 
  {
    axis0 = axis;
    axis0.normalize();

    i = axis0.x * sine;
    j = axis0.y * sine;
    k = axis0.z * sine;
  }
  else 
  {
    i = axis.x * sine;
    j = axis.y * sine;
    k = axis.z * sine;
  }

}

  
INLINE
void Quat::set(const Mat3 &R)
{
  real qs2, qx2, qy2, qz2;  // squared magniudes of quaternion components
  real tmp;
  int n;

  // first compute squared magnitudes of quaternion components - at least one
  // will be greater than 0 since quaternion is unit magnitude

  qs2 = 0.25 * (R.XX + R.YY + R.ZZ + 1);
  qx2 = qs2 -   0.5 * (R.YY + R.ZZ);
  qy2 = qs2 -   0.5 * (R.ZZ + R.XX);
  qz2 = qs2 -   0.5 * (R.XX + R.YY);

  
  // find maximum magnitude component
  n = (qs2 > qx2 ) ?
    ((qs2 > qy2) ? ((qs2 > qz2) ? 0 : 3) : ((qy2 > qz2) ? 2 : 3)) :
    ((qx2 > qy2) ? ((qx2 > qz2) ? 1 : 3) : ((qy2 > qz2) ? 2 : 3));

  // compute signed quaternion components using numerically stable method
  switch(n) 
  {
  case 0:
    s = ulsqrt(qs2);
    tmp = 0.25 / s;
    i = (R.ZY - R.YZ) * tmp;
    j = (R.XZ - R.ZX) * tmp;
    k = (R.YX - R.XY) * tmp;
    break;
  case 1:
    i = ulsqrt(qx2);
    tmp = 0.25 / i;
    s = (R.ZY - R.YZ) * tmp;
    j = (R.XY + R.YX) * tmp;
    k = (R.XZ + R.ZX) * tmp;
    break;
  case 2:
    j = ulsqrt(qy2);
    tmp = 0.25 / j;
    s = (R.XZ - R.ZX) * tmp;
    k = (R.YZ + R.ZY) * tmp;
    i = (R.YX + R.XY) * tmp;
    break;
  case 3:
    k = ulsqrt(qz2);
    tmp = 0.25 / k;
    s = (R.YX - R.XY) * tmp;
    i = (R.ZX + R.XZ) * tmp;
    j = (R.ZY + R.YZ) * tmp;
    break;
  }
  // for consistency, force positive scalar component [ (s; v) = (-s; -v) ]
  if (s < 0) 
  {
    s = -s;
    i = -i;
    j = -j;
    k = -k;
  }
  ulNormalize(i,j,k,s);

  
}




INLINE
void Quat::mult(const Quat &p, const Quat &q)
{
  s = p.s * q.s - (p.i * q.i + p.j * q.j + p.k * q.k);
  i = p.s * q.i +  q.s * p.i + p.j * q.k - p.k * q.j;
  j = p.s * q.j +  q.s * p.j + p.k * q.i - p.i * q.k;
  k = p.s * q.k +  q.s * p.k + p.i * q.j - p.j * q.i;
}


INLINE
void Quat::mult(const Quat &q, bool post)
{
  real ox, oy, oz;
  if (post == false)
  {
    
    ox = i; oy = j; oz = k;
    
    i = q.s * ox +  s * q.i + q.j * oz - q.k * oy;
    j = q.s * oy +  s * q.j + q.k * ox - q.i * oz;
    k = q.s * oz +  s * q.k + q.i * oy - q.j * ox;
    s = q.s * s - (q.i * ox + q.j * oy + q.k * oz);
  }
  else
  {
    
    ox = i; oy = j; oz = k;
    
    i = s * q.i +  ox * q.s + oy * q.k - oz * q.j;
    j = s * q.j +  oy * q.s + oz * q.i - ox * q.k;
    k = s * q.k +  oz * q.s + ox * q.j - oy * q.i;
    s = s * q.s - (ox * q.i + oy * q.j + oz * q.k);
  }
}




INLINE
void Quat::xform(const Vec3 &v, Vec3 &xv) const
{
  Vec3 uv, uuv;
  uv.cross(vec3, v);
  uuv.cross(vec3, uv);


  uv.scale(2.0 * s);
  uuv.scale(2.0);
  xv.add(v, uv);
  xv.add(uuv);
}


INLINE
void Quat::xform(Vec3 &v) const
{
  Vec3 uv, uuv;

  uv.cross(vec3, v);
  uuv.cross(vec3, uv);
  uv.scale(2.0 * s);
  uuv.scale(2.0);
  v.add(uv);
  v.add(uuv);
}


INLINE
void Quat::invXform(const Vec3 &v, Vec3 &xv) const
{
  Vec3 uv, uuv;
  
  //u = (Vec3 *) &i;
  uv.cross(vec3, v);
  uuv.cross(vec3, uv);
  uv.scale(2.0 * -s);
  uuv.scale(2.0);
  xv.add(v, uv);
  xv.add(uuv);
}


INLINE
void Quat::invXform(Vec3 &v) const
{
  Vec3 uv, uuv;
  
  //u = (Vec3 *) &i;
  uv.cross(vec3, v);
  uuv.cross(vec3, uv);
  uv.scale(2.0 * -s);
  uuv.scale(2.0);
  v.add(uv);
  v.add(uuv);
}




INLINE
ostream& Quat::print(ostream &os) const
{
  int oldFlags = os.setf(ios::showpos);
  os << '(' << s << ' ' << i << ' ' << j << ' ' << k << ')';
  os.flags(oldFlags);
  return os;
}




INLINE
Vec3 Quat::axis() const
{
  Vec3 v(i, j, k);
  if (v.GetMagnitude() == 0.0) 
  {
    v.set(0,0,0);
    //v = Vec3_I;  // axis is arbitrary here
  }
  else 
    v.normalize();
  return v;
}


INLINE
real Quat::angle() const
{
  return 2 * acos(s);
}


INLINE
void Quat::normalize(const Quat &q)
{
  real scale;

  scale = ulrsqrt(q.s*q.s + q.i*q.i + q.j*q.j + q.k*q.k);
  s = scale * q.s;
  i = scale * q.i;
  j = scale * q.j;
  k = scale * q.k;
}





/*
Quat operator * (const Quat& v, Mat4& a)
{ 
  return a.GetTranspose() * v; 
} */


INLINE
void Quat::SetIdentity()
{
  i = 0;
  j = 0;
  k = 0;
  s = 1;
}


INLINE
void Quat::set(const real i_, const real j_, const real k_, const real s_)
{
  i = i_; 
  j = j_; 
  k = k_; 
  s = s_; 
}








  












#endif   // #ifndef __NVIDIA_ULQUAT_H__
