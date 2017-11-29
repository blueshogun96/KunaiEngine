/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulVec3.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/

#ifndef __NVIDIA_ULVEC3_H__
#define __NVIDIA_ULVEC3_H__


//#include <ul.h>
#include <ctype.h>

#include "ulCommon.h"


class Vec3
{
    
public:
    
    union
    {
        real n[3];
        struct
        {
            real x,y,z;
        };
    };
    
public:
    
    // Constructors
    
    Vec3() {}
    Vec3(const real _x, const real _y, const real _z) {x = _x; y = _y; z = _z;}
    Vec3(const Vec3& v){x = v.x; y = v.y; z = v.z;}          // copy constructor
    
    void set(real _x, real _y, real _z){ x = _x; y = _y; z = _z;}
    
    // Assignment operators

    
    void clear() { x = y = z = 0; }
    Vec3 & operator = ( const Vec3& v );      // assignment of a Vec3         
    Vec3 & operator += ( const Vec3& v );     // incrementation by a Vec3
    Vec3 & operator -= ( const Vec3& v );     // decrementation by a Vec3
    Vec3 & operator *= ( const real d );     // multiplication by a constant
    Vec3 & operator /= ( const real d );     // division by a constant
    real& operator [] ( int i) { return n[i]; };       // indexing
    const real& operator[](int i) const { return n[i];}

    real& operator () ( int i) { return n[i]; };       // indexing
    const real& operator()(int i) const { return n[i];}
    
    INLINE Vec3 operator^(const Vec3& v) const;

    // special functions
    
    real GetMagnitude();         // length of a Vec3
    real GetMagnitudeSquared();  // squared length of a Vec3
    HRESULT normalize();
    
    
    
    void Maximize( const Vec3& rhs )
    {
        x = ( x > rhs.x ) ? x : rhs.x;
        y = ( y > rhs.y ) ? y : rhs.y;
        z = ( z > rhs.z ) ? z : rhs.z;
    }
    void Minimize( const Vec3& rhs )
    {
        x = ( x < rhs.x ) ? x : rhs.x;
        y = ( y < rhs.y ) ? y : rhs.y;
        z = ( z < rhs.z ) ? z : rhs.z;
    }
    
    Vec3& ApplyFunc(ulFuncPtrf fct);  // apply a func. to each component
    
    // //friends
    
    friend Vec3 operator - (const Vec3& v);         // -v1
    friend Vec3 operator + (const Vec3& v1, const Vec3& v2);      // v1 + v2
    friend Vec3 operator - (const Vec3& v, const Vec3& v2);      // v1 - v2
    friend Vec3 operator * (const Vec3& v, const real d);      // v1 * d
    friend Vec3 operator * (const real d, const Vec3& a);      // d * v1
    
    friend Vec3 operator * (const Mat4& m, const Vec3& v);      // M * v
    friend Vec3 operator * (const Vec3& v, const Mat4& m);      // M(transpose) * v
    
    friend Vec3 operator * (const MatR& m, const Vec3& v);      // M * v
    friend Vec3 operator * (const Vec3& v, const MatR& m);      // M(transpose) * v
    
    friend Vec3 operator * (const MatQ& m, const Vec3& v);      // M * v
    friend Vec3 operator * (const Vec3& v, const MatQ& m);      // M(transpose) * v
    
    
    
    friend real operator & (const Vec3& v1, const Vec3& v2);    // dot product
    
    friend Vec3 operator * (const Vec3& v1, const Vec3& b);    // V1 * V2
    friend Vec3 operator / (const Vec3& v, const real d);      // v1 / 3.0
    friend Vec3 operator CP (const Vec3& v1, const Vec3& v2);      // cross product
    //friend Vec3 operator % (const Vec3& v1, const Vec3& v2);
    friend int operator == (const Vec3& v1, const Vec3& v2);      // v1 == v2 ?
    friend int operator != (const Vec3& v1, const Vec3& v2);      // v1 != v2 ?
    friend ostream& operator << (ostream& s, Vec3& v);      // output to stream
    friend istream& operator >> (istream& s, Vec3& v);      // input from strm.
    
    Vec3& operator *= (const Vec3& v);
    Vec3& operator /= (const Vec3& v);
    
    // =====================================
    // Unary operators
    // =====================================
    
    friend Vec3 operator + (const Vec3& v);
    friend Vec3 operator - (const Vec3& v);
    
    
    // =====================================
    // Binary operators
    // =====================================
    
    // Addition and subtraction
    friend Vec3 operator + (const Vec3& v1, const Vec3& v2);
    friend Vec3 operator - (const Vec3& v1, const Vec3& v2);
    // Scalar multiplication and division
    friend Vec3 operator * (const Vec3& v, real s);
    friend Vec3 operator * (real s, const Vec3& v);
    friend Vec3 operator / (const Vec3& v, real s);
    // Memberwise multiplication and division
    friend Vec3 operator * (const Vec3& v1, const Vec3& v2);
    friend Vec3 operator / (const Vec3& v1, const Vec3& v2);
    
    // Vector dominance
    friend int operator < (const Vec3& v1, const Vec3& v2);
    friend int operator <= (const Vec3& v1, const Vec3& v2);
    
    // Bitwise equality
    friend int operator == (const Vec3& v1, const Vec3& v2);
    
    
    // this = v1 CP v2
    void cross(const Vec3 & v1, const Vec3 & v2);
    // this *= s;
    void scale(real s);
    
    // this = v * s
    void scale(const Vec3 &v, real s);
    // this += v
    void add(const Vec3 & v);
    // this = v1 + v2
    void add(const Vec3 & v1, const Vec3 & v2);
    
    // this = this DOT other
    real dot(const Vec3 &other) const;
    
    // min of all components
    real Min() const;
    
    // min abs of all components
    real MinAbs() const;
    // maximum of all components
    real Max() const;
    // max abs of components
    real MaxAbs() const;
    
    // this = -v
    void negate(const Vec3 &v);
    // this = -this
    void negate();
    // this -= v
    void sub(const Vec3 &v);
    
    // this = v1 - v2
    void sub(const Vec3 &v1, const Vec3 &v2);
    // this *= v
    void mult(const Vec3 &v);
    // this = v1 * v2
    void mult(const Vec3 &v1, const Vec3 &v2);
    //  this = v1 + lambda * v2; 
    void displace(const Vec3 &v1, const Vec3 &v2, real lambda); 
    
    //  this += lambda * v; 
    void displace(const Vec3 &v, real lambda);
    
    //this = lambda * v1 + (1 - lambda) * v2
    void interpolate(const Vec3 &v1, const Vec3 &v2, real lambda);
    
    // mag(this - other)
    real distance (const Vec3 &other) const;
    // mag(this - other) ^2
    real distance2(const Vec3 &other) const;  
    
    
    friend ostream& operator << (ostream& s, Vec3& v);      // output to stream
    friend istream& operator >> (istream& s, Vec3& v);      // input from strm.
    ostream &print(ostream &os) const;
    
    // this = base1 CP base 2
    // orthagonalize (possibly change base1)
    HRESULT MakeOrthonormalBasis(Vec3 & base1, Vec3 & base2);
};

// crossproduct
INLINE
Vec3 operator CP (const Vec3& p, const Vec3& q) 
{
    
    return  Vec3(
        p.y * q.z - p.z * q.y,
        p.z * q.x - p.x * q.z,
        p.x * q.y - p.y * q.x);
}

// dot
INLINE real operator & (const Vec3& a, const Vec3& b)
{
    return (a.x*b.x + a.y*b.y + a.z*b.z); 
}

INLINE Vec3 ulProduct(const Vec3& a, const Vec3& b)
{ 
    return Vec3(a.x * b.x, a.y * b.y, a.z * b.z); 
}

INLINE int operator != (const Vec3& a, const Vec3& b)
{ 
    return !(a == b); 
}



INLINE Vec3& Vec3::operator = (const Vec3& v)
{ 
    x = v.x; 
    y = v.y; 
    z = v.z; 
    return *this; 
}

INLINE Vec3&Vec3::operator += (const Vec3& v)
{
    x += v.x;   
    y += v.y;   
    z += v.z;
    return *this;
}

INLINE Vec3& Vec3::operator -= (const Vec3& v)
{
    x -= v.x;   y -= v.y;   z -= v.z;
    return *this;
}

INLINE Vec3&Vec3::operator *= (const Vec3& v)
{
    x *= v.x;   
    y *= v.y;   
    z *= v.z;
    return *this;
}

INLINE Vec3&
Vec3::operator /= (const Vec3& v)
{
    x /= v.x;   
    y /= v.y;   
    z /= v.z;
    return *this;
}

INLINE Vec3& Vec3::operator *= (real s)
{
    x *= s;   
    y *= s;   
    z *= s;
    return *this;
}

INLINE Vec3& Vec3::operator /= (real d)
{
    Float64 u = 1.0 / d;
    x = (real)(x*u);   
    y = (real)(y*u);   
    z = (real)(z*u);
    return *this;
}

INLINE Vec3 operator + (const Vec3& v)
{
    return v;
}

INLINE Vec3 operator - (const Vec3& v)
{
    return Vec3(-v.x, -v.y, -v.z);
}

INLINE Vec3 operator + (const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

INLINE Vec3 operator - (const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}

INLINE Vec3 operator * (const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
}

INLINE Vec3 operator / (const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z);
}

INLINE 
int operator < (const Vec3& v1, const Vec3& v2)
{
    return v1.x < v2.x && v1.y < v2.y && v1.z < v2.z;
}

INLINE 
int operator <= (const Vec3& v1, const Vec3& v2)
{
    return v1.x <= v2.x && v1.y <= v2.y && v1.z <= v2.z;
}

INLINE 
Vec3 operator * (const Vec3& v, real s)
{
    return Vec3(s*v.x, s*v.y, s*v.z);
}

INLINE 
Vec3 operator * (real s, const Vec3& v)
{
    return Vec3(s*v.x, s*v.y, s*v.z);
}

INLINE Vec3 operator / (const Vec3& v, real s)
{
    return Vec3(v.x/s, v.y/s, v.z/s);
}

INLINE int operator == (const Vec3& v1, const Vec3& v2)
{
    return v1.x==v2.x && v1.y==v2.y && v1.z == v2.z;
}



// SPECIAL FUNCTIONS

INLINE real Vec3::GetMagnitude()
{  
    //return (real)sqrt(GetMagnitudeSquared()); 
    return ulsqrt(GetMagnitudeSquared()); 
}

INLINE real Vec3::GetMagnitudeSquared()
{  
    return n[X]*n[X] + n[Y]*n[Y] + n[Z]*n[Z]; 
}







INLINE 
void Vec3::cross(const Vec3 &u, const Vec3 &v)
{
    x = u.y * v.z - u.z * v.y;
    y = u.z * v.x - u.x * v.z;
    z = u.x * v.y - u.y * v.x;
}

INLINE 
real Vec3::dot(const Vec3 &other) const
{
    return x * other.x + y * other.y + z * other.z;
}



INLINE 
real Vec3::Min() const
{
    return (x <= y) ? ((x <= z) ? x : z) : ((y <= z) ? y : z);
}


INLINE 
real Vec3::Max() const
{
    return (x >= y) ? ((x >= z) ? x : z) : ((y >= z) ? y : z);
}



INLINE 
void Vec3::negate(const Vec3 &v)
{
    x = - v.x;
    y = - v.y;
    z = - v.z;
}


INLINE 
void Vec3::negate()
{
    x = - x;
    y = - y;
    z = - z;
}


INLINE 
void Vec3::add(const Vec3 &u, const Vec3 &v)
{
    x = u.x + v.x;
    y = u.y + v.y;
    z = u.z + v.z;
}


INLINE 
void Vec3::add(const Vec3 &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}


INLINE 
void Vec3::sub(const Vec3 &u, const Vec3 &v)
{
    x = u.x - v.x;
    y = u.y - v.y;
    z = u.z - v.z;
}


INLINE 
void Vec3::sub(const Vec3 &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
}


INLINE 
void Vec3::mult(const Vec3 &u, const Vec3 &v)
{
    x = u.x * v.x;
    y = u.y * v.y;
    z = u.z * v.z;
}


INLINE 
void Vec3::mult(const Vec3 &v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
}


INLINE 
void Vec3::scale(const Vec3 &v, real s)
{
    x = s * v.x;
    y = s * v.y;
    z = s * v.z;
}


INLINE 
void Vec3::scale(real s)
{
    x *= s;
    y *= s;
    z *= s;
}




INLINE 
void Vec3::displace(const Vec3 &v, const Vec3 &u, real lambda)
{
    x = v.x + lambda * u.x;
    y = v.y + lambda * u.y;
    z = v.z + lambda * u.z;
}


INLINE 
void Vec3::displace(const Vec3 &u, real lambda)
{
    x += lambda * u.x;
    y += lambda * u.y;
    z += lambda * u.z;
}


INLINE 
void Vec3::interpolate(const Vec3 &u, const Vec3 &v, real lambda)
{
    real lambda2 = 1.0f - lambda;
    
    x = lambda2 * u.x + lambda * v.x;
    y = lambda2 * u.y + lambda * v.y;
    z = lambda2 * u.z + lambda * v.z;
}

INLINE 
real Vec3::distance(const Vec3 &other) const
{
    Vec3 w;
    
    w.sub(other, *this);
    return w.GetMagnitude();
}



INLINE 
ostream& Vec3::print(ostream &os) const
{
    int oldFlags = os.setf(ios::showpos);
    os << '(' << x << ' ' << y << ' ' << z << ')';
    os.flags(oldFlags);
    return os;
}



INLINE 
real Vec3::MinAbs() const
{
    real ax, ay, az;
    
    ax = ulAbs(x);
    ay = ulAbs(y);
    az = ulAbs(z);
    return (ax <= ay) ? ((ax <= az) ? ax : az) : ((ay <= az) ? ay : az);
}

INLINE HRESULT Vec3::normalize() // it is up to caller to avoid divide-by-zero
{
    return ulNormalize(x, y, z);
}

INLINE 
real Vec3::MaxAbs() const
{
    real ax, ay, az;
    
    ax = ulAbs(x);
    ay = ulAbs(y);
    az = ulAbs(z);
    return (ax >= ay) ? ((ax >= az) ? ax : az) : ((ay >= az) ? ay : az);
}


INLINE Vec3 Vec3::operator^(const Vec3& v) const
{
    Vec3 w( n[1]*v[2] - v[1]*n[2],
	   -n[0]*v[2] + v[0]*n[2],
	    n[0]*v[1] - v[0]*n[1] );
    return w;
}


INLINE ostream &operator<<(ostream &os, const Vec3 &v)  {return v.print(os);}



INLINE HRESULT Vec3::MakeOrthonormalBasis(Vec3 & base1, Vec3 & base2)
{

  if (base1.normalize())
    return ULERR_CANTMAKEVECTOR;

  if (base2.normalize())
    return ULERR_CANTMAKEVECTOR;

  *this = base1 CP base2;

  // should be already normalized
  if (normalize())
    return ULERR_CANTMAKEVECTOR;
 
  base1 = base2 CP *this;

  // this too, should be already normalized
  if (base1.normalize())
    return ULERR_CANTMAKEVECTOR;

  return 0;
}

#endif   // #ifndef __NVIDIA_ULVEC3_H__

