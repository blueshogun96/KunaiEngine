/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulCommon.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:

Things common to all ul classes - like types & things the classes need
  to define themselves.

******************************************************************************/


#pragma warning(disable:4244)  //conversion from 'double' to 'float', possible loss of data

#ifndef __NVIDIA_ULCOMMON_H__
#define __NVIDIA_ULCOMMON_H__


// #include <ctype.h>

#include "NV_Common.h"



#ifndef STRICT
#define STRICT
#endif


// set this to the size of the floating point that you want
// default size
#ifndef FLOAT_SIZE
#define FLOAT_SIZE 32
#endif

//#define FLOAT_SIZE 64


// kill some warnings
#ifdef KILL_WARNINGS
#pragma warning(disable : 4800)     // 
#pragma warning(disable : 4101)     // 
#pragma warning(disable : 4244)     // double to real
#endif

// do not include MIN and MAX operator in the WINDOWS stuff
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <stdio.h>
#include <stdlib.h>
#ifndef _WIN32
#include <iostream.h>
#else
#include <iostream>
#endif
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <malloc.h>
#include <string.h>
#include <tchar.h>
#include <atlbase.h>




// Error code
#define ULERR_CANTMAKEMATRIX    -20000
#define ULERR_CANTNORMALIZE     -20001
#define UL_CANTINVERTMATRIX     -20002
#define ULERR_CANTMAKEVECTOR    -20003


// for vect and matric referencing
#ifndef MIX_NO_AXIS_NAMES
enum Axis {X=0, Y=1, Z=2, W=3};
#define MIX_NO_AXIS_NAMES
#endif


// in matrix, which column mean Right, Up, and Normal
const int UL_R = X;
const int UL_U = Y;
const int UL_N = Z;

// in local coordinates, which is on the plane (RX,RY) and which is up (RZ)
// makes the local 2D calculations easier
const int  RX = X;
const int  RY = Z;
const int  RZ = Y;


#define UL_PI      3.1415926535897932384626433832795028841971693993751
#define UL_SQRT_2  1.4142135623730950488016887242096980785696718753769
#define UL_PI2     (UL_PI/2.0)


#define UL_DEG2RAD     (UL_PI / 180.0)
//0.01745329251994
#define UL_RAD2DEG     (180.0 / UL_PI)
//57.29577951308232

#define ulDeg2Rad(x)   ((x) * UL_DEG2RAD)
#define ulRad2Deg(x)   ((x) * UL_RAD2DEG) 


#define UL_EPS 1e-12
#define UL_EPS2 (UL_EPS * UL_EPS)






#if (FLOAT_SIZE==32)
typedef float Float;
typedef float real;
//#define UL_MAX_FLOAT 3.40282346638528860e+38
#define UL_MAX_FLOAT  1.0e+38f

#else
#define UL_MAX_FLOAT 1e+300
typedef double Float;
typedef double real;
#endif



typedef float				Float32;
typedef float				real32;
typedef double				Float64;
typedef double				real64;

typedef long				Int; 

typedef unsigned char		uchar;
typedef unsigned char		byte;


typedef unsigned int		uint;

typedef unsigned __int64	uLong; 
typedef __int64				Long; 


// Utility Library CComPtr
// Taken from atlbase.h and added a Clear function


template <class T>
class ulComPtr
{
public:
    typedef T _PtrClass;


    T		*	p;			// pointer thing! =)


    ulComPtr()
    {
        p=NULL;
    }
    ulComPtr(T* lp)
    {
        if ((p = lp) != NULL)
            p->AddRef();
    }
    ulComPtr(const ulComPtr<T>& lp)
    {
        if ((p = lp.p) != NULL)
            p->AddRef();
    }
    ~ulComPtr()
    {
        if (p)
            p->Release();
    }
    void Release()
    {
        IUnknown* pTemp = p;
        if (pTemp)
        {
            p = NULL;
            pTemp->Release();
        }
    }
    void Clear()
    {
        p=NULL;
    }
    void Kill()
    {
        if (p)
            while(p->Release())
                ;
            p=NULL;
    }
    
    operator T*() const
    {
        return (T*)p;
    }
    T& operator*() const
    {
        ATLASSERT(p!=NULL);
        return *p;
    }
    //The assert on operator& usually indicates a bug.  If this is really
    //what is needed, however, take the address of the p member explicitly.
    T** operator&()
    {
        ATLASSERT(p==NULL);
        return &p;
    }
    _NoAddRefReleaseOnCComPtr<T>* operator->() const
    {
        ATLASSERT(p!=NULL);
        return (_NoAddRefReleaseOnCComPtr<T>*)p;
    }
    T* operator=(T* lp)
    {
        return (T*)AtlComPtrAssign((IUnknown**)&p, lp);
    }
    T* operator=(const ulComPtr<T>& lp)
    {
        return (T*)AtlComPtrAssign((IUnknown**)&p, lp.p);
    }
    bool operator!() const
    {
        return (p == NULL);
    }
    bool operator<(T* pT) const
    {
        return p < pT;
    }
    bool operator==(T* pT) const
    {
        return p == pT;
    }
    // Compare two objects for equivalence
    bool IsEqualObject(IUnknown* pOther)
    {
        if (p == NULL && pOther == NULL)
            return true; // They are both NULL objects
        
        if (p == NULL || pOther == NULL)
            return false; // One is NULL the other is not
        
        ulComPtr<IUnknown> punk1;
        ulComPtr<IUnknown> punk2;
        p->QueryInterface(IID_IUnknown, (void**)&punk1);
        pOther->QueryInterface(IID_IUnknown, (void**)&punk2);
        return punk1 == punk2;
    }
    void Attach(T* p2)
    {
        if (p)
            p->Release();
        p = p2;
    }
    T* Detach()
    {
        T* pt = p;
        p = NULL;
        return pt;
    }
    HRESULT CopyTo(T** ppT)
    {
        ATLASSERT(ppT != NULL);
        if (ppT == NULL)
            return E_POINTER;
        *ppT = p;
        if (p)
            p->AddRef();
        return S_OK;
    }
    HRESULT SetSite(IUnknown* punkParent)
    {
        return AtlSetChildSite(p, punkParent);
    }
    HRESULT Advise(IUnknown* pUnk, const IID& iid, LPDWORD pdw)
    {
        return AtlAdvise(p, pUnk, iid, pdw);
    }
    
    HRESULT CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
    {
        ATLASSERT(p == NULL);
        return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
    }
    HRESULT CoCreateInstance(LPCOLESTR szProgID, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
    {
        CLSID clsid;
        HRESULT hr = CLSIDFromProgID(szProgID, &clsid);
        ATLASSERT(p == NULL);
        if (SUCCEEDED(hr))
            hr = ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
        return hr;
    }
    
    
    template <class Q>
        HRESULT QueryInterface(Q** pp) const
    {
        ATLASSERT(pp != NULL && *pp == NULL);
        return p->QueryInterface(__uuidof(Q), (void**)pp);
    }
//    T* p;
};


// Straight pointer class with no AddRef/Release mechanism
// not currently used

template <class _Type>
class ulPtr
{
public:
    typedef _Type _PtrClass;
    ulPtr(const ulPtr<_Type>& lp)
    {
        p = lp.p;
    }
    ~ulPtr()
    {
        delete this;
    }
    operator _Type*() const
    {
        return p;
    }
    _Type& operator*() const
    {
        ATLASSERT(p!=NULL);
        return *p;
    }
    //The assert on operator& usually indicates a bug.  If this is really
    //what is needed, however, take the address of the p member explicitly.
    _Type** operator&()
    {
        ATLASSERT(p==NULL);
        return &p;
    }
    
    _Type* operator->() const
    {
        ATLASSERT(p!=NULL);
        return p;
    }
    
    _Type* operator=(_Type* lp)
    {
        p = lp;
        return p;
    }
    _Type* operator=(const ulPtr<_Type>& lp)
    {
        p = lp;
        return p;
    }
    bool operator!() const
    {
        return (p == NULL);
    }
    bool operator<(_Type* pT) const
    {
        return p < pT;
    }
    bool operator==(_Type* pT) const
    {
        return p == pT;
    }
    // Compare two objects for equivalence
    bool IsEqualObject(IUnknown* pOther)
    {
        if (p == NULL && pOther == NULL)
            return true; // They are both NULL objects
        
        if (p == NULL || pOther == NULL)
            return false; // One is NULL the other is not
        
        return p == pOther;
    }
    _Type* p;
};


// Floating point acces macros


// access as DWORD
#define FP_BITS(fp) (*(DWORD *)&(fp))
// Clear MSb
#define FP_ABS_BITS(fp) (FP_BITS(fp)&0x7FFFFFFF)
// Get MSb
#define FP_SIGN_BIT(fp) (FP_BITS(fp)&0x80000000)
// Floating pt 1.0
#define FP_ONE_BITS 0x3F800000



void ulBuildInvSqrtTable();



/*#ifdef FAST_MATH
	INLINE real64 ulsqrt(real64 x) 
	{
		return ulsqrt( x );
	}


	INLINE real64 ulrsqrt(real64 x)
	{
		return ulrsqrt( x );
	}

	INLINE real32 ulsqrt(real32 x) 
	{
		return ulsqrt((real32)x);
	}

	INLINE real32 ulrsqrt(real32 x)
	{
		return ulrsqrt((real32)x);
	}
#else
  */
	INLINE real64 ulsqrt(real64 x) 
	{
		return sqrt(x);
	}

	INLINE real64 ulrsqrt(real64 x)
	{
		return 1.0 / sqrt(x);
	}

	INLINE real32 ulsqrt(real32 x) 
	{
		return (real32)sqrt(x);
	}

	INLINE real32 ulrsqrt(real32 x)
	{
		return (real32)(1.0 / sqrt(x));
	}

//#endif






// function pointer, used to apply a user function to each element in vector
typedef real (*ulFuncPtrf)(const real &);
typedef int   (*ulFuncPtri)(const int &);
typedef void  (*ulFuncPtrb)(const bool &);
typedef void  (*ulFuncPtrv)(void);

// derivative callback for ODE, 
typedef void (*dydt_func)(void * tope, real t, real y[], real * ydot);

// forward references
class Mat3;
class Mat4;
class MatR;
class MatQ;

//class Vec2;
class Vec3;
class Vec4;
class Vec2;

class Quat;

class Plane;


typedef Vec2 TextureCoords2;
typedef Vec3 TextureCoords3;
typedef Vec4 TextureCoords4;


// 32 bit walking one
extern DWORD power_table[32];


// dot product
#define DOT &

// cross product
#define CP %


////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// macros



/* ulnormal.cpp */

template < class T > INLINE HRESULT ulNormalize(T &x, T &y, T &z)
{
    T u;
    u = x * x + y * y + z * z;
    
//    if (ulAbs(u - 1.0) < UL_EPS)
  //      return 0; // already normalized
    if ( fabs(u - 1.0) < UL_EPS)
        return 0; // already normalized
    
    if ( fabs(u) < UL_EPS2)
    {
        x = y = z = 0.0;
        return ULERR_CANTNORMALIZE;
    }


    u = (real)ulrsqrt(u);
    
    /*if (ulAbs(u) < UL_EPS2)
    {
        x = y = z = 0.0;
        return ULERR_CANTNORMALIZE;
    }
    u = 1 / u;*/
    x *= u;
    y *= u;
    z *= u;
    return 0;
}

/*
INLINE HRESULT ulNormalize( real32 & x, real32 & y, real32 & z )
{
	real32 u;
	u = x * x + y * y + z * z;

	if( fabs(u - 1.0f) < UL_EPS )
		return( S_OK );			// already normalized

	if( fabs(u) < UL_EPS2 )
	{
		x = y = z = 0.0f;
		return( ULERR_CANTNORMALIZE );
	}


//@    T u;
    u = x * x + y * y + z * z;
    
    if (ulAbs(u - 1.0) < UL_EPS)
        return 0; // already normalized
    
    if (ulAbs(u) < UL_EPS2)
    {
        x = y = z = 0.0;
        return ULERR_CANTNORMALIZE;
    }


    u = (real)ulrsqrt(u);
    


    x *= u;
    y *= u;
    z *= u;
    return 0;
}
*/


template < class T > INLINE HRESULT ulNormalize(T &x, T &y, T &z, T &w)
{
    T u;
    u = x * x + y * y + z * z + w * w;
    
    if (ulAbs(u - 1.0) < UL_EPS)
        return 0; // already normalized
    
    if (ulAbs(u) < UL_EPS2)
    {
        x = y = z = 0.0;
        w = 1;
        return ULERR_CANTNORMALIZE;
    }
    
    //u = (real)(1 / sqrt(u));
    u = ulrsqrt(u);
    
    x *= u;
    y *= u;
    z *= u;
    w *= u;
    return 0;
}


template < class T > INLINE T ulSqr (const T &x)
{
    return x * x;
}

template < class T > INLINE T ulCube(const T & x)
{
    return x * x * x;
}
template < class T > INLINE void ulSwap (T &x, T &y)
{
    T tmp;
    tmp = x; 
    x = y; 
    y = tmp;
}


INLINE real64 ulAbs (const real64 &x)
{
	return( fabs(x));
}


INLINE real32 ulAbs (const real32 & x)
{
	return( (real32)fabs(x) );
}



template < class T > INLINE T ulMin(const T &x, const T &y)
{
    if (x < y)
        return x;
    else
        return y;
}


template < class T > INLINE T ulMax(const T &x, const T &y)
{
    if (x < y)
        return y;
    else
        return x;
}

template < class T > INLINE int ulSgn(const T & x)
{
    if (x > 0)
        return 1;
    else if (x < 0)
        return -1;
    else
        return 0;
}

template < class T > INLINE int ulSgnP1(const T &x)
{
    if (x > 0)
        return 2;
    else if (x < 0)
        return 0;
    else
        return 1;
}

template < class T > INLINE T ulClamp(const T &low, const T & x, const T &high)
{
    if (x < low)
        return low;
    else if (x > high)
        return high;
    else
        return x;
}




/////////////////////////////////////////////////////////////////
// Function prototypes!

// Algebra.cpp 
void ulDisplacePoint ( Vec3 &point , Vec3 &vect , real lambda , Vec3 &result );

// ulgeom.cpp
real ulDotProduct(const Vec3 & v1, const Vec3 & v2);
Vec3 ulCrossProduct(const Vec3 & v1, const Vec3 & v2);
real ulDistanceSquared(const Vec3 & v1, const Vec3 & v2);
real ulDistance(const Vec3 & v1, const Vec3 & v2);
real ulDistance ( real x1 , real y1 , real x2 , real y2 );
real ulDistance ( real x1 , real y1 , real z1 , real x2 , real y2 , real z2 );
real ulDistance ( real x , real y );
int ulPointInPolygon2D ( int n , Vec2 &point , real poly [][2 ]);
real ulSegmentLength (  const Vec3 &a ,  const Vec3 &b );
real ulSegmentLengthSquared ( Vec3 &a , Vec3 &b );
real ulFindCircle ( Vec3 &v1 , Vec3 &v2 , Vec3 &v3 , Vec3 &circle );
real ulFindSphere ( Vec3 &p1 , Vec3 &p2 , Vec3 &p3 , Vec3 &sphere );
real ulDistancePointToSegment ( const Vec3 &p ,  const Vec3 &p1 ,  const Vec3 &p2 , int parallel );
real ulDistancePointToLine ( Vec3 &p , Vec3 &p1 , Vec3 &p2 );
real ulDot2Deg ( real dot );
real ulAngleBetweenVectorsDeg ( Vec3 &v1 , Vec3 &v2 );
void ulReflect ( Vec3 &L , Vec3 &n , Vec3 &r );
void ulReflect2 ( Vec3 &L , Vec3 &n , Vec3 &r );
real ulFlatness ( Vec3 &a , Vec3 &b , Vec3 &c , Vec3 &d , int absolute );
int ulBoxRegion ( real x , real y , real x1 , real y1 , real x2 , real y2 );
int ulClosestToBox ( real x , real y , real &rx , real &ry , real x1 , real y1 , real x2 , real y2 );
real ulDistancePointToSegment2 ( real x , real y , real x1 , real y1 , real x2 , real y2 , real linear_distance );
real ulDistancePointToLine ( real x , real y , real x1 , real y1 , real x2 , real y2 );
real ulDistancePointToSegment ( real x , real y , real x1 , real y1 , real x2 , real y2 );
real ulTensor ( real z1 , real z2 , real z3 , real z4 , real y , real x );
Int ulSamePoint ( real p1 [3 ], real p2 [3 ]);
void ulDmsToDec ( real deg , real min , real sec , char hemisphere , real *decimal_number );
void ulDecToDms ( real decimal_number , real *deg , real *min , real *sec );
void ulXform3D ( real h , real p , real r , real x0 , real y0 , real z0 , real *x , real *y , real *z );
void ulRotate2d ( real angle , real x , real y , real *xr , real *yr );
void ulRectToPolar ( real x , real y , real *r , real *angle );
void ulPolarToRect ( real r , real angle , real *x , real *y );
void ulPolarToRect2 ( real rx , real ry , real angle , real *x , real *y );
real ulFrac ( real a );
int ulWhichSegment ( real x , real y , real z , real x1 , real y1 , real z1 , real x2 , real y2 , real z2 );
int ulWhichSegment ( real x , real y , real x1 , real y1 , real x2 , real y2 );
void ulClosestPoint2D (real x , real y , real x1 , real y1 , real x2 , real y2 , 
                       real &x3 , real &y3 , real &t );
int ulSegmentIntersection ( real x0 , real y0 , real x1 , real y1 , real x2 , real y2 , real x3 , real y3 , real &xa , real &ya , real &xb , real &yb , real &r , real &s , int clamp );
void ulClosestPointToSegment ( Vec3 &p1 , Vec3 &p2 , Vec3 &v , Vec3 &p );
int ulAlmostEqual ( Vec3 &a , Vec3 &b , real eps );
void ulODE ( void *data , real y [], real yout [], int n , real t0 , real h , dydt_func dydt , real dydx [], real *dym , real *dyt , real *yt );
int ulClosestPoints ( Vec3 &p1 , Vec3 &p2 , Vec3 &p3 , Vec3 &p4 , Vec3 &o1 , Vec3 &o2 );
real ulPlaneDist ( Plane &plane , Vec3 &point );
void ulDisplacePoint ( Vec3 &point , Vec3 &vect , real lambda , Vec3 &result );

// Getopt.cpp
int _getopt_internal ( int argc , char *const *argv , const char *optstring , const struct option *longopts , int *longind , int long_only );
int getopt ( int argc , char **argv , char *optstring );

// ML_FLOAT.Cpp
void ulInitMath ( void );
real ulcos ( real angle );
real ulsin ( real angle );
real ultan ( real angle );
real ulacos ( real angle );
real ulGetAngle ( real x , real y );
real ulAnglify ( real x );
real ulAnglify180 ( real x );
real ulJustifyAngle ( real angle , real min_angle , real max_angle );
real ulatan2 ( real y , real x );
void ulBuildSqrtTable ( void );




HRESULT ulInvertMatrixArbitrary(const Mat3 &a, Mat3 & b);
HRESULT ulInvertMatrixArbitrary(Mat3 & b);
void ulInvertMatrix(const Mat3 &a, Mat3 & b);
void ulInvertMatrix(Mat3 & b);

HRESULT ulInvertMatrixArbitrary(const Mat4 &a, Mat4 & b);
HRESULT ulInvertMatrixArbitrary(Mat4 & b);
HRESULT ulInvertMatrixAffine(const Mat4 &a, Mat4 & b);
HRESULT ulInvertMatrixAffine(Mat4 & b);
void ulInvertMatrix(const Mat4 &a, Mat4 & b);
void ulInvertMatrix(Mat4 & b);

HRESULT ulInvertMatrixArbitrary(const MatQ &a, MatQ & b);
HRESULT ulInvertMatrixArbitrary(MatQ & b);
HRESULT ulInvertMatrix(const MatQ &a, MatQ & b);
HRESULT ulInvertMatrix(MatQ & b);


void ulInvertMatrix(const MatR &a, MatR & b);
void ulInvertMatrix(MatR & b);



void dprintf( LPSTR fmt, ... );


// winsupport.cpp
char *ulGetFileName ( HWND hWnd , char *fileMask );


// ppm.cpp
//FILE * ulOpenPPMFile ( char * strFilename , DWORD *pdwWidth , DWORD *pdwHeight , DWORD *pdwMaxgrey );
//


Int ulFindNormal ( const Vec3 &p0 , const Vec3 &p1 , const Vec3 &p2 , Vec3 &n , int unitize = 0 );
Int ulFindNormal (real x1, real y1, real z1, 
                  real x2, real y2, real z2, 
                  real x3, real y3, real z3, 
                  Vec3 &n , int unitize = 0 );







#endif		// #ifndef __NVIDIA_ULCOMMON_H__