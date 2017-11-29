/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulMatQ.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:

  Quaternion + Translation Vector

******************************************************************************/



#ifndef __NVIDIA_ULMATQ_H__QQQQQQQ__
#define __NVIDIA_ULMATQ_H__QQQQQQQ__


#include <ctype.h>
#include "ulCommon.h"
#include "ulVec3.h"
#include "ulQuat.h"


class MatQ 
{
    
    friend class MatR;
    
    //private:
public:
    
    Quat q;     // rotation component
    Vec3 d;    // translation component
    
public:
    
    // constructors //////////////////////////////////////////////////////////////
    
    
    MatQ() {}
    MatQ(const Quat &q_, const Vec3 &d_) {set(q_, d_);}
    MatQ(const MatR &X) {set(X);}
    
    // setters / accessors / translators /////////////////////////////////////////
    
    void set(const Quat &q_, const Vec3 &d_) {q = q_; d = d_;}
    void set(const MatR &X) {q.set(X.R); d = X.d;}
    
    const Quat  &rot()   const {return q;}
    const Vec3 &trans() const {return d;}
    Quat  &rot()         {return q;}
    Vec3 &trans()       {return d;}
    
    
    //ostream& print(ostream &os) const;
    
    void mult(const MatQ &T, const MatQ &U);    // T * U     
    void mult(const MatQ &T, bool post = false);               // T * this  
    //void postmult(const MatQ &T);              // this * T 
    
    void SetIdentity();
    void Invert();                            // this^-1
    void InvertArbitrary();                            // this^-1
    void InvertAffine();                            // this^-1
    void GetInverse(MatQ &T) const;                // T^-1
    void GetInverseArbitrary(MatQ &T) const;                // T^-1
    void GetInverseAffine(MatQ &T) const;                // T^-1
    void SetInverse(const MatQ &T);
    void SetInverseArbitrary(const MatQ &T);
    
    
    // Transforming Vec3s ///////////////////////////////////////////////////////
    
    // MatQs can transform elements of R^3 either as vectors or as
    // points.  Multiple operands need not be distinct.
    
    void xformVect(const Vec3 &v, Vec3 &xv) const; 
    
    real xformAxis(const Vec3 &v, int axis) const;
    
    void xformVect(Vec3 &v) const;       // this * (v 0) => v
    void xformPoint(const Vec3 &p, Vec3 &xp) const; // this * (p 1) => xp
    void xformPoint(Vec3 &p) const;      // this * (p 1) => p
    
    // These are exactly like the above methods, except the inverse
    // transform this^-1 is used.
    void invXformVect(const Vec3 &v, Vec3 &xv) const;
    void invXformVect(Vec3 &v) const;                 
    void invXformPoint(const Vec3 &p, Vec3 &xp) const;
    void invXformPoint(Vec3 &p) const;                 
    
    ostream& print(ostream &os) const;
    
    void SetTranslate(real x, real y, real z);
    void SetTranslate(const Vec3 & v);
    void SetRotation(const Mat3 & m);
    
    HRESULT CalcRotationMatrixFromPoints(const Vec3 & p1, const Vec3 & p2, const Vec3 & p3);
    
    HRESULT CalcRotationMatrixFromRN(Vec3 vr, Vec3 vpn);
    HRESULT CalcRotationMatrixFromRN(Vec3 vr, Vec3 vpn, const Vec3 &p1);
    
    HRESULT CalcRotationMatrixFromNU(Vec3 vpn, Vec3 vup);
    HRESULT CalcRotationMatrixFromNU(Vec3 vpn, Vec3 vup, const Vec3 &p1);
    
    HRESULT CalcRotationMatrixFromUR(Vec3 vup, Vec3 vr);
    HRESULT CalcRotationMatrixFromUR(Vec3 vup, Vec3 vr, const Vec3 &p1);
    
    
    void CalcRotationMatrixFromAllVectors(const Vec3 & vpn, const Vec3 &vup, const Vec3 &vr, const Vec3 &pos);
        
};


INLINE ostream &operator<<(ostream &os, const MatQ &T)  {return T.print(os);}



#endif   // #ifndef __NVIDIA_ULMATQ_H__QQQQQQQ__
