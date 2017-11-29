/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulMatR.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:

	3 x 3 Rotation + Translation vector 

******************************************************************************/

#ifndef __NVIDIA_ULMATR_H__
#define __NVIDIA_ULMATR_H__


#include <ctype.h>
#include "ulCommon.h"
#include "ulMat3.h"
#include "ulVec3.h"


class MatR 
{
    
    friend class MatQ;
    
public:
    
    Mat3 R;
    Vec3 d;
    
public:
    
    // constructors //////////////////////////////////////////////////////////////
    
    MatR()                                {}
    MatR(const Mat3 &R_, const Vec3 &d_) {set(R_, d_);}
    MatR(const MatQ &T)                    {set(T);}
    
    
    // setters / accessors / translators /////////////////////////////////////////
    
    void set(const Mat3 &R_, const Vec3 &d_) {R = R_; d = d_;}
    void set(const MatQ &T);
    void set(const Quat &q, const Vec3 & v);
    
    const Mat3  &rot()   const {return R;}
    const Vec3 &trans() const {return d;}
    
    Mat3  &rot()         {return R;}
    Vec3 &trans()       {return d;}
    
    
    Vec3 GetViewUp() { return R.GetViewUp() ;}
    void GetViewUp(Vec3 & v) { v = R.GetViewUp();}
    
    Vec3 GetViewNormal() {return R.GetViewNormal();}
    void GetViewNormal(Vec3 &v) {v = R.GetViewNormal();}
    
    Vec3 GetViewRight() { return R.GetViewRight();}
    void GetViewRight(Vec3 & v) { v = R.GetViewRight();}
    
    
    void SetRotation(const Mat3& a) { R = a;}
    
    
    friend MatR operator * (const MatR& a, const MatR& b);        // m1 * m2
    
    void SetIdentity();
    void Invert();                       
    void GetInverse(MatR &M) const;                 // M^-1     
    void SetInverse(const MatR &M);                 // M^-1      
    Mat4 GetMat4();                 // Cat
    
    void mult(const MatR &M, const MatR &N); 
    void mult(const MatR &M, bool post = false);
    //void postmult(const MatR &M);
    
    
    // Transforming Vec3s ///////////////////////////////////////////////////////
    
    
    void xformVect(const Vec3 &v, Vec3 &xv) const;   
    void xformVect(Vec3 &v) const;                 
    void xformPoint(const Vec3 &p, Vec3 &xp) const;  
    void xformPoint(Vec3 &p) const;                
    
    
    void invXformVect(const Vec3 &v, Vec3 &xv) const;
    void invXformVect(Vec3 &v) const;                 
    
    void invXformPoint(const Vec3 &p, Vec3 &xp) const;
    void invXformPoint(Vec3 &p) const;                 
    
    
    
    HRESULT CalcRotationMatrixFromPoints(const Vec3 & p1, const Vec3 & p2, const Vec3 & p3);
    
    HRESULT CalcRotationMatrixFromRN(const Vec3 & vr, const Vec3 & vpn);
    HRESULT CalcRotationMatrixFromRN(const Vec3 & vr, const Vec3 & vpn, const Vec3 &p1);
    
    HRESULT CalcRotationMatrixFromNU(const Vec3 & vpn, const Vec3 & vup);
    HRESULT CalcRotationMatrixFromNU(const Vec3 & vpn, const Vec3 & vup, const Vec3 &p1);
    
    HRESULT CalcRotationMatrixFromUR(const Vec3 & vup, const Vec3 & vr);
    HRESULT CalcRotationMatrixFromUR(const Vec3 & vup, const Vec3 & vr, const Vec3 &p1);
    
    
    HRESULT CalcRotationMatrixFromAllVectors(Vec3 & vpn, Vec3 &vup, Vec3 &vr, Vec3 &pos);
    
    void SetTranslate(const Vec3 & v);
    void SetTranslate(const real x, const real y, const real z);
    void AddTranslate(const Vec3 & v);
    void AddTranslate(const real x, const real y, const real z);
    
    ostream& print(ostream &os) const;
    real xformAxis(const Vec3 &v, int axis) const;
};


INLINE ostream &operator<<(ostream &os, const MatR &X)  {return X.print(os);}


#endif   // #ifndef __NVIDIA_ULMATR_H__
