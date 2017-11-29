/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulMat4.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/

#ifndef __NVIDIA_ULMAT4_H_44444__
#define __NVIDIA_ULMAT4_H_44444__


#include <ctype.h>

#include "ulVec4.h"
#include "ulCommon.h"



class Mat4
{
    
public:
    Vec4 row[4];
    static Mat4 I;


    Mat4 Mat4::adjoint() const;

    

    real & m(int i, int j) { return row[i][j]; }
    real m(int i, int j) const { return row[i][j]; }

    // Constructors
    
    Mat4() {}
    Mat4(real a0, real b0, real c0, real d0,
        real a1, real b1, real c1, real d1,
        real a2, real b2, real c2, real d2,
        real a3, real b3, real c3, real d3);
    
    void set(real a0, real b0, real c0, real d0,
        real a1, real b1, real c1, real d1,
        real a2, real b2, real c2, real d2,
        real a3, real b3, real c3, real d3);
    
    void set(const Mat4 &mat);
    Mat4(const Vec4& v0, const Vec4& v1, const Vec4& v2, const Vec4& v3);
    Mat4(const real d);
    Mat4(const Mat4& m);
    
    // Assignment operators
    
    Mat4& operator  = ( const Mat4& m );      // assignment of a Mat4
    Mat4& operator += ( const Mat4& m );      // incrementation by a Mat4
    Mat4& operator -= ( const Mat4& m );      // decrementation by a Mat4
    Mat4& operator *= ( const real d );      // multiplication by a constant
    Mat4& operator /= ( const real d );      // division by a constant
    real & operator ()  ( const int i, const int j) { return row[i][j]; }
    real operator ()  ( const int i, const int j) const { return row[i][j]; }
    
    // special functions
    
    Mat3 rot();
    void SetRotation(const Mat3 & m);
    Vec3 trans() const;
    
    Vec3 GetViewUp() ;
    void GetViewUp(Vec3 & v);
    
    Vec3 GetViewNormal() ;
    void GetViewNormal(Vec3 &v) ;
    
    Vec3 GetViewRight() ;
    void GetViewRight(Vec3 & v);
    
    Vec3 GetTranslation() ;
    void GetTranslation(Vec3 & v);

    void SetTranslation(const Vec3 & v);

    Mat4 & GetMat4();
    
    void Transpose();         // transpose
    
    void Invert();            // inverse
    HRESULT InvertArbitrary();            // inverse
    HRESULT InvertAffine();            // inverse
    void GetInverse(Mat4 & q) const;
    HRESULT GetInverseArbitrary(Mat4 & q) const;
    HRESULT GetInverseAffine(Mat4 & q) const;
    void SetInverse(const Mat4 & q);
    HRESULT SetInverseArbitrary(const Mat4 & q);
    HRESULT SetInverseAffine(const Mat4 & q);
    
    void GetTranspose(Mat4 & q) const;
    void SetIdentity();           // make this identity
    void SetToZero();           // make this zero
    
    void SetRotateXDeg(real degrees);
    void RotateXDeg(real degrees, bool post = false);
    
    void SetRotateYDeg(real degrees);
    void RotateYDeg(real degrees, bool post = false);
    
    void SetRotateZDeg(real degrees);
    void RotateZDeg(real degrees, bool post = false);
    
    
    void SetScale(real sx, real sy, real sz);
    void SetScale(const Vec3 & tvec);
    void Scale(real sx, real sy, real sz, bool post = false);
    void Scale(const Vec3 & tvec, bool post = false);
    
    
    
    void SetTranslate(const real &x, const real &y, const real &z);
    void SetTranslate(const Vec3 & tvec);
    
    void Translate(const real &tx, const real &ty, const real &tz, bool post = false);
    void Translate(const Vec3 & tvec, bool post = false);
    
    
    void SetTranslationPoint(const Vec3 &pos);
    
    friend Mat4 operator - (const Mat4& a);         // -m1
    friend Mat4 operator + (const Mat4& a, const Mat4& b);      // m1 + m2
    friend Mat4 operator - (const Mat4& a, const Mat4& b);      // m1 - m2
    friend Mat4 operator * (const Mat4& a, const Mat4& b);      // m1 * m2
    friend Mat4 operator * (const Mat4& a, const real d);      // m1 * d
    friend Mat4 operator * (const real d, const Mat4& a);      // d * m1
    friend Mat4 operator / (const Mat4& a, const real d);      // m1 / d
    friend int operator == (const Mat4& a, const Mat4& b);      // m1 == m2 ?
    friend int operator != (const Mat4& a, const Mat4& b);      // m1 != m2 ?
    friend ostream& operator << (ostream& s, Mat4& m);      // output to stream
    friend istream& operator >> (istream& s, Mat4& m);      // input from strm.
    
    HRESULT CalcRotationMatrixFromPoints(const Vec3 & p1, const Vec3 & p2, const Vec3 & p3);
    
    HRESULT CalcRotationMatrixFromRN(Vec3 vr, Vec3 vpn);
    HRESULT CalcRotationMatrixFromRN(Vec3 vr, Vec3 vpn, const Vec3 &p1);
    
    HRESULT CalcRotationMatrixFromNU(Vec3 vpn, Vec3 vup);
    HRESULT CalcRotationMatrixFromNU(Vec3 vpn, Vec3 vup, const Vec3 &p1);
    
    HRESULT CalcRotationMatrixFromUR(Vec3 vup, Vec3 vr);
    HRESULT CalcRotationMatrixFromUR(Vec3 vup, Vec3 vr, const Vec3 &p1);
    
    
    void CalcRotationMatrixFromAllVectors(const Vec3 & vpn, const Vec3 &vup, const Vec3 &vr, const Vec3 &pos);
    void CalcRotationMatrixFromAllVectors(const Vec3 & vpn, const Vec3 &vup, const Vec3 &vr);
    HRESULT CalcProjectionMatrixDeg(real fFOV_degrees, real fAspect,
        real Znear, real Zfar );
    
    void Normalize3x3Vectors();		// normalize each 3x1 vector of the 3x3 subset

    void CalcRotationMatrixAboutLine(const Vec3 &pos, const real &angle, const Vec3 &vpn);
    
    real xformAxis(const Vec3 &v, int axis) const;
};



////////////////
#include "ulMatrixVec.h"

/****************************************************************
*								                                                *
*		    Mat4 member functions			                              *
*								                                                *
****************************************************************/




INLINE void ulInvertMatrix(const Mat4 & src, Mat4 & inverse)
{
    inverse.XX = src.XX;
    inverse.XY = src.YX;
    inverse.XZ = src.ZX;
    inverse.YX = src.XY;
    inverse.YY = src.YY;
    inverse.YZ = src.ZY;
    inverse.ZX = src.XZ;
    inverse.ZY = src.YZ;
    inverse.ZZ = src.ZZ;
    
    /* the new displacement vector is given by:  d' = -(R^-1) * d */
    
    inverse.WX = -( src.WX * inverse.XX + src.WY * inverse.YX + src.WZ * inverse.ZX );
    inverse.WY = -( src.WX * inverse.XY + src.WY * inverse.YY + src.WZ * inverse.ZY );
    inverse.WZ = -( src.WX * inverse.XZ + src.WY * inverse.YZ + src.WZ * inverse.ZZ );
    
    /* the rest stays the same */
    inverse.XW = inverse.YW = inverse.ZW = 0.0; 
    inverse.WW = 1.0;
}       

INLINE void ulInvertMatrix(Mat4 & a)
{
    Mat4 b;
    ulInvertMatrix(a, b);
    a = b;
    
}


INLINE HRESULT ulInvertMatrixAffine(const Mat4 &src, Mat4 &inverse) 
{
    real64 a00 = src.XX;
    real64 a01 = src.XY;
    real64 a02 = src.XZ;
    real64 a20 = src.ZX;
    
    real64 a11 = src.YY;
    real64 a22 = src.ZZ;
    real64 a12 = src.YZ;
    real64 a21 = src.ZY;
    real64 a10 = src.YX;
    
    real64 a11_22 = a11 * a22;
    real64 a12_21 = a12 * a21;
    real64 a10_22 = a10 * a22;
    real64 a10_21 = a10 * a21;
    real64 a11_20 = a11 * a20;
    real64 a12_20 = a12 * a20;
    
    
    
    real64 fDetInv = 
        a00 * ( a11_22 - a12_21 ) -
        a01 * ( a10_22 - a12_20 ) +
        a02 * ( a10_21 - a11_20 );
    
    
    if (ulAbs(fDetInv) < UL_EPS)
        return UL_CANTINVERTMATRIX;
    
    
    fDetInv = 1.0 / fDetInv;
    
    
    real64 b00;
    real64 b10;
    real64 b20;
    
    real64 b01;
    real64 b11;
    real64 b21;
    
    real64 b02;
    real64 b12;
    real64 b22;
    
    inverse.XX = b00 =  fDetInv * ( a11_22 - a12_21 );
    inverse.XY = b01 = -fDetInv * ( a01 * a22 - a02 * a21 );
    inverse.XZ = b02 =  fDetInv * ( a01 * a12 - a02 * a11 );
    inverse.XW = 0.0f;
    
    inverse.YX = b10 = -fDetInv * ( a10_22 - a12_20 );
    inverse.YY = b11 =  fDetInv * ( a00 * a22 - a02 * a20 );
    inverse.YZ = b12 = -fDetInv * ( a00 * a12 - a02 * a10 );
    inverse.YW = 0.0f;
    
    inverse.ZX = b20 =  fDetInv * ( a10_21 - a11_20 );
    inverse.ZY = b21 = -fDetInv * ( a00 * a21 - a01 * a20 );
    inverse.ZZ = b22 =  fDetInv * ( a00 * a11 - a01 * a10 );
    inverse.ZW = 0.0f;
    
    real a30 = src.WX;
    real a31 = src.WY;
    real a32 = src.WZ;
    
    
    inverse.WX = -( a30 * b00 + a31 * b10 + a32 * b20 );
    inverse.WY = -( a30 * b01 + a31 * b11 + a32 * b21 );
    inverse.WZ = -( a30 * b02 + a31 * b12 + a32 * b22 );
    inverse.WW = 1.0f;
    
    
    return 0;
}


INLINE HRESULT ulInvertMatrixArbitrary(const Mat4 &src, Mat4 &inverse) 
{
    real64 x00, x01, x02;
    real64 x10, x11, x12;
    real64 x20, x21, x22;
    real64 x30, x31, x32;
    real64 rcp;
    real64 y01, y02, y03, y12, y13, y23;
    real64 z02, z03, z12, z13, z22, z23, z32, z33;
    
#define x03 x01
#define x13 x11
#define x23 x21
#define x33 x31
#define z00 x02
#define z10 x12
#define z20 x22
#define z30 x32
#define z01 x03
#define z11 x13
#define z21 x23
#define z31 x33
    
    /* read 1st two columns of matrix into registers */
    x00 = src.XX;
    x01 = src.XY;
    x10 = src.YX;
    x11 = src.YY;
    x20 = src.ZX;
    x21 = src.ZY;
    x30 = src.WX;
    x31 = src.WY;
    
    /* compute all six 2x2 determinants of 1st two columns */
    y01 = x00*x11 - x10*x01;
    y02 = x00*x21 - x20*x01;
    y03 = x00*x31 - x30*x01;
    y12 = x10*x21 - x20*x11;
    y13 = x10*x31 - x30*x11;
    y23 = x20*x31 - x30*x21;
    
    /* read 2nd two columns of matrix into registers */
    x02 = src.XZ;
    x03 = src.XW;
    x12 = src.YZ;
    x13 = src.YW;
    x22 = src.ZZ;
    x23 = src.ZW;
    x32 = src.WZ;
    x33 = src.WW;
    
    /* compute all 3x3 cofactors for 2nd two columns */
    z33 = x02*y12 - x12*y02 + x22*y01;
    z23 = x12*y03 - x32*y01 - x02*y13;
    z13 = x02*y23 - x22*y03 + x32*y02;
    z03 = x22*y13 - x32*y12 - x12*y23;
    z32 = x13*y02 - x23*y01 - x03*y12;
    z22 = x03*y13 - x13*y03 + x33*y01;
    z12 = x23*y03 - x33*y02 - x03*y23;
    z02 = x13*y23 - x23*y13 + x33*y12;
    
    /* compute all six 2x2 determinants of 2nd two columns */
    y01 = x02*x13 - x12*x03;
    y02 = x02*x23 - x22*x03;
    y03 = x02*x33 - x32*x03;
    y12 = x12*x23 - x22*x13;
    y13 = x12*x33 - x32*x13;
    y23 = x22*x33 - x32*x23;
    
    /* read 1st two columns of matrix into registers */
    x00 = src.XX;
    x01 = src.XY;
    x10 = src.YX;
    x11 = src.YY;
    x20 = src.ZX;
    x21 = src.ZY;
    x30 = src.WX;
    x31 = src.WY;
    
    /* compute all 3x3 cofactors for 1st column */
    z30 = x11*y02 - x21*y01 - x01*y12;
    z20 = x01*y13 - x11*y03 + x31*y01;
    z10 = x21*y03 - x31*y02 - x01*y23;
    z00 = x11*y23 - x21*y13 + x31*y12;
    
    /* compute 4x4 determinant & its reciprocal */
    rcp = x30*z30 + x20*z20 + x10*z10 + x00*z00;
    if (rcp == 0.0f)
        return -1;
    rcp = 1.0f/rcp;
    
    /* compute all 3x3 cofactors for 2nd column */
    z31 = x00*y12 - x10*y02 + x20*y01;
    z21 = x10*y03 - x30*y01 - x00*y13;
    z11 = x00*y23 - x20*y03 + x30*y02;
    z01 = x20*y13 - x30*y12 - x10*y23;
    
    /* multiply all 3x3 cofactors by reciprocal */
    inverse.XX = (real)(z00*rcp);
    inverse.YX = (real)(z01*rcp);
    inverse.XY = (real)(z10*rcp);
    inverse.ZX = (real)(z02*rcp);
    inverse.XZ = (real)(z20*rcp);
    inverse.WX = (real)(z03*rcp);
    inverse.XW = (real)(z30*rcp);
    inverse.YY = (real)(z11*rcp);
    inverse.ZY = (real)(z12*rcp);
    inverse.YZ = (real)(z21*rcp);
    inverse.WY = (real)(z13*rcp);
    inverse.YW = (real)(z31*rcp);
    inverse.ZZ = (real)(z22*rcp);
    inverse.WZ = (real)(z23*rcp);
    inverse.ZW = (real)(z32*rcp);
    inverse.WW = (real)(z33*rcp);
    
    return 0;
    
#undef x03
#undef x13
#undef x23
#undef x33
#undef z00
#undef z10
#undef z20
#undef z30
#undef z01
#undef z11
#undef z21
#undef z31
    
}






INLINE HRESULT ulInvertMatrixArbitrary(Mat4 & a)
{
    HRESULT res;
    Mat4 b;
    res = ulInvertMatrixArbitrary(a, b);
    a = b;
    return res;
}


INLINE HRESULT ulInvertMatrixAffine(Mat4 & a)
{
    HRESULT res;
    Mat4 b;
    res = ulInvertMatrixAffine(a, b);
    a = b;
    return res;
}


INLINE Mat4::Mat4(const Vec4& v0, const Vec4& v1, const Vec4& v2, const Vec4& v3)
{ 
    
    XX = v0.n[0];
    XY = v0.n[1];
    XZ = v0.n[2];
    XW = v0.n[3];
    
    YX = v1.n[0];
    YY = v1.n[1];
    YZ = v1.n[2];
    YW = v1.n[3];
    
    ZX = v2.n[0];
    ZY = v2.n[1];
    ZZ = v2.n[2];
    ZW = v2.n[3];
    
    WX = v3.n[0];
    WY = v3.n[1];
    WZ = v3.n[2];
    WW = v3.n[3];
    
    
    
}


INLINE void Mat4::set( real a0,  real b0,  real c0, real d0,
               real a1,  real b1,  real c1, real d1,
               real a2,  real b2,  real c2, real d2,
               real a3,  real b3,  real c3, real d3)
{
    XX = a0;
    XY = b0;
    XZ = c0;
    XW = d0;
    
    
    YX = a1;
    YY = b1;
    YZ = c1;
    YW = d1;
    
    
    ZX = a2;
    ZY = b2;
    ZZ = c2;
    ZW = d2;
    
    
    WX = a3;
    WY = b3;
    WZ = c3;
    WW = d3;
}




INLINE Mat4::Mat4( real a0,  real b0,  real c0, real d0,
           real a1,  real b1,  real c1, real d1,
           real a2,  real b2,  real c2, real d2,
           real a3,  real b3,  real c3, real d3)
{
    XX = a0;
    XY = b0;
    XZ = c0;
    XW = d0;
    
    
    YX = a1;
    YY = b1;
    YZ = c1;
    YW = d1;
    
    
    ZX = a2;
    ZY = b2;
    ZZ = c2;
    ZW = d2;
    
    
    WX = a3;
    WY = b3;
    WZ = c3;
    WW = d3;
}


INLINE Mat4::Mat4(const real d)
{ 
    XX = d;
    XY = d;
    XZ = d;
    XW = d;
    
    YX = d;
    YY = d;
    YZ = d;
    YW = d;
    
    
    ZX = d;
    ZY = d;
    ZZ = d;
    ZW = d;
    
    WX = d;
    WY = d;
    WZ = d;
    WW = d;
}

INLINE Mat4::Mat4(const Mat4& n)
{
    XX = n.XX;
    XY = n.XY;
    XZ = n.XZ;
    XW = n.XW;
    
    YX = n.YX;
    YY = n.YY;
    YZ = n.YZ;
    YW = n.YW;
    
    ZX = n.ZX;
    ZY = n.ZY;
    ZZ = n.ZZ;
    ZW = n.ZW;
    
    WX = n.WX;
    WY = n.WY;
    WZ = n.WZ;
    WW = n.WW;
    
    
    
}

INLINE void Mat4::set(const Mat4& n)
{
    XX = n.XX;
    XY = n.XY;
    XZ = n.XZ;
    XW = n.XW;
    
    YX = n.YX;
    YY = n.YY;
    YZ = n.YZ;
    YW = n.YW;
    
    ZX = n.ZX;
    ZY = n.ZY;
    ZZ = n.ZZ;
    ZW = n.ZW;
    
    WX = n.WX;
    WY = n.WY;
    WZ = n.WZ;
    WW = n.WW;
}

/*
real Mat4::operator () ( const int i, const int j) const
{
    return row[i][j];
}

real & Mat4::operator () ( const int i, const int j) 
{
    return row[i][j];
} */


// ASSIGNMENT OPERATORS

INLINE Mat4& Mat4::operator = ( const Mat4& n )
{ 
    XX = n.XX;
    XY = n.XY;
    XZ = n.XZ;
    XW = n.XW;
    
    YX = n.YX;
    YY = n.YY;
    YZ = n.YZ;
    YW = n.YW;
    
    ZX = n.ZX;
    ZY = n.ZY;
    ZZ = n.ZZ;
    ZW = n.ZW;
    
    WX = n.WX;
    WY = n.WY;
    WZ = n.WZ;
    WW = n.WW;
    
    return *this; 
}



INLINE Mat4& Mat4::operator += ( const Mat4& n )
{ 
    XX += n.XX;
    XY += n.XY;
    XZ += n.XZ;
    XW += n.XW;
    
    YX += n.YX;
    YY += n.YY;
    YZ += n.YZ;
    YW += n.YW;
    
    ZX += n.ZX;
    ZY += n.ZY;
    ZZ += n.ZZ;
    ZW += n.ZW;
    
    WX += n.WX;
    WY += n.WY;
    WZ += n.WZ;
    WW += n.WW;
    
    return *this; 
}

INLINE Mat4& Mat4::operator -= ( const Mat4& n )
{
    XX -= n.XX;
    XY -= n.XY;
    XZ -= n.XZ;
    XW -= n.XW;
    
    YX -= n.YX;
    YY -= n.YY;
    YZ -= n.YZ;
    YW -= n.YW;
    
    ZX -= n.ZX;
    ZY -= n.ZY;
    ZZ -= n.ZZ;
    ZW -= n.ZW;
    
    WX -= n.WX;
    WY -= n.WY;
    WZ -= n.WZ;
    WW -= n.WW;
    return *this; 
}

INLINE Mat4& Mat4::operator *= ( const real d )
{ 
    XX *= d;
    XY *= d;
    XZ *= d;
    XW *= d;
    
    YX *= d;
    YY *= d;
    YZ *= d;
    YW *= d;
    
    
    ZX *= d;
    ZY *= d;
    ZZ *= d;
    ZW *= d;
    
    WX *= d;
    WY *= d;
    WZ *= d;
    WW *= d;
    return *this; 
}

INLINE Mat4& Mat4::operator /= ( const real d )
{ 
    Float64 u = 1.0 / d;
    
    XX *= u;
    XY *= u;
    XZ *= u;
    XW *= u;
    
    YX *= u;
    YY *= u;
    YZ *= u;
    YW *= u;
    
    
    ZX *= u;
    ZY *= u;
    ZZ *= u;
    ZW *= u;
    
    WX *= u;
    WY *= u;
    WZ *= u;
    WW *= u;
    
    return *this; 
}


INLINE void Mat4::Transpose() 
{
    Mat4 t;
    
    t.XX = XX;
    t.XY = XY;
    t.XZ = XZ;
    t.XW = XW;
    
    t.YX = YX;
    t.YY = YY;
    t.YZ = YZ;
    t.YW = YW;
    
    t.ZX = ZX;
    t.ZY = ZY;
    t.ZZ = ZZ;
    t.ZW = ZW;
    
    t.WX = WX;
    t.WY = WY;
    t.WZ = WZ;
    t.WW = WW;
    
    
    
    // swap i,j
    XX = t.XX;
    XY = t.YX;
    XZ = t.ZX;
    XW = t.WX;
    
    YX = t.XY;
    YY = t.YY;
    YZ = t.ZY;
    YW = t.WY;
    
    ZX = t.XZ;
    ZY = t.YZ;
    ZZ = t.ZZ;
    ZW = t.WZ;
    
    WX = t.XW;
    WY = t.YW;
    WZ = t.ZW;
    WW = t.WW;
    
}




// FRIENDS
// unary negate
INLINE Mat4 operator - (const Mat4& n)
{ 
    Mat4 b;
    
    b.XX = -n.XX;
    b.XY = -n.XY;
    b.XZ = -n.XZ;
    b.XW = -n.XW;
    
    b.YX = -n.YX;
    b.YY = -n.YY;
    b.YZ = -n.YZ;
    b.YW = -n.YW;
    
    b.ZX = -n.ZX;
    b.ZY = -n.ZY;
    b.ZZ = -n.ZZ;
    b.ZW = -n.ZW;
    
    b.WX = -n.WX;
    b.WY = -n.WY;
    b.WZ = -n.WZ;
    b.WW = -n.WW;
    
    return b;
}

INLINE Mat4 operator + (const Mat4& a, const Mat4& b)
{ 
    
    Mat4 c;
    
    c.XX = a.XX + b.XX;
    c.XY = a.XY + b.XY;
    c.XZ = a.XZ + b.XZ;
    c.XW = a.XW + b.XW;
    
    c.YX = a.YX + b.YX;
    c.YY = a.YY + b.YY;
    c.YZ = a.YZ + b.YZ;
    c.YW = a.YW + b.YW;
    
    c.ZX = a.ZX + b.ZX;
    c.ZY = a.ZY + b.ZY;
    c.ZZ = a.ZZ + b.ZZ;
    c.ZW = a.ZW + b.ZW;
    
    c.WX = a.WX + b.WX;
    c.WY = a.WY + b.WY;
    c.WZ = a.WZ + b.WZ;
    c.WW = a.WW + b.WW;
    
    return c; 
    
}

INLINE Mat4 operator - (const Mat4& a, const Mat4& b)
{ 
    Mat4 c;
    
    c.XX = a.XX - b.XX;
    c.XY = a.XY - b.XY;
    c.XZ = a.XZ - b.XZ;
    c.XW = a.XW - b.XW;
    
    c.YX = a.YX - b.YX;
    c.YY = a.YY - b.YY;
    c.YZ = a.YZ - b.YZ;
    c.YW = a.YW - b.YW;
    
    c.ZX = a.ZX - b.ZX;
    c.ZY = a.ZY - b.ZY;
    c.ZZ = a.ZZ - b.ZZ;
    c.ZW = a.ZW - b.ZW;
    
    c.WX = a.WX - b.WX;
    c.WY = a.WY - b.WY;
    c.WZ = a.WZ - b.WZ;
    c.WW = a.WW - b.WW;
    
    return c;
}


INLINE void Mat4::SetIdentity()
{
    XX = 1;
    XY = 0;
    XZ = 0;
    XW = 0;
    
    YX = 0;
    YY = 1;
    YZ = 0;
    YW = 0;
    
    ZX = 0;
    ZY = 0;
    ZZ = 1;
    ZW = 0;
    
    WX = 0;
    WY = 0;
    WZ = 0;
    WW = 1;
}


INLINE void Mat4::SetToZero()
{
    XX = 0;
    XY = 0;
    XZ = 0;
    XW = 0;
    
    YX = 0;
    YY = 0;
    YZ = 0;
    YW = 0;
    
    ZX = 0;
    ZY = 0;
    ZZ = 0;
    ZW = 0;
    
    WX = 0;
    WY = 0;
    WZ = 0;
    WW = 0;
}

INLINE Mat4 operator * (const Mat4& a, const Mat4& b) 
{
    Mat4 c;
    
    
    c.XX = a.XX * b.XX + a.YX * b.XY + a.ZX * b.XZ + a.WX * b.XW;
    c.XY = a.XY * b.XX + a.YY * b.XY + a.ZY * b.XZ + a.WY * b.XW;
    c.XZ = a.XZ * b.XX + a.YZ * b.XY + a.ZZ * b.XZ + a.WZ * b.XW;
    c.XW = a.XW * b.XX + a.YW * b.XY + a.ZW * b.XZ + a.WW * b.XW;
    
    c.YX = a.XX * b.YX + a.YX * b.YY + a.ZX * b.YZ + a.WX * b.YW;
    c.YY = a.XY * b.YX + a.YY * b.YY + a.ZY * b.YZ + a.WY * b.YW;
    c.YZ = a.XZ * b.YX + a.YZ * b.YY + a.ZZ * b.YZ + a.WZ * b.YW;
    c.YW = a.XW * b.YX + a.YW * b.YY + a.ZW * b.YZ + a.WW * b.YW;
    
    c.ZX = a.XX * b.ZX + a.YX * b.ZY + a.ZX * b.ZZ + a.WX * b.ZW;
    c.ZY = a.XY * b.ZX + a.YY * b.ZY + a.ZY * b.ZZ + a.WY * b.ZW;
    c.ZZ = a.XZ * b.ZX + a.YZ * b.ZY + a.ZZ * b.ZZ + a.WZ * b.ZW;
    c.ZW = a.XW * b.ZX + a.YW * b.ZY + a.ZW * b.ZZ + a.WW * b.ZW;
    

    c.WX = a.XX * b.WX + a.YX * b.WY + a.ZX * b.WZ + a.WX * b.WW;
    c.WY = a.XY * b.WX + a.YY * b.WY + a.ZY * b.WZ + a.WY * b.WW;
    c.WZ = a.XZ * b.WX + a.YZ * b.WY + a.ZZ * b.WZ + a.WZ * b.WW;
    c.WW = a.XW * b.WX + a.YW * b.WY + a.ZW * b.WZ + a.WW * b.WW;                                                                  
    
    return c;
    
    
}

INLINE Mat4 operator * (const Mat4& a, const real d)
{ 
    Mat4 b;
    b.XX = a.XX * d;
    b.XY = a.XY * d;
    b.XZ = a.XZ * d;
    b.XW = a.XW * d;
    
    b.YX = a.YX * d;
    b.YY = a.YY * d;
    b.YZ = a.YZ * d;
    b.YW = a.YW * d;
    
    b.ZX = a.ZX * d;
    b.ZY = a.ZY * d;
    b.ZZ = a.ZZ * d;
    b.ZW = a.ZW * d;
    
    b.WX = a.WX * d;
    b.WY = a.WY * d;
    b.WZ = a.WZ * d;
    b.WW = a.WW * d;
    
    return b;
}

INLINE Mat4 operator * (const real d, const Mat4& a)
{ return a*d; }


INLINE Vec3 operator * (const Vec3& v3, const Mat4& m4)
{
    // multiply v3 as though it were v4 with w = 1	
    Vec4 v4;
    Vec3 ret;
    v4.x = v3.x;
    v4.y = v3.y;
    v4.z = v3.z;
    v4.w = 1.0f;
    v4 = v4 * m4;
    v4.Homogenize();
    
    ret.x = v4.x;
    ret.y = v4.y;
    ret.z = v4.z;
    return( ret );
}


INLINE Mat4 operator / (const Mat4& a, const real d)
{
    Mat4 b;
    real u = 1 / d;
    b.XX = a.XX * u;
    b.XY = a.XY * u;
    b.XZ = a.XZ * u;
    b.XW = a.XW * u;
    
    b.YX = a.YX * u;
    b.YY = a.YY * u;
    b.YZ = a.YZ * u;
    b.YW = a.YW * u;
    
    b.ZX = a.ZX * u;
    b.ZY = a.ZY * u;
    b.ZZ = a.ZZ * u;
    b.ZW = a.ZW * u;
    
    b.WX = a.WX * u;
    b.WY = a.WY * u;
    b.WZ = a.WZ * u;
    b.WW = a.WW * u;
    
    return b;
}

INLINE int operator == (const Mat4& a, const Mat4& b)
{ 
    return (
        (b.XX == a.XX) &&
        (b.XY == a.XY) &&
        (b.XZ == a.XZ) &&
        (b.XW == a.XW) &&
        
        (b.YX == a.YX) &&
        (b.YY == a.YY) &&
        (b.YZ == a.YZ) &&
        (b.YW == a.YW) &&
        
        (b.ZX == a.ZX) &&
        (b.ZY == a.ZY) &&
        (b.ZZ == a.ZZ) &&
        (b.ZW == a.ZW) &&
        
        (b.WX == a.WX) &&
        (b.WY == a.WY) &&
        (b.WZ == a.WZ) &&
        (b.WW == a.WW));
}

INLINE int operator != (const Mat4& a, const Mat4& b)
{ return !(a == b); }

INLINE ostream& operator << (ostream& s, Mat4& a)
{ 
    return s 
        << a.XX << " " << a.XY << " " << a.XZ << " " << a.XW <<'\n' \
        << a.YX << " " << a.YY << " " << a.YZ << " " << a.YW <<'\n' \
        << a.ZX << " " << a.ZY << " " << a.ZZ << " " << a.ZW <<'\n' \
        << a.WX << " " << a.WY << " " << a.WZ << " " << a.WW <<'\n';
}

INLINE void Mat4::SetRotateXDeg(real angle)
{
    SetIdentity();  
    
    real sin_angle, cos_angle;
    sin_angle = ulsin (angle);
    cos_angle = ulcos (angle);
    sin_angle = ulsin (angle);
    cos_angle = ulcos (angle);
    
    m(Y,Y) = cos_angle;
    m(Y,Z) = sin_angle;
    m(Z,Y) = -sin_angle;
    m(Z,Z) = cos_angle;
}


INLINE void Mat4::RotateXDeg(real angle,bool post)
{
    Mat4 Rx;
    Rx.SetRotateXDeg(angle);
    
    if (post)
        *this = *this * Rx;
    else
        *this = Rx * *this;
    
}


INLINE void Mat4::SetRotateYDeg(real angle)
{
    
    real sin_angle, cos_angle;
    
    SetIdentity();  
    
    sin_angle = ulsin (angle);
    cos_angle = ulcos (angle);
    m(X, X) = cos_angle;
    m(X, Z) = -sin_angle;
    m(Z, X) = sin_angle;
    m(Z, Z) = cos_angle;
}




INLINE void Mat4::RotateYDeg(real angle, bool post)
{
    Mat4 Ry;
    Ry.SetRotateYDeg(angle);  
    
    if (post)
        *this = *this * Ry;
    else
        *this = Ry * *this;
    
}



INLINE void Mat4::SetRotateZDeg(real angle)
{
    real sin_angle, cos_angle;
    
    SetIdentity();  
    
    sin_angle = ulsin (angle);
    cos_angle = ulcos (angle);
    m(X, X) = cos_angle;
    m(X, Y) = sin_angle;
    m(Y, X) = -sin_angle;
    m(Y, Y) = cos_angle;
    
}


INLINE void Mat4::RotateZDeg(real angle, bool post)
{
    Mat4 Rz;
    Rz.SetRotateZDeg(angle);  
    
    if (post)
        *this = *this * Rz;
    else
        *this = Rz * *this;
    
}


INLINE void Mat4::SetScale(const Vec3 & v)
{
    SetIdentity();  
    
    m(X, X) = v.x;
    m(Y, Y) = v.y;
    m(Z, Z) = v.z;
}         

INLINE void Mat4::SetScale(real sx, real sy, real sz)
{
    SetIdentity();  
    
    m(X, X) = sx;
    m(Y, Y) = sy;
    m(Z, Z) = sz;
}         


INLINE void Mat4::Scale(real sx, real sy, real sz, bool post)
{
    Mat4 S;
    S.SetScale(sx,sy,sz);  
    
    if (post)
        *this = *this * S;
    else
        *this = S * *this;
    
}


INLINE void Mat4::Scale(const Vec3 & v, bool post)
{
    Mat4 S;
    S.SetScale(v);  
    
    if (post)
        *this = *this * S;
    else
        *this = S * *this;
}


INLINE void Mat4::SetTranslate(const real &tx, const real &ty, const real &tz)
{
    SetIdentity();  
    
    m(W, X) = tx;
    m(W, Y) = ty;
    m(W, Z) = tz;
}         

INLINE void Mat4::SetTranslate(const Vec3 & v)
{
    SetIdentity();  
    
    m(W, X) = v.x;
    m(W, Y) = v.y;
    m(W, Z) = v.z;
}         





INLINE void Mat4::Translate(const real &tx, const real &ty, const real &tz, bool post)
{
    Mat4 T;
    T.SetTranslate(tx,ty,tz);  
    
    if (post)
        *this = *this * T;
    else
        *this = T * *this;
    
}


INLINE void Mat4::Translate(const Vec3 & tvec, bool post)
{
    Mat4 T;
    T.SetTranslate(tvec);  
    if (post)
        *this = *this * T;
    else
        *this = T * *this;
    
}








INLINE void Mat4::Invert()
{
    ulInvertMatrix(*this);
    
}


INLINE void Mat4::SetInverse(const Mat4 & q)
{
    ulInvertMatrix(q, *this);
}

INLINE HRESULT Mat4::SetInverseArbitrary(const Mat4 & q)
{
    return ulInvertMatrixArbitrary(q, *this);
}





INLINE void Mat4::GetInverse(Mat4 & q) const
{
    ulInvertMatrix(*this, q);
}

INLINE HRESULT Mat4::GetInverseArbitrary(Mat4 & q) const
{
    return ulInvertMatrixArbitrary(*this, q);
}



INLINE void Mat4::GetTranspose(Mat4 & a) const
{
    
    a.XX = XX;
    a.XY = YX;
    a.XZ = ZX;
    a.XW = WX;
    
    a.YX = XY;
    a.YY = YY;
    a.YZ = ZY;
    a.YW = WY;
    
    a.ZX = XZ;
    a.ZY = YZ;
    a.ZZ = ZZ;
    a.ZW = WZ;
    
    a.WX = XW;
    a.WY = YW;
    a.WZ = ZW;
    a.WW = WW;
}







INLINE HRESULT Mat4::CalcRotationMatrixFromRN(Vec3 vr, Vec3 vpn, const Vec3 &p1)
{
    
    Vec3 vup;
    
    HRESULT hr = vup.MakeOrthonormalBasis(vr, vpn);
    if (hr != 0)
        return hr;
    
    CalcRotationMatrixFromAllVectors(vpn, vup, vr, p1);
    return 0;
}

INLINE HRESULT Mat4::CalcRotationMatrixFromRN(Vec3 vr, Vec3 vpn)
{
    Vec3 vup;
    HRESULT hr = vup.MakeOrthonormalBasis(vr, vpn);
    if (hr != 0)
        return hr;
    
    CalcRotationMatrixFromAllVectors(vpn, vup, vr);
    return 0;
}



INLINE HRESULT Mat4::CalcRotationMatrixFromNU(Vec3 vpn, Vec3 vup, const Vec3 &p1)
{
    
    Vec3 vr;
    HRESULT hr = vr.MakeOrthonormalBasis(vpn, vup);
    if (hr != 0)
        return hr;
    
    CalcRotationMatrixFromAllVectors(vpn, vup, vr, p1);
    
    return 0;
}

INLINE HRESULT Mat4::CalcRotationMatrixFromNU(Vec3 vpn, Vec3 vup)
{
    Vec3 vr;
    HRESULT hr = vr.MakeOrthonormalBasis(vpn, vup);
    if (hr != 0)
        return hr;
    
    CalcRotationMatrixFromAllVectors(vpn, vup, vr);
    
    return 0;
}



INLINE HRESULT Mat4::CalcRotationMatrixFromUR(Vec3 vup, Vec3 vr, const Vec3 &p1)
{
    
    Vec3 vpn;
    
    HRESULT hr = vpn.MakeOrthonormalBasis(vup, vr);
    if (hr != 0)
        return hr;
    
    CalcRotationMatrixFromAllVectors(vpn, vup, vr, p1);
    
    return 0;
}


INLINE HRESULT Mat4::CalcRotationMatrixFromUR(Vec3 vup, Vec3 vr)
{
    
    Vec3 vpn;
    
    HRESULT hr = vpn.MakeOrthonormalBasis(vup, vr);
    if (hr != 0)
        return hr;
    
    CalcRotationMatrixFromAllVectors(vpn, vup, vr);
    
    return 0;
}




INLINE void Mat4::CalcRotationMatrixFromAllVectors(const Vec3 & vpn, const Vec3 &vup, const Vec3 &vr)
{
    CalcRotationMatrixFromAllVectors(vpn, vup, vr, Vec3(0,0,0));
}

INLINE void Mat4::CalcRotationMatrixFromAllVectors(const Vec3 & vpn, const Vec3 &vup, const Vec3 &vr, const Vec3 &pos)
{
    m(X, UL_R) = vr[X];
    m(Y, UL_R) = vr[Y];
    m(Z, UL_R) = vr[Z];
    m(W, UL_R) = -(vr DOT pos);
    
    m(X, UL_U) = vup[X];
    m(Y, UL_U) = vup[Y];
    m(Z, UL_U) = vup[Z];
    m(W, UL_U) = -(vup DOT pos);
    
    m(X, UL_N) = vpn[X];
    m(Y, UL_N) = vpn[Y];
    m(Z, UL_N) = vpn[Z];
    m(W, UL_N) = -(vpn DOT pos);
    
    m(X, W) = 0;
    m(Y, W) = 0;
    m(Z, W) = 0;
    m(W, W) = 1;
}

INLINE void Mat4::Normalize3x3Vectors()
{
    // normalize each of the 3 vectors of the 3x3 subsection of the matrix
    Vec3 temp;
    
    temp.set( m(X, UL_R), m(Y, UL_R), m(Z, UL_R) );
    temp.normalize();
    m(X, UL_R) = temp.x; m(Y, UL_R) = temp.y; m(Z, UL_R) = temp.z;
    
    temp.set( m(X, UL_U), m(Y, UL_U), m(Z, UL_U) );
    temp.normalize();
    m(X, UL_U) = temp.x; m(Y, UL_U) = temp.y; m(Z, UL_U) = temp.z;
    
    temp.set( m(X, UL_N), m(Y, UL_N), m(Z, UL_N) );
    temp.normalize();
    m(X, UL_N) = temp.x; m(Y, UL_N) = temp.y; m(Z, UL_N) = temp.z;
}


INLINE void Mat4::SetTranslationPoint(const Vec3 &pos)
{
    m(W, UL_R) = -(m(X, UL_R) * pos.x + m(Y, UL_R) * pos.y + m(Z, UL_R) * pos.z);
    m(W, UL_U) = -(m(X, UL_U) * pos.x + m(Y, UL_U) * pos.y + m(Z, UL_U) * pos.z);
    m(W, UL_N) = -(m(X, UL_N) * pos.x + m(Y, UL_N) * pos.y + m(Z, UL_N) * pos.z);
}            



INLINE HRESULT Mat4::CalcRotationMatrixFromPoints(const Vec3 & p1, const Vec3 & p2, const Vec3 & p3)
{
    
    Vec3 vpn, vr;
    
    vpn[X] = p2[X] - p1[X];
    vpn[Y] = p2[Y] - p1[Y];
    vpn[Z] = p2[Z] - p1[Z];
    
    vr[X] = p1[X] - p3[X];
    vr[Y] = p1[Y] - p3[Y];
    vr[Z] = p1[Z] - p3[Z];
    
    return CalcRotationMatrixFromRN(vr, vpn, p1);
}




INLINE HRESULT Mat4::CalcProjectionMatrixDeg(real fFOV_degrees, real fAspect,
                                      real Znear, real Zfar )
{
    real c,s,Q,d, a;
    
    if( (Zfar-Znear) < UL_EPS )
        return ULERR_CANTMAKEMATRIX;
    
    c = ulcos( fFOV_degrees/2.0f);
    s = ulsin( fFOV_degrees/2.0f);
    
    Q = Zfar / ( Zfar - Znear );
    d = c / s;
    a = d * fAspect;
    
    
    if (Q == 1.0)
    {
        set(
            a, 0, 0, 0,
            0, d, 0, 0,
            0, 0, 1, 1,
            0, 0,  -Znear, 0);
    }
    else
    {
        set(
            a, 0, 0, 0,
            0, d, 0, 0,
            0, 0, Q, 1,
            0, 0,  -Q * Znear, 0);
    }
    return 0;
}



INLINE void Mat4::CalcRotationMatrixAboutLine(const Vec3 &pos, const real &angle, const Vec3 &vpn)
{
    // rot is now align with y
    Mat4 inv;
    Vec3 vup(0,1,0);
    
    
    if (ulAbs(vpn.x) < UL_EPS && ulAbs(vpn.y) < UL_EPS)
    {
        vup.x = 0;
        vup.y = 1;
        vup.z = 0;
    }
    
    CalcRotationMatrixFromNU(vpn, vup, pos);
    
    GetInverse(inv);
    
    RotateZDeg(angle);
    
    *this = inv * *this;
}



INLINE Mat3 Mat4::rot()
{
    Mat3 a;
    a.XX = XX;
    a.XY = YX;
    a.XZ = ZX;
    
    a.YX = XY;
    a.YY = YY;
    a.YZ = ZY;
    
    a.ZX = XZ;
    a.ZY = YZ;
    a.ZZ = ZZ;
    
    return a;
}


INLINE void Mat4::SetRotation(const Mat3 & a)
{
    XX = a.XX;
    XY = a.YX;
    XZ = a.ZX;
    
    YX = a.XY;
    YY = a.YY;
    YZ = a.ZY;
    
    ZX = a.XZ;
    ZY = a.YZ;
    ZZ = a.ZZ;
}



INLINE real Mat4::xformAxis(const Vec3 &v, int axis) const
{
    return m(X, axis) * v.x + m(Y, axis) * v.y + m(Z, axis) * v.z;
}

INLINE Vec3 Mat4::trans() const
{
    Vec3 t;
    t.x = m(W, 0);
    t.y = m(W, 1);
    t.z = m(W, 2);
    return t;
}

INLINE Mat4 & Mat4::GetMat4() 
{
    return *this;
}


INLINE Vec3 Mat4::GetViewNormal()
{
    return Vec3(XZ, YZ, ZZ);
}
INLINE Vec3 Mat4::GetViewUp()
{
    return Vec3(XY, YY, ZY);
}
INLINE Vec3 Mat4::GetViewRight()
{
    return Vec3(XX, YX, ZX);
}

INLINE Vec3 Mat4::GetTranslation()
{
    return Vec3(m(W, 0), m(W, 1), m(W, 2));
}

INLINE void Mat4::GetTranslation(Vec3 & v)
{
    v.x = m(W, 0);
    v.y = m(W, 1);
    v.z = m(W, 2);
}               

INLINE void Mat4::SetTranslation(const Vec3 & v)
{
    m(W, 0) = v.x;
    m(W, 1) = v.y;
    m(W, 2) = v.z;
}         


// Code adapted from VecLib4d.c in Graphics Gems V
INLINE Vec4 cross(const Vec4& a, const Vec4& b, const Vec4& c)
{
    Vec4 result;

    double d1 = (b[Z] * c[W]) - (b[W] * c[Z]);
    double d2 = (b[Y] * c[W]) - (b[W] * c[Y]);
    double d3 = (b[Y] * c[Z]) - (b[Z] * c[Y]);
    double d4 = (b[X] * c[W]) - (b[W] * c[X]);
    double d5 = (b[X] * c[Z]) - (b[Z] * c[X]);
    double d6 = (b[X] * c[Y]) - (b[Y] * c[X]);

    result[X] = - a[Y] * d1 + a[Z] * d2 - a[W] * d3;
    result[Y] =   a[X] * d1 - a[Z] * d4 + a[W] * d5;
    result[Z] = - a[X] * d2 + a[Y] * d4 - a[W] * d6;
    result[W] =   a[X] * d3 - a[Y] * d5 + a[Z] * d6;

    return result;
}





INLINE Mat4 Mat4::adjoint() const
{
    Mat4 A;

    A.row[0] = cross( row[1], row[2], row[3]);
    A.row[1] = cross(-row[0], row[2], row[3]);
    A.row[2] = cross( row[0], row[1], row[3]);
    A.row[3] = cross(-row[0], row[1], row[2]);
        
    return A;
}



#endif   // #ifndef __NVIDIA_ULMAT4_H_44444__
