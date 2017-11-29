/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulVec4.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/




#ifndef __NVIDIA_ULVEC4_H__V4V4V4__
#define __NVIDIA_ULVEC4_H__V4V4V4__


#include <ctype.h>
#include "ulCommon.h"
#include "ulVec3.h"


class Vec4
{
    
public:
    
    union
    {
        real n[4];
        struct
        {
            real x,y,z,w;
        };
        struct
        {
            Vec3 vec3;
            real Wdummy;
        };
        struct
        {
            real r,g,b,a;
        };
        struct
        {
            real u,v,q,r;
        };
        
    };
    
public:
    
    Vec4() {}
    Vec4(const real x, const real y, const real z, real w);
    void set(real x, real y, real z, real w);
    Vec4(const Vec4& v);          // copy constructor
    
    
    // Assignment operators
    
    Vec4 & operator = ( const Vec4& v );      // assignment of a Vec4         
    Vec4 & operator += ( const Vec4& v );     // incrementation by a Vec4
    Vec4 & operator -= ( const Vec4& v );     // decrementation by a Vec4
    Vec4 & operator *= ( const real d );     // multiplication by a constant
    Vec4 & operator /= ( const real d );     // division by a constant
    real& operator [] ( int i) { return n[i]; };       // indexing
    const real&operator[](int i) const;
    real& operator () ( int i) { return n[i]; };       // indexing
    const real&operator()(int i) const;
    
    // special functions
    
    real GetMagnitude();         // length of a Vec4
    real GetMagnitudeSquared();  // squared length of a Vec4
    HRESULT normalize();
    
    
    
    Vec4& ApplyFunc(ulFuncPtrf fct);  // apply a func. to each component
    void Homogenize( void );
    
    
    // //friends
    
    friend Vec4 operator - (const Vec4& v);         // -v1
    friend Vec4 operator + (const Vec4& v1, const Vec4& v2);      // v1 + v2
    friend Vec4 operator - (const Vec4& v, const Vec4& v2);      // v1 - v2
    friend Vec4 operator * (const Vec4& v, const real d);      // v1 * d
    friend Vec4 operator * (const real d, const Vec4& a);      // d * v1
    
    friend Vec4 operator * (const Mat4& m, const Vec4& v);      // M * v
    friend Vec4 operator * (const Vec4& v, const Mat4& m);      // M(transpose) * v
    
    friend Vec4 operator * (const MatR& m, const Vec4& v);      // M * v
    friend Vec4 operator * (const Vec4& v, const MatR& m);      // M(transpose) * v
    
    friend Vec4 operator * (const MatQ& m, const Vec4& v);      // M * v
    friend Vec4 operator * (const Vec4& v, const MatQ& m);      // M(transpose) * v
    
    
    friend real operator & (const Vec4& v1, const Vec4& v2);    // dot product
    friend Vec4 operator * (const Vec4& v1, const Vec4& b);    // V1 * V2
    friend Vec4 operator / (const Vec4& v, const real d);      // v1 / 3.0
    //friend Vec4 operator % (const Vec4& v1, const Vec4& v2);
    friend int operator == (const Vec4& v1, const Vec4& v2);      // v1 == v2 ?
    friend int operator != (const Vec4& v1, const Vec4& v2);      // v1 != v2 ?
    friend ostream& operator << (ostream& s, Vec4& v);      // output to stream
    friend istream& operator >> (istream& s, Vec4& v);      // input from strm.
    
    Vec4& operator *= (const Vec4& v);
    Vec4& operator /= (const Vec4& v);
    
    // =====================================
    // Unary operators
    // =====================================
    
    friend Vec4 operator + (const Vec4& v);
    friend Vec4 operator - (const Vec4& v);
    
    
    // =====================================
    // Binary operators
    // =====================================
    
    // Addition and subtraction
    friend Vec4 operator + (const Vec4& v1, const Vec4& v2);
    friend Vec4 operator - (const Vec4& v1, const Vec4& v2);
    // Scalar multiplication and division
    friend Vec4 operator * (const Vec4& v, real s);
    friend Vec4 operator * (real s, const Vec4& v);
    friend Vec4 operator / (const Vec4& v, real s);
    // Memberwise multiplication and division
    friend Vec4 operator * (const Vec4& v1, const Vec4& v2);
    friend Vec4 operator / (const Vec4& v1, const Vec4& v2);
    
    // Vector dominance
    friend int operator < (const Vec4& v1, const Vec4& v2);
    friend int operator <= (const Vec4& v1, const Vec4& v2);
    
    // Bitwise equality
    friend int operator == (const Vec4& v1, const Vec4& v2);
    
    
    // this *= s;
    void scale(real s);
    
    // this = v * s
    void scale(const Vec4 &v, real s);
    // this += v
    void add(const Vec4 & v);
    // this = v1 + v2
    void add(const Vec4 & v1, const Vec4 & v2);
    
    
    // min of all components
    real Min() const;
    
    // min abs of all components
    real MinAbs() const;
    // maximum of all components
    real Max() const;
    // max abs of components
    real MaxAbs() const;
    
    void Maximize( const Vec4& rhs )
    {
        x = ( x > rhs.x ) ? x : rhs.x;
        y = ( y > rhs.y ) ? y : rhs.y;
        z = ( z > rhs.z ) ? z : rhs.z;
        w = ( w > rhs.w ) ? w : rhs.w;
    }
    void Minimize( const Vec4& rhs )
    {
        x = ( x < rhs.x ) ? x : rhs.x;
        y = ( y < rhs.y ) ? y : rhs.y;
        z = ( z < rhs.z ) ? z : rhs.z;
        w = ( w < rhs.w ) ? w : rhs.w;
    }
    // this = -v
    void negate(const Vec4 &v);
    // this = -this
    void negate();
    // this -= v
    void sub(const Vec4 &v);
    
    // this = v1 - v2
    void sub(const Vec4 &v1, const Vec4 &v2);
    // this *= v
    void mult(const Vec4 &v);
    // this = v1 * v2
    void mult(const Vec4 &v1, const Vec4 &v2);
    //  this = v1 + lambda * v2; 
    void displace(const Vec4 &v1, const Vec4 &v2, real lambda); 
    
    //  this += lambda * v; 
    void displace(const Vec4 &v, real lambda);
    
    //this = lambda * v1 + (1 - lambda) * v2
    void interpolate(const Vec4 &v1, const Vec4 &v2, real lambda);
    
    // mag(this - other)
    real distance (const Vec4 &other) const;
    // mag(this - other) ^2
    real distance2(const Vec4 &other) const;  
    
    
    friend ostream& operator << (ostream& s, Vec4& v);      // output to stream
    friend istream& operator >> (istream& s, Vec4& v);      // input from strm.
    ostream &print(ostream &os) const;
    
    
};

INLINE
Vec4& Vec4::operator = (const Vec4& v)
{ 
    x = v.x; 
    y = v.y; 
    z = v.z; 
    w = v.w; 
    return *this; 
}


INLINE void Vec4::Homogenize( void )
{
    Float64 u = 1.0 / w;
    x = (real)(x*u);
    y = (real)(y*u);
    z = (real)(z*u);
}



INLINE
Vec4::Vec4(real _x, real _y, real _z, real _w)
{
    x = _x; 
    y = _y; 
    z = _z;
    w = _w;
}

// copy contructor
INLINE
Vec4::Vec4(const Vec4 & v)
{
    x = v.x; 
    y = v.y; 
    z = v.z;
    w = v.w;
} 




// =====================================
// Access grants
// =====================================

INLINE const real& Vec4::operator[](int i) const
{
    return n[i];
}

INLINE
int operator != (const Vec4& a, const Vec4& b)
{ return !(a == b); }


INLINE
real Vec4::GetMagnitude()
{
    //return (real)sqrt(GetMagnitudeSquared()); 
    return ulsqrt(GetMagnitudeSquared()); 
}

INLINE
real Vec4::GetMagnitudeSquared()
{ 
    return n[X]*n[X] + n[Y]*n[Y] + n[Z]*n[Z] + n[W]*n[W]; 
}



// =====================================
// Assignment operators
// =====================================

INLINE Vec4&
Vec4::operator += (const Vec4& v)
{
    x += v.x;   
    y += v.y;   
    z += v.z;
    w += v.w;
    return *this;
}

INLINE Vec4&
Vec4::operator -= (const Vec4& v)
{
    x -= v.x;   
    y -= v.y;   
    z -= v.z;
    w -= v.w;
    return *this;
}

INLINE Vec4&
Vec4::operator *= (const Vec4& v)
{
    x *= v.x;   
    y *= v.y;   
    z *= v.z;
    w *= v.w;
    return *this;
}

INLINE Vec4&
Vec4::operator /= (const Vec4& v)
{
    x /= v.x;   
    y /= v.y;   
    z /= v.z;
    w /= v.w;
    return *this;
}

INLINE Vec4&
Vec4::operator *= (real s)
{
    x *= s;   
    y *= s;   
    z *= s;
    w *= s; 
    return *this;
}

INLINE Vec4&
Vec4::operator /= (real d)
{
    Float64 u = 1.0 / d;
    x = (real)(x*u);   
    y = (real)(y*u);   
    z = (real)(z*u);
    w = (real)(w*u);
    return *this;
}

INLINE Vec4
operator + (const Vec4& v)
{
    return v;
}

INLINE Vec4
operator - (const Vec4& v)
{
    return Vec4(-v.x, -v.y, -v.z, -v.w);
}

INLINE Vec4
operator + (const Vec4& v1, const Vec4& v2)
{
    return Vec4(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z, v1.w+v2.w);
}

INLINE Vec4
operator - (const Vec4& v1, const Vec4& v2)
{
    return Vec4(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z, v1.w-v2.w);
}

INLINE Vec4
operator * (const Vec4& v1, const Vec4& v2)
{
    return Vec4(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.w*v2.w);
}

INLINE Vec4
operator / (const Vec4& v1, const Vec4& v2)
{
    return Vec4(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z, v1.w/v2.w);
}

INLINE int
operator < (const Vec4& v1, const Vec4& v2)
{
    return v1.x < v2.x && v1.y < v2.y && v1.z < v2.z && v1.w < v2.w;
}

INLINE int
operator <= (const Vec4& v1, const Vec4& v2)
{
    return v1.x <= v2.x && v1.y <= v2.y && v1.z <= v2.z && v1.w <= v2.w;
}

INLINE Vec4
operator * (const Vec4& v, real s)
{
    return Vec4(s*v.x, s*v.y, s*v.z, s * v.w);
}

INLINE Vec4
operator * (real s, const Vec4& v)
{
    return Vec4(s*v.x, s*v.y, s*v.z, s*v.w);
}

INLINE Vec4
operator / (const Vec4& v, real s)
{
    return Vec4(v.x/s, v.y/s, v.z/s, v.w/s);
}

INLINE int
operator == (const Vec4& v1, const Vec4& v2)
{
    return v1.x==v2.x && v1.y==v2.y && v1.z == v2.z && v1.w == v2.w;
}

// Vec4
INLINE 
HRESULT Vec4::normalize() 
{
    return ulNormalize(x,y,z,w);
}



INLINE Vec4& Vec4::ApplyFunc(ulFuncPtrf func)
{ 
  n[X] = func(n[X]); 
  n[Y] = func(n[Y]); 
  n[Z] = func(n[Z]);
  n[W] = func(n[W]); 
  return *this; 
}
   


/*
ostream& operator << (ostream& s, Vec4& v)
{ return s << "| " << v.x << ' ' << v.y << ' ' << v.z << ' '
  << v.w << " |"; }

istream& operator >> (istream& s, Vec4& v) 
{
  Vec4	v_tmp;
  char	c = ' ';
  
  while (isspace(c))
    s >> c;
  // The vectors can be formatted either as x y z w or | x y z w |
  if (c == '|') {
    s >> v_tmp[X] >> v_tmp[Y] >> v_tmp[Z] >> v_tmp[W];
    while (s >> c && isspace(c)) ;
    //if (c != '|')
    //  s.set(_bad); DHR
  }
  else {
    s.putback(c);
    s >> v_tmp[X] >> v_tmp[Y] >> v_tmp[Z] >> v_tmp[W];
  }
  if (s)
    v = v_tmp;
  return s;
}


  */


INLINE Vec4 ulProduct(const Vec4& a, const Vec4& b)
{
	return Vec4(a.x * b.x, a.y * b.y, a.z * b.z,
				a.w * b.w);
}





INLINE void Vec4::set(real _x, real _y, real _z, real _w)
{
  x = _x;
  y = _y;
  z = _z;
  w = _w;
}



#endif   // #ifndef __NVIDIA_ULVEC4_H__V4V4V4__
