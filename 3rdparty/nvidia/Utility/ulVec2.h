/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulVec2.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:

	2D Vector !  =)

******************************************************************************/



#ifndef __NVIDIA_ULVEC2_H__V2V2V2V2__
#define __NVIDIA_ULVEC2_H__V2V2V2V2__


#include <ctype.h>
#include "ulCommon.h"


class Vec2
{
    
public:
    
    union
    {
        real n[2];
        struct
        {
            real x,y;
        };
        struct
        {
            real s, t;  // for texture coord
        };
        struct
        {
            real u, v;  // for texture coord
        };
    };
    
    
public:
    
    
    // Constructors
    Vec2(){}
    Vec2(const real x, const real y);
    void set(real _x, real _y) {x = _x; y = _y;}
    Vec2(const Vec2& v) {x = v.x; y = v.y;}          // copy constructor
    
    
    // Assignment operators
    
    Vec2 & operator = ( const Vec2& v );      // assignment of a Vec2         
    Vec2 & operator += ( const Vec2& v );     // incrementation by a Vec2
    Vec2 & operator -= ( const Vec2& v );     // decrementation by a Vec2
    Vec2 & operator *= ( const real d );     // multiplication by a constant
    Vec2 & operator /= ( const real d );     // division by a constant
    real& operator [] ( int i) { return n[i]; };       // indexing
    const real&operator[](int i) const;
    
    // special functions
    
    real GetMagnitude();         // length of a Vec2
    real GetMagnitudeSquared();  // squared length of a Vec2
    HRESULT Normalize();          // normalize a Vec2
    HRESULT normalize() { return Normalize(); }
    
    
    void Maximize( const Vec2& rhs )
    {
        x = ( x > rhs.x ) ? x : rhs.x;
        y = ( y > rhs.y ) ? y : rhs.y;
    }
    void Minimize( const Vec2& rhs )
    {
        x = ( x < rhs.x ) ? x : rhs.x;
        y = ( y < rhs.y ) ? y : rhs.y;
    }
    
    Vec2& ApplyFunc(ulFuncPtrf fct);  // apply a func. to each component
    
    // //friends
    
    friend Vec2 operator - (const Vec2& v);         // -v1
    friend Vec2 operator + (const Vec2& v1, const Vec2& v2);      // v1 + v2
    friend Vec2 operator - (const Vec2& v, const Vec2& v2);      // v1 - v2
    friend Vec2 operator * (const Vec2& v, const real d);      // v1 * d
    friend Vec2 operator * (const real d, const Vec2& a);      // d * v1
    
    friend Vec2 operator * (const Mat3& m, const Vec2& v);      // M * v
    friend Vec2 operator * (const Vec2& v, const Mat3& m);      // M(transpose) * v
    
    friend real operator & (const Vec2& v1, const Vec2& v2);    // dot product
    
    friend Vec2 operator * (const Vec2& v1, const Vec2& b);    // V1 * V2
    friend Vec2 operator / (const Vec2& v, const real d);      // v1 / 3.0
    friend Vec2 operator CP (const Vec2& v1, const Vec2& v2);      // cross product
    //friend Vec2 operator % (const Vec2& v1, const Vec2& v2);
    friend int operator == (const Vec2& v1, const Vec2& v2);      // v1 == v2 ?
    friend int operator != (const Vec2& v1, const Vec2& v2);      // v1 != v2 ?
    friend ostream& operator << (ostream& s, Vec2& v);      // output to stream
    friend istream& operator >> (istream& s, Vec2& v);      // input from strm.
    
    Vec2& operator *= (const Vec2& v);
    Vec2& operator /= (const Vec2& v);
    
    // =====================================
    // Unary operators
    // =====================================
    
    friend Vec2 operator + (const Vec2& v);
    friend Vec2 operator - (const Vec2& v);
    
    
    // =====================================
    // Binary operators
    // =====================================
    
    // Addition and subtraction
    friend Vec2 operator + (const Vec2& v1, const Vec2& v2);
    friend Vec2 operator - (const Vec2& v1, const Vec2& v2);
    // Scalar multiplication and division
    friend Vec2 operator * (const Vec2& v, real s);
    friend Vec2 operator * (real s, const Vec2& v);
    friend Vec2 operator / (const Vec2& v, real s);
    // Memberwise multiplication and division
    friend Vec2 operator * (const Vec2& v1, const Vec2& v2);
    friend Vec2 operator / (const Vec2& v1, const Vec2& v2);
    
    // Vector dominance
    friend int operator < (const Vec2& v1, const Vec2& v2);
    friend int operator <= (const Vec2& v1, const Vec2& v2);
    
    // Bitwise equality
    friend int operator == (const Vec2& v1, const Vec2& v2);
    
    
    
    
    // this = v1 CP v2
    void cross(const Vec2 & v1, const Vec2 & v2);
    // this *= s;
    void scale(real s);
    
    // this = v * s
    void scale(const Vec2 &v, real s);
    // this += v
    void add(const Vec2 & v);
    // this = v1 + v2
    void add(const Vec2 & v1, const Vec2 & v2);
    
    // this = this DOT other
    real dot(const Vec2 &other) const;
    
    // min of all components
    real Min() const;
    
    // min abs of all components
    real MinAbs() const;
    // maximum of all components
    real Max() const;
    // max abs of components
    real MaxAbs() const;
    
    // this = -v
    void negate(const Vec2 &v);
    // this = -this
    void negate();
    // this -= v
    void sub(const Vec2 &v);
    
    // this = v1 - v2
    void sub(const Vec2 &v1, const Vec2 &v2);
    // this *= v
    void mult(const Vec2 &v);
    // this = v1 * v2
    void mult(const Vec2 &v1, const Vec2 &v2);
    //  this = v1 + lambda * v2; 
    void displace(const Vec2 &v1, const Vec2 &v2, real lambda); 
    
    //  this += lambda * v; 
    void displace(const Vec2 &v, real lambda);
    
    //this = lambda * v1 + (1 - lambda) * v2
    void interpolate(const Vec2 &v1, const Vec2 &v2, real lambda);
    
    // mag(this - other)
    real distance (const Vec2 &other) const;
    // mag(this - other) ^2
    real distance2(const Vec2 &other) const;  
    
    
    friend ostream& operator << (ostream& s, Vec2& v);      // output to stream
    friend istream& operator >> (istream& s, Vec2& v);      // input from strm.
    ostream &print(ostream &os) const;
    
    
}; 

INLINE
Vec2 prod(const Vec2& a, const Vec2& b)
{
    return Vec2(a.n[X] * b.n[X], a.n[Y] * b.n[Y]); 
}


INLINE
real Vec2::GetMagnitude()
{ 
    //return (real)sqrt(GetMagnitudeSquared()); 
    return ulsqrt(GetMagnitudeSquared()); 
}

INLINE
real Vec2::GetMagnitudeSquared()
{ 
    return n[X]*n[X] + n[Y]*n[Y]; 
}


INLINE
Vec2& Vec2::ApplyFunc(ulFuncPtrf func)
{ 
    n[X] = func(n[X]); 
    n[Y] = func(n[Y]); 
    return *this; 
}



INLINE
Vec2& Vec2::operator = (const Vec2& v)
{ 
    x = v.x; 
    y = v.y; 
    return *this; 
}





INLINE
Vec2::Vec2(real _x, real _y)
{
    x = _x; 
    y = _y; 
}


// =====================================
// Access grants
// =====================================

INLINE const real& Vec2::operator[](int i) const
{
    return n[i];
}

// =====================================
// Assignment operators
// =====================================
INLINE
int operator != (const Vec2& a, const Vec2& b)
{ return !(a == b); }



INLINE Vec2&
Vec2::operator += (const Vec2& v)
{
    x += v.x;   
    y += v.y;   
    return *this;
}

INLINE Vec2&
Vec2::operator -= (const Vec2& v)
{
    x -= v.x;   y -= v.y;   
    return *this;
}

INLINE Vec2&
Vec2::operator *= (const Vec2& v)
{
    x *= v.x;   
    y *= v.y;   
    return *this;
}

INLINE Vec2&
Vec2::operator /= (const Vec2& v)
{
    x /= v.x;   
    y /= v.y;   
    return *this;
}

INLINE Vec2&
Vec2::operator *= (real s)
{
    x *= s;   
    y *= s;   
    return *this;
}

INLINE Vec2&
Vec2::operator /= (real d)
{
    Float64 u = 1.0 / d;
    x = (real)(x*u);   
    y = (real)(y*u);   
    return *this;
}

INLINE Vec2
operator + (const Vec2& v)
{
    return v;
}

INLINE Vec2
operator - (const Vec2& v)
{
    return Vec2(-v.x, -v.y);
}

INLINE Vec2
operator + (const Vec2& v1, const Vec2& v2)
{
    return Vec2(v1.x+v2.x, v1.y+v2.y);
}

INLINE Vec2
operator - (const Vec2& v1, const Vec2& v2)
{
    return Vec2(v1.x-v2.x, v1.y-v2.y);
}

INLINE Vec2
operator * (const Vec2& v1, const Vec2& v2)
{
    return Vec2(v1.x*v2.x, v1.y*v2.y);
}

INLINE Vec2
operator / (const Vec2& v1, const Vec2& v2)
{
    return Vec2(v1.x/v2.x, v1.y/v2.y);
}

INLINE int
operator < (const Vec2& v1, const Vec2& v2)
{
    return v1.x < v2.x && v1.y < v2.y;
}

INLINE int
operator <= (const Vec2& v1, const Vec2& v2)
{
    return v1.x <= v2.x && v1.y <= v2.y;
}

INLINE Vec2
operator * (const Vec2& v, real s)
{
    return Vec2(s*v.x, s*v.y);
}

INLINE Vec2
operator * (real s, const Vec2& v)
{
    return Vec2(s*v.x, s*v.y);
}

INLINE Vec2
operator / (const Vec2& v, real s)
{
    return Vec2(v.x/s, v.y/s);
}

INLINE int
operator == (const Vec2& v1, const Vec2& v2)
{
    return v1.x==v2.x && v1.y==v2.y;
}

INLINE 
HRESULT Vec2::Normalize() // it is up to caller to avoid divide-by-zero
{ 
    *this /= GetMagnitude(); 
    return 0;
}



#endif   // #ifndef __NVIDIA_ULVEC2_H__V2V2V2V2__
