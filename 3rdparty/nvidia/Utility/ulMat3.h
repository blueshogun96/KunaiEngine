/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulMat3.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/


#ifndef __NVIDIA_ULMAT3_H__M3M3M3M3__
#define __NVIDIA_ULMAT3_H__M3M3M3M3__


#include <ctype.h>
#include "ulCommon.h"
#include "ulVec3.h"



#define XX row[0].x
#define XY row[0].y
#define XZ row[0].z
#define XW row[0].w

#define YX row[1].x
#define YY row[1].y
#define YZ row[1].z
#define YW row[1].w

#define ZX row[2].x
#define ZY row[2].y
#define ZZ row[2].z
#define ZW row[2].w

#define WX row[3].x
#define WY row[3].y
#define WZ row[3].z
#define WW row[3].w


class Mat3
{
    
public:
    /*union
    {
        real m[3][3];
        struct
        {
            real xx, xy, xz,
                yx, yy, yz,
                zx, zy, zz;
        };

        
    };*/

    Vec3 row[3];
    
public:
    
    double invert(Mat3& inv);
    Mat3 transpose();
    void clear() 
    {
        row[0].x=
            row[0].y=
            row[0].z=
            
            row[1].x=
            row[1].y=
            row[1].z=
            
            row[2].x=
            row[2].y=
            row[2].z=0;
    }


    INLINE Vec3 col(int i) const {return Vec3(row[0][i],row[1][i],row[2][i]);}


    // Constructors

    
    Mat3() {}
    Mat3(const Vec3& v0, const Vec3& v1, const Vec3& v2);
    Mat3(const real m00, const real m01, const real m02,
        const real m10, const real m11, const real m12,
        const real m20, const real m21, const real m22);
    Mat3(const real d);
    Mat3(const Mat3& m);
    
    // Assignment operators
    
    Mat3& operator  = ( const Mat3& m );      // assignment of a Mat3
    Mat3& operator += ( const Mat3& m );      // this += m
    Mat3& operator -= ( const Mat3& m );      // this -= m
    Mat3& operator *= ( const real d );      // this *= d
    Mat3& operator /= ( const real d );      // this /= d
    
    real & operator ()  ( const int i, const int j){ return row[i][j]; }
    real operator ()  ( const int i, const int j) const { return row[i][j]; }

    
    Mat3 adjoint();

    real & m(int i, int j)  { return row[i][j]; }
    real m(int i, int j) const  { return row[i][j]; }


    Vec3 GetViewUp();
    Vec3 GetViewNormal();
    Vec3 GetViewRight();
    
    // this = identity
    void SetIdentity();
    // this = transpose(this)
    void Transpose();
    // m = transpose(this)
    void GetTranspose(Mat3 & m) const;
    // this = transpose(m)
    void SetTranspose(const Mat3 & m);
    
    // this = invert(this),
    void Invert();
    // this = invert(this) for arbitrary matrix
    HRESULT InvertArbitrary();
    
    // m = inverse(this)
    void GetInverse(Mat3 & m) const;
    // m = invert(this) for arbitrary matrix
    HRESULT GetInverseArbitrary(Mat3 & m) const;
    // this = inverse(m)
    void SetInverse(const Mat3 & m);
    
    // this = inverse(m)
    HRESULT SetInverseArbitrary(const Mat3 & m);
    
    // this = 0
    void SetToZero();           // make this zero
    
    // this = rotate_x
    void SetRotateXDeg(real degrees);
    // this = rotate_x * this
    void RotateXDeg(real degrees, bool post = false);
    
    // this = rotate_z
    void SetRotateYDeg(real degrees);
    // this = rotate_y * this
    void RotateYDeg(real degrees, bool post = false);
    
    
    // this = rotate_z
    void SetRotateZDeg(real degrees);
    // this = rotate_z * this
    void RotateZDeg(real degrees, bool post = false);
    
    
    
    friend Mat3 operator ~ (const Mat3& m);         // ~m
    friend Mat3 operator - (const Mat3& m);         // -m
    friend Mat3 operator + (const Mat3& m1, const Mat3& m2);      // m1 + m2
    friend Mat3 operator - (const Mat3& m1, const Mat3& m2);      // m1 - m2
    friend Mat3 operator * (Mat3& m1, Mat3& m2);        // m1 * m2
    friend Mat3 operator * (const Mat3& m, const real d);      // m * d
    friend Mat3 operator * (const real d, const Mat3& m);      // d * m
    friend Mat3 operator / (const Mat3& m, const real d);      // m1 / d
    friend int operator == (const Mat3& m1, const Mat3& m2);      // m1 == m2 ?
    friend int operator != (const Mat3& m1, const Mat3& m2);      // m1 != m2 ?
    friend ostream& operator << (ostream& s, Mat3& m);      // output to stream
    friend istream& operator >> (istream& s, Mat3& m);      // input from strm.
    
    
    friend Vec3 operator * (const Mat3& m, const Vec3& v);      // v = m * v



	/*
	Up
	|     Normal  
	|      /
	|    /
	|  /
	|/_________ Right

  
    
      
        
			  for three points, contruct a matrix so that vpn is p1 to p2 and p3 is on the 
			  Z, X plane. For Mat3 this is only the rotation part.
          
            
              
					Y          Z
					|        p2  
					|      /
					|    /
					|  /
					|/_________  X
					p1
                
					  p3 is on X, Z plane some where
                  
    */
    
    // given two vectors, calculate a rotation matrix
    
    HRESULT CalcRotationMatrixFromUR(Vec3 vup, Vec3 vr); // up right
    HRESULT CalcRotationMatrixFromRN(Vec3 vr, Vec3 vpn); // right normal
    HRESULT CalcRotationMatrixFromNU(Vec3 vpn, Vec3 vup); // normal up
    
    // given the threee vectorsm normal, up and right
    // calc the rotation matrix
    void CalcRotationMatrixFromAllVectors(const Vec3 & vpn, const Vec3 &vup, const Vec3 &vr);
    // given three points in space, calc the rotation matrix
    HRESULT CalcRotationMatrixFromPoints(const Vec3 & p1, const Vec3 & p2, const Vec3 & p3);
    
    real det() const;
    
    void xform(const Vec3 &v, Vec3 &xv) const; // (this)(v) => xv; 
    // v & xv must be distinct
    void xform(Vec3 &v) const;                  // (this)(v) => v
    
    void invXform(const Vec3 &v, Vec3 &xv) const;
    void invXform(Vec3 &v) const;
    
    
    Mat3(const Vec3 &diag, const Vec3 &sym) {set(diag, sym);}
    // make a symmetric matrix, given the diagonal and symmetric
    // (off-diagonal) elements in canonical order
    
    void set(const Quat &q);
    void set(const Mat3& m);
    void set(const Vec3 &diag, const Vec3 &sym);
    
    void xpose(const Mat3 &M);                   // M^T      
    void xpose();                                // this^T
    void symmetrize(const Mat3 &M);              // M + M^T
    void symmetrize();                           // this + this^T
    
    //double  invert(const Mat3 &M);                  // M^-1      
    double  invert();                               // this^-1
    
    void negate(const Mat3 &M);                  // -M
    void negate();                               // -this
    void add(const Mat3 &M, const Mat3 &N);      // M + N
    void add(const Mat3 &M);                     // this + M
    void sub(const Mat3 &M, const Mat3 &N);      // M - N
    void sub(const Mat3 &M);                     // this - M
    void scale(const Mat3 &M, real s);           // s * M
    void scale(real s);                          // s * this
    void mult(const Mat3 &M, const Mat3 &N);     // M * N     
    void mult(const Mat3 &M, bool  post = false);                 // M * this  
    //void postmult(const Mat3 &M);                // this * M  
    
    // for reading a symmetric matrix
    Vec3 diag() const {return Vec3(XX, YY, ZZ);}
    Vec3 sym()  const {return Vec3(YZ, ZX, XY);}
    
    // set matrix to the skew symmetric matrix corresponding to 'v X'
    void setSkew(const Vec3 &v);
    
    // for reading rows
    const Vec3 &xrow() const {return *((Vec3 *) &XX);}
    const Vec3 &yrow() const {return *((Vec3 *) &YX);}
    const Vec3 &zrow() const {return *((Vec3 *) &ZX);}
    // for writing to rows
    Vec3 &xrow()  {return *((Vec3 *) &XX);}
    Vec3 &yrow()  {return *((Vec3 *) &YX);}
    Vec3 &zrow()  {return *((Vec3 *) &ZX);}
    
    // for reading columns
    Vec3 xcol() const {return Vec3(XX, YX, ZX);}
    Vec3 ycol() const {return Vec3(XY, YY, ZY);}
    Vec3 zcol() const {return Vec3(XZ, YZ, ZZ);}
    // for writing to columns
    void setXcol(const Vec3 &v);
    void setYcol(const Vec3 &v);
    void setZcol(const Vec3 &v);
    
    ostream& print(ostream &os) const;
};


INLINE ostream &operator<<(ostream &os, const Mat3 &M)  {return M.print(os);}
#include "ulCommon.h"
#include "ulVec4.h"
#include "ulQuat.h"

#include <ctype.h>


/****************************************************************
*								                                                *
*		    Mat3 member functions			                              *
*								                                                *
****************************************************************/

INLINE
void ulInvertMatrix(const Mat3 & a, Mat3 & b)  
{

  b(0, 0) = a(0, 0);
  b(1, 0) = a(0, 1);
  b(2, 0) = a(0, 2);
	                
  b(0, 1) = a(1, 0);
  b(1, 1) = a(1, 1);
  b(2, 1) = a(1, 2);
	          	     
  b(0, 2) = a(2, 0);
  b(1, 2) = a(2, 1);
  b(2, 2) = a(2, 2);
}

INLINE
void ulInvertMatrix(Mat3 & a)
{
  Mat3 b;
  ulInvertMatrix(a, b);
  a = b;

}



INLINE
HRESULT ulInvertMatrixArbitrary(const Mat3 &a, Mat3 & b)  
{

  double fDetInv = a(0, 0) * ( a(1, 1) * a(2, 2) - a(1, 2) * a(2, 1) ) -
    a(0, 1) * ( a(1, 0) * a(2, 2) - a(1, 2) * a(2, 0) ) +
    a(0, 2) * ( a(1, 0) * a(2, 1) - a(1, 1) * a(2, 0) );
  

  if (ulAbs(fDetInv) < UL_EPS)
  {
    b.SetIdentity();
    return UL_CANTINVERTMATRIX;
  }
  fDetInv = 1.0 /fDetInv;
  b(0, 0) =  fDetInv * ( a(1, 1) * a(2, 2) - a(1, 2) * a(2, 1) );
  b(0, 1) = -fDetInv * ( a(0, 1) * a(2, 2) - a(0, 2) * a(2, 1) );
  b(0, 2) =  fDetInv * ( a(0, 1) * a(1, 2) - a(0, 2) * a(1, 1) );
  
  b(1, 0) = -fDetInv * ( a(1, 0) * a(2, 2) - a(1, 2) * a(2, 0) );
  b(1, 1) =  fDetInv * ( a(0, 0) * a(2, 2) - a(0, 2) * a(2, 0) );
  b(1, 2) = -fDetInv * ( a(0, 0) * a(1, 2) - a(0, 2) * a(1, 0) );
  
  b(2, 0) =  fDetInv * ( a(1, 0) * a(2, 1) - a(1, 1) * a(2, 0) );
  b(2, 1) = -fDetInv * ( a(0, 0) * a(2, 1) - a(0, 1) * a(2, 0) );
  b(2, 2) =  fDetInv * ( a(0, 0) * a(1, 1) - a(0, 1) * a(1, 0) );
  
  return 0;
}

INLINE
HRESULT ulInvertMatrixArbitrary(Mat3 & a)
{
  HRESULT res;
  Mat3 b;
  res = ulInvertMatrixArbitrary(a, b);
  a = b;
  return res;
}


INLINE
Mat3::Mat3(const Vec3& v0, const Vec3& v1, const Vec3& v2)
{ 
  m(0, 0) = v0(0);
  m(0, 1) = v0(1);
  m(0, 2) = v0(2);
               
  m(1, 0) = v1(0);
  m(1, 1) = v1(1);
  m(1, 2) = v1(2);
               
  m(2, 0) = v2(0);
  m(2, 1) = v2(1);
  m(2, 2) = v2(2);

}

INLINE
Mat3::Mat3(const real d)
{ 
  m(0, 0) = d;
  m(0, 1) = d;
  m(0, 2) = d;
    
  m(1, 0) = d;
  m(1, 1) = d;
  m(1, 2) = d;
    
  m(2, 0) = d;
  m(2, 1) = d;
  m(2, 2) = d;
}

INLINE
Mat3::Mat3(const Mat3& n)
{ 
  m(0, 0) = n(0, 0);
  m(0, 1) = n(0, 1);
  m(0, 2) = n(0, 2);
    
  m(1, 0) = n(1, 0);
  m(1, 1) = n(1, 1);
  m(1, 2) = n(1, 2);
    
  m(2, 0) = n(2, 0);
  m(2, 1) = n(2, 1);
  m(2, 2) = n(2, 2);
}


// ASSIGNMENT OPERATORS

INLINE
Mat3::Mat3( const real m00,  const real m01,  const real m02,
            const real m10,  const real m11,  const real m12,
            const real m20,  const real m21,  const real m22)
{                                            
  m(0, 0) = m00;
  m(0, 1) = m01;
  m(0, 2) = m02;
    
  m(1, 0) = m10;
  m(1, 1) = m11;
  m(1, 2) = m12;
    
  m(2, 0) = m20;
  m(2, 1) = m21;
  m(2, 2) = m22;
}
 

INLINE
Mat3& Mat3::operator = ( const Mat3& n )
{
  m(0, 0) = n(0, 0);
  m(0, 1) = n(0, 1);
  m(0, 2) = n(0, 2);
    
  m(1, 0) = n(1, 0);
  m(1, 1) = n(1, 1);
  m(1, 2) = n(1, 2);
    
  m(2, 0) = n(2, 0);
  m(2, 1) = n(2, 1);
  m(2, 2) = n(2, 2);
 
  
  return *this; 
}

INLINE
Mat3& Mat3::operator += ( const Mat3& n )
{
  m(0, 0) += n(0, 0);
  m(0, 1) += n(0, 1);
  m(0, 2) += n(0, 2);
    
  m(1, 0) += n(1, 0);
  m(1, 1) += n(1, 1);
  m(1, 2) += n(1, 2);
    
  m(2, 0) += n(2, 0);
  m(2, 1) += n(2, 1);
  m(2, 2) += n(2, 2);
 
  return *this; 
}

INLINE
Mat3& Mat3::operator -= ( const Mat3& n )
{ 
  m(0, 0) -= n(0, 0);
  m(0, 1) -= n(0, 1);
  m(0, 2) -= n(0, 2);
    
  m(1, 0) -= n(1, 0);
  m(1, 1) -= n(1, 1);
  m(1, 2) -= n(1, 2);
    
  m(2, 0) -= n(2, 0);
  m(2, 1) -= n(2, 1);
  m(2, 2) -= n(2, 2);
 
  return *this; 
}

INLINE
Mat3& Mat3::operator *= ( const real d )
{
  m(0, 0) *= d;
  m(0, 1) *= d;
  m(0, 2) *= d;
    
  m(1, 0) *= d;
  m(1, 1) *= d;
  m(1, 2) *= d;
    
  m(2, 0) *= d;
  m(2, 1) *= d;
  m(2, 2) *= d;
  
  return *this; 
}

INLINE
Mat3& Mat3::operator /= ( const real d )
{ 
  real u = 1/d;
  m(0, 0) *= u;
  m(0, 1) *= u;
  m(0, 2) *= u;
    
  m(1, 0) *= u;
  m(1, 1) *= u;
  m(1, 2) *= u;
    
  m(2, 0) *= u;
  m(2, 1) *= u;
  m(2, 2) *= u;
  

  return *this;
}





INLINE
void Mat3::Transpose() 
{
  real tmp;

  tmp = XY;
  XY = YX;
  YX = tmp;

  tmp = YZ;
  YZ = ZY;
  ZY = tmp;

  tmp = ZX;
  ZX = XZ;
  XZ = tmp;
}





INLINE
Mat3 operator - (const Mat3& a)
{

  Mat3 b;
  b(0, 0) = -a(0, 0);
  b(0, 1) = -a(0, 1);
  b(0, 2) = -a(0, 2);

  
  b(1, 0) = -a(1, 0);
  b(1, 1) = -a(1, 1);
  b(1, 2) = -a(1, 2);

  
  b(2, 0) = -a(2, 0);
  b(2, 1) = -a(2, 1);
  b(2, 2) = -a(2, 2);
 
  return b;
}

INLINE
Mat3 operator + (const Mat3& a, const Mat3& b)
{ 
  Mat3 c;

  c(0, 0) = a(0, 0) + b(0, 0);
  c(0, 1) = a(0, 1) + b(0, 1);
  c(0, 2) = a(0, 2) + b(0, 2);
  
  c(1, 0) = a(1, 0) + b(1, 0);
  c(1, 1) = a(1, 1) + b(1, 1);
  c(1, 2) = a(1, 2) + b(1, 2);
  
  c(2, 0) = a(2, 0) + b(2, 0);
  c(2, 1) = a(2, 1) + b(2, 1);
  c(2, 2) = a(2, 2) + b(2, 2);
 
  return c;
}

INLINE
Mat3 operator - (const Mat3& a, const Mat3& b)
{ 
  Mat3 c;

  c(0, 0) = a(0, 0) - b(0, 0);
  c(0, 1) = a(0, 1) - b(0, 1);
  c(0, 2) = a(0, 2) - b(0, 2);
  
  c(1, 0) = a(1, 0) - b(1, 0);
  c(1, 1) = a(1, 1) - b(1, 1);
  c(1, 2) = a(1, 2) - b(1, 2);
  
  c(2, 0) = a(2, 0) - b(2, 0);
  c(2, 1) = a(2, 1) - b(2, 1);
  c(2, 2) = a(2, 2) - b(2, 2);
  return c;
}


INLINE
Mat3 operator * (const Mat3& a, const real d)
{ 
  Mat3 b;

  b(0, 0) = a(0, 0) * d;
  b(0, 1) = a(0, 1) * d;
  b(0, 2) = a(0, 2) * d;

  b(1, 0) = a(1, 0) * d;
  b(1, 1) = a(1, 1) * d;
  b(1, 2) = a(1, 2) * d;

  b(2, 0) = a(2, 0) * d;
  b(2, 1) = a(2, 1) * d;
  b(2, 2) = a(2, 2) * d;
 
  
  return b;
}


INLINE
Mat3 operator / (const Mat3& a, const real d)
{
  Mat3 b;
  Float64 u = 1.0 / d;

  b(0, 0) = a(0, 0) * u;
  b(0, 1) = a(0, 1) * u;
  b(0, 2) = a(0, 2) * u;

  b(1, 0) = a(1, 0) * u;
  b(1, 1) = a(1, 1) * u;
  b(1, 2) = a(1, 2) * u;

  b(2, 0) = a(2, 0) * u;
  b(2, 1) = a(2, 1) * u;
  b(2, 2) = a(2, 2) * u;
 
  return b;
}

INLINE
int operator == (const Mat3& a, const Mat3& b)
{
  return (
    b(0, 0) == a(0, 0) &&
    b(0, 1) == a(0, 1) &&
    b(0, 2) == a(0, 2) &&
    
    b(1, 0) == a(1, 0) &&
    b(1, 1) == a(1, 1) &&
    b(1, 2) == a(1, 2) &&
    
    b(2, 0) == a(2, 0) &&
    b(2, 1) == a(2, 1) &&
    b(2, 2) == a(2, 2));

}

INLINE
int operator != (const Mat3& a, const Mat3& b)
{ return !(a == b); }

INLINE
ostream& operator << (ostream& s, Mat3& a)
{ 
  return s << a(0, 0) << " " << a(0, 1) << " " << a(0, 2) <<'\n' \
    << a(1, 0) << " " << a(1, 1) << " " << a(1, 2) <<'\n'
    << a(2, 0) << " " << a(2, 1) << " " << a(2, 2) <<'\n';

}


INLINE
void Mat3::SetIdentity()
{
  m(0, 0) = 1;
  m(0, 1) = 0;
  m(0, 2) = 0;
    
  m(1, 0) = 0;
  m(1, 1) = 1;
  m(1, 2) = 0;
    
  m(2, 0) = 0;
  m(2, 1) = 0;
  m(2, 2) = 1;

}


INLINE
void Mat3::SetToZero()
{
  m(0, 0) = 0;
  m(0, 1) = 0;
  m(0, 2) = 0;

  m(1, 0) = 0;
  m(1, 1) = 0;
  m(1, 2) = 0;


  m(2, 0) = 0;
  m(2, 1) = 0;
  m(2, 2) = 0;

}



INLINE
void Mat3::Invert()
{
  ulInvertMatrix(*this);
  
}



INLINE
void Mat3::GetInverse(Mat3 & a) const
{
  ulInvertMatrix(*this, a);
}

INLINE
void Mat3::SetInverse(const Mat3 & a)
{
  ulInvertMatrix(a, *this);
}

INLINE
HRESULT Mat3::SetInverseArbitrary(const Mat3 & a)
{
  return ulInvertMatrixArbitrary(a, *this);
}






INLINE
Mat3 operator ~(const Vec3& a)
{
  return Mat3(   0, -a.z,  a.y,  
               a.z,    0, -a.x, 
              -a.y,  a.x,    0); 

}



INLINE
Mat3 operator * (Mat3& a, Mat3& b) 
{
  Mat3 c;
  int i,j,k;

  c.SetToZero();

  for( i=0; i<3; i++ ) 
    for( j=0; j<3; j++ ) 
      for( k=0; k<3; k++ ) 
        c(i, j) += a(k, j) * b(i, k);
      
  return c;
}


INLINE
void Mat3::SetRotateXDeg(real angle)
{
    
  real sin_angle, cos_angle;

  SetIdentity();

  sin_angle = ulsin (angle);
  cos_angle = ulcos (angle);
  sin_angle = ulsin (angle);
  cos_angle = ulcos (angle);

  m(Y, Y) = cos_angle;
  m(Y, Z) = sin_angle;
  m(Z, Y) = -sin_angle;
  m(Z, Z) = cos_angle;
}

INLINE
void Mat3::RotateXDeg(real angle, bool post)
{
    
  Mat3 Rx;
  Rx.SetRotateXDeg(angle);

  if (post)
    *this = *this * Rx;
  else
    *this = Rx * *this;
}


INLINE
void Mat3::SetRotateYDeg(real angle)
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



INLINE
void Mat3::RotateYDeg(real angle, bool post)
{
  Mat3 Ry;
  Ry.SetRotateYDeg(angle);
  if (post)
    *this = *this * Ry;
  else
    *this = Ry * *this;
}



INLINE
void Mat3::SetRotateZDeg(real angle)
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


INLINE
void Mat3::RotateZDeg(real angle, bool post)
{

  Mat3 Rz;
  Rz.SetRotateZDeg(angle);

  if (post)
    *this = *this * Rz;
  else
    *this = Rz * *this; 
}






INLINE
void Mat3::GetTranspose(Mat3 & a) const 
{

  a(0, 0) = m(0, 0);
  a(1, 0) = m(0, 1);
  a(2, 0) = m(0, 2);
	               
  a(0, 1) = m(1, 0);
  a(1, 1) = m(1, 1);
  a(2, 1) = m(1, 2);
		              
  a(0, 2) = m(2, 0);
  a(1, 2) = m(2, 1);
  a(2, 2) = m(2, 2);
}

INLINE
void Mat3::SetTranspose(const Mat3 & a) 
{

  m(0, 0) = a(0, 0);
  m(1, 0) = a(0, 1);
  m(2, 0) = a(0, 2);
	                
  m(0, 1) = a(1, 0);
  m(1, 1) = a(1, 1);
  m(2, 1) = a(1, 2);
	          	     
  m(0, 2) = a(2, 0);
  m(1, 2) = a(2, 1);
  m(2, 2) = a(2, 2);
}                  



// p1 at 0
// p2 on z
// p3 x,y
/*
   left handed,
   z into plane
*/

INLINE
HRESULT Mat3::CalcRotationMatrixFromPoints(const Vec3 & p1, const Vec3 & p2, const Vec3 & p3)
{
 
  Vec3 vpn, vup, vr;

  vpn[X] = p2[X] - p1[X];
  vpn[Y] = p2[Y] - p1[Y];
  vpn[Z] = p2[Z] - p1[Z];

  vr[X] = p3[X] - p1[X];
  vr[Y] = p3[Y] - p1[Y];
  vr[Z] = p3[Z] - p1[Z];


  return CalcRotationMatrixFromRN(vr, vpn);
}



INLINE
HRESULT Mat3::CalcRotationMatrixFromRN(Vec3 vr, Vec3 vpn)
{
  HRESULT res;

  Vec3 vup;

  res = vup.MakeOrthonormalBasis(vr, vpn);
  if (res)
    return res;
 
  CalcRotationMatrixFromAllVectors(vpn, vup, vr);

  return 0;
}


INLINE
HRESULT Mat3::CalcRotationMatrixFromUR(Vec3 vup, Vec3 vr)
{
  HRESULT res;
  Vec3 vpn;

  res = vpn.MakeOrthonormalBasis(vup, vr);
 
  CalcRotationMatrixFromAllVectors(vpn, vup, vr);
  return 0;

}

INLINE
HRESULT Mat3::CalcRotationMatrixFromNU(Vec3 vpn, Vec3 vup)
{
  HRESULT res;
  Vec3 vr;

  res = vr.MakeOrthonormalBasis(vpn, vup);
 
  CalcRotationMatrixFromAllVectors(vpn, vup, vr);
  return 0;

}





INLINE
void Mat3::CalcRotationMatrixFromAllVectors(const Vec3 & vpn, const Vec3 & vup,
                                            const Vec3 & vr)
{
  m(X, UL_R) = vr[X];
  m(Y, UL_R) = vr[Y];
  m(Z, UL_R) = vr[Z];

  m(X, UL_U) = vup[X];
  m(Y, UL_U) = vup[Y];
  m(Z, UL_U) = vup[Z];

  m(X, UL_N) = vpn[X];
  m(Y, UL_N) = vpn[Y];
  m(Z, UL_N) = vpn[Z];
 
}




INLINE
void Mat3::xform(const Vec3 &v, Vec3 &xv) const
{
  xv.x = XX * v.x + XY * v.y + XZ * v.z;
  xv.y = YX * v.x + YY * v.y + YZ * v.z;
  xv.z = ZX * v.x + ZY * v.y + ZZ * v.z;
}


INLINE
void Mat3::xform(Vec3 &v) const
{
  real ox, oy;

  ox = v.x; oy= v.y;
  v.x = XX * ox + XY * oy + XZ * v.z;
  v.y = YX * ox + YY * oy + YZ * v.z;
  v.z = ZX * ox + ZY * oy + ZZ * v.z;
}


INLINE
void Mat3::invXform(const Vec3 &v, Vec3 &xv) const
{
  xv.x = XX * v.x + YX * v.y + ZX * v.z;
  xv.y = XY * v.x + YY * v.y + ZY * v.z;
  xv.z = XZ * v.x + YZ * v.y + ZZ * v.z;
}


INLINE
void Mat3::invXform(Vec3 &v) const
{
  real ox, oy;

  ox = v.x; oy= v.y;
  v.x = XX * ox + YX * oy + ZX * v.z;
  v.y = XY * ox + YY * oy + ZY * v.z;
  v.z = XZ * ox + YZ * oy + ZZ * v.z;
}



INLINE
void Mat3::set(const Quat &q)
{
  XX = 2.0f * (q.s * q.s + q.i * q.i - 0.5f);
  YY = 2.0f * (q.s * q.s + q.j * q.j - 0.5f);
  ZZ = 2.0f * (q.s * q.s + q.k * q.k - 0.5f);

  XY = 2.0f * (q.j * q.i - q.k * q.s);
  YX = 2.0f * (q.i * q.j + q.k * q.s);


  YZ = 2.0f * (q.k * q.j - q.i * q.s);
  ZY = 2.0f * (q.j * q.k + q.i * q.s);

  ZX = 2.0f * (q.i * q.k - q.j * q.s);
  XZ = 2.0f * (q.k * q.i + q.j * q.s);
}



INLINE
void Mat3::mult(const Mat3 &M, const Mat3 &N)
{
  XX = M.XX * N.XX + M.XY * N.YX + M.XZ * N.ZX;
  XY = M.XX * N.XY + M.XY * N.YY + M.XZ * N.ZY;
  XZ = M.XX * N.XZ + M.XY * N.YZ + M.XZ * N.ZZ;
  YX = M.YX * N.XX + M.YY * N.YX + M.YZ * N.ZX;
  YY = M.YX * N.XY + M.YY * N.YY + M.YZ * N.ZY;
  YZ = M.YX * N.XZ + M.YY * N.YZ + M.YZ * N.ZZ;
  ZX = M.ZX * N.XX + M.ZY * N.YX + M.ZZ * N.ZX;
  ZY = M.ZX * N.XY + M.ZY * N.YY + M.ZZ * N.ZY;
  ZZ = M.ZX * N.XZ + M.ZY * N.YZ + M.ZZ * N.ZZ;
}


INLINE
void Mat3::mult(const Mat3 &M, bool post)
{
    
    real oxy, oyz, ozx, oyx, ozy, oxz;
    if (post == false)
    {
        oxy = XY; oyx = YX; oyz = YZ; ozy = ZY; ozx = ZX; oxz = XZ;
        
        XY = M.XX * oxy + M.XY * YY  + M.XZ * ozy;
        XZ = M.XX * oxz + M.XY * oyz + M.XZ * ZZ;
        YX = M.YX * XX  + M.YY * oyx + M.YZ * ozx;
        YZ = M.YX * oxz + M.YY * oyz + M.YZ * ZZ;
        ZX = M.ZX * XX  + M.ZY * oyx + M.ZZ * ozx;
        ZY = M.ZX * oxy + M.ZY * YY  + M.ZZ * ozy;
        
        XX = M.XX * XX  + M.XY * oyx + M.XZ * ozx;
        YY = M.YX * oxy + M.YY * YY  + M.YZ * ozy;
        ZZ = M.ZX * oxz + M.ZY * oyz + M.ZZ * ZZ;
    }
    else
    {
        
        //real oxy, oyz, ozx, oyx, ozy, oxz;
        
        oxy = XY; oyx = YX; oyz = YZ; ozy = ZY; ozx = ZX; oxz = XZ;
        
        XY = XX *  M.XY + oxy * M.YY + oxz * M.ZY;
        XZ = XX *  M.XZ + oxy * M.YZ + oxz * M.ZZ;
        YX = oyx * M.XX + YY  * M.YX + oyz * M.ZX;
        YZ = oyx * M.XZ + YY  * M.YZ + oyz * M.ZZ;
        ZX = ozx * M.XX + ozy * M.YX + ZZ  * M.ZX;
        ZY = ozx * M.XY + ozy * M.YY + ZZ  * M.ZY;
        
        XX = XX  * M.XX + oxy * M.YX + oxz * M.ZX;
        YY = oyx * M.XY + YY  * M.YY + oyz * M.ZY;
        ZZ = ozx * M.XZ + ozy * M.YZ + ZZ  * M.ZZ;
    }
}



INLINE
void Mat3::xpose(const Mat3 &M)
{
  XX = M.XX;
  XY = M.YX;
  XZ = M.ZX;

  YX = M.XY;
  YY = M.YY;
  YZ = M.ZY;

  ZX = M.XZ;
  ZY = M.YZ;
  ZZ = M.ZZ;
}


INLINE
void Mat3::xpose()
{
  real tmp;

  tmp = XY;
  XY = YX;
  YX = tmp;

  tmp = YZ;
  YZ = ZY;
  ZY = tmp;

  tmp = ZX;
  ZX = XZ;
  XZ = tmp;
}




INLINE
void Mat3::set(const Vec3 &diag, const Vec3 &sym)
{
  XX = diag.x;
  YY = diag.y;
  ZZ = diag.z;
  YZ = ZY = sym.x;
  ZX = XZ = sym.y;
  XY = YX = sym.z;
}

INLINE
void Mat3::setXcol(const Vec3 &v)
{
  XX = v.x;
  YX = v.y;
  ZX = v.z;
}


INLINE
void Mat3::setYcol(const Vec3 &v)
{
  XY = v.x;
  YY = v.y;
  ZY = v.z;
}


INLINE
void Mat3::setZcol(const Vec3 &v)
{
  XZ = v.x;
  YZ = v.y;
  ZZ = v.z;
}


INLINE
void Mat3::setSkew(const Vec3 &v)
{
  XX = YY = ZZ = 0.0;
  ZY =  v.x;
  YZ = -v.x;
  XZ =  v.y;
  ZX = -v.y;
  YX =  v.z;
  XY = -v.z;
}


INLINE
real Mat3::det() const
{
  return  XX * (YY * ZZ - YZ * ZY)
        + XY * (YZ * ZX - YX * ZZ)
        + XZ * (YX * ZY - YY * ZX);
}



INLINE
ostream& Mat3::print(ostream &os) const
{
  int oldFlags = os.setf(ios::showpos);
  os << '[' << XX << ' ' << XY << ' ' << XZ << ')' << endl;
  os << '[' << YX << ' ' << YY << ' ' << YZ << ')' << endl;
  os << '[' << ZX << ' ' << ZY << ' ' << ZZ << ')' << endl;
  os.flags(oldFlags);
  return os;
}







/*Mat3 operator * (const real d, const Mat3& a)
{ 
} */



INLINE
Vec3 Mat3::GetViewNormal()
{
  return Vec3(XZ, YZ, ZZ);
}
INLINE
Vec3 Mat3::GetViewUp()
{
  return Vec3(XY, YY, ZY);
}
INLINE
Vec3 Mat3::GetViewRight()
{
  return Vec3(XX, YX, ZX);
}


INLINE
Mat3 Mat3::adjoint()
{
    return Mat3(row[1]^row[2],
		row[2]^row[0],
		row[0]^row[1]);
}

INLINE
Mat3 Mat3::transpose()
{
    return Mat3(col(0), col(1), col(2));
}

INLINE
double Mat3::invert(Mat3& inv)
{
    Mat3 A = adjoint();
    double d = A.row[0] DOT row[0];

    if( d==0.0 )
	    return 0.0;

    inv = A.transpose() / d;
    return d;
}

// invert self
INLINE
double Mat3::invert()
{
    Mat3 inv;
    double d;
    d = invert(inv);
    
    m(0, 0) = inv(0, 0);
    m(0, 1) = inv(0, 1);
    m(0, 2) = inv(0, 2);
                     
    m(1, 0) = inv(1, 0);
    m(1, 1) = inv(1, 1);
    m(1, 2) = inv(1, 2);
                     
                     
    m(2, 0) = inv(2, 0);
    m(2, 1) = inv(2, 1);
    m(2, 2) = inv(2, 2);
    
    return d;

}



#endif   // #ifndef __NVIDIA_ULMAT3_H__M3M3M3M3__
