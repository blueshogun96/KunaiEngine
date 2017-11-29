/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulOrientation.h

  Copyright (C) 1999, 2000 NVIDIA Corporation
  This file is provided without support, instruction, or implied warranty of any
  kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
  not liable under any circumstances for any damages or loss whatsoever arising
  from the use or inability to use this file or items derived from it.
  
    Comments:
    
      
******************************************************************************/



#ifndef __NVIDIA_ULORIENTATION_H__
#define __NVIDIA_ULORIENTATION_H__


#include <ctype.h>
#include "ulCommon.h"

#include "ulMat4.h"
#include "ulVec3.h"
#include "ulMatrixVec.h"


class ulOrientation : public Mat4
{
public:
    ulOrientation();
    virtual ~ulOrientation();
    
    void SetPosition(const Vec3 & pos);
    void SetPosition(const real &x, const real &y, const real &z);
    Vec3 GetPosition();
    void GetPosition(Vec3 & pos);
    void GetPosition(Float64 &x, Float64 &y, Float64 &z);
    void GetPosition(Float32 &x, Float32 &y, Float32 &z);
    
    void SetPivotPoint(const Vec3 & pos);
    void SetPivotPoint(const real &x, const real &y, const real &z);
    Vec3 GetPivotPoint();
    void GetPivotPoint(Vec3 & pos);
    void GetPivotPoint(Float64 &x, Float64 &y, Float64 &z);
    void GetPivotPoint(Float32 &x, Float32 &y, Float32 &z);
    
    
    void SetTarget(const real &x, const real &y, const real &z);
    void SetTarget(const Vec3 & pos);
    Vec3 GetTarget();
    void GetTarget(Vec3 & pos);
    void GetTarget(Float64 &x, Float64 &y, Float64 &z);
    void GetTarget(Float32 &x, Float32 &y, Float32 &z);
    
    void SetDirection(const real &x, const real &y, const real &z);
    void SetDirection(const Vec3 & dir);
    
    Vec3 GetDirection();
    void GetDirection(Vec3 &);
    void GetDirection (Float64 &x, Float64 &y, Float64 &z);
    void GetDirection (Float32 &x, Float32 &y, Float32 &z);
    
    void SetUp(const real &x, const real &y, const real &z);
    void SetUp(const Vec3 & dir);
    
    
    void SetRoll(const real &roll );
    const real &GetRoll() {return m.roll;}
    void GetRoll(real & r) {r = m.roll;}
    
    
    void MoveAlong(const Vec3 & n, const real &x, const real &y, const real &z);
    
    void CreateMatrixFromViewParameters();
    
    void Strafe(const real &x, const real &y, const real &z);
    void TranslatePosition(const real &x, const real &y, const real &z);
    
    void RotateXDeg( real degrees_to_rotate );
    void RotateYDeg( real degrees_to_rotate );
    void RotateZDeg( real degrees_to_rotate );
    void RotateXDeg( real degrees, bool post )  {RotateXDeg(degrees);};
    void RotateYDeg( real degrees, bool post )  {RotateYDeg(degrees);};
    void RotateZDeg( real degrees, bool post )  {RotateZDeg(degrees);};
    
    void RotateXDegLocal( real degrees_to_rotate, bool post );
    void RotateYDegLocal( real degrees_to_rotate, bool post );
    void RotateZDegLocal( real degrees_to_rotate, bool post );
    
    bool RefreshOrientation();
    Mat4 GetOrientationMatrix();
    void SetOrientationMatrix(Mat4 & mat);
    
    Vec3 &GetUserPosition() { return m.user_pos;}
    void SetUserPosition(Vec3 & pos) {m.user_pos = pos;}
    
    
    real &GetUserValue() { return m.user_value;}
    void SetUserValue(real & v) {m.user_value = v;}
    
private:
    
    typedef struct ulOrientationData
    {
        Vec3 pos;
        Vec3 pivot;
        Vec3 target;
        Vec3 Up;
        real roll;
        real dist;
        bool bViewParametersChanged;
        
        Vec3 user_pos;
        real user_value;
        
    } ulOrientationData;
    
    ulOrientationData m;
};

typedef ulOrientation * ulpOrientation;   


INLINE
void ulOrientation::SetPosition(const Vec3 & pos)
{
    m.pos = pos;
    
    m.bViewParametersChanged = true;
}





INLINE
void ulOrientation::SetPosition(const real &x, const real &y, const real &z)
{
    m.pos.x = x;
    m.pos.y = y;
    m.pos.z = z;
    
    m.bViewParametersChanged = true;
}




INLINE
void ulOrientation::GetPosition(Float64 &x, Float64 &y, Float64 &z)
{
    GetPosition();
    x  = m.pos.x;
    y  = m.pos.y;
    z  = m.pos.z;
}

INLINE
void ulOrientation::GetPosition(Float32 &x, Float32 &y, Float32 &z)
{
    GetPosition();
    
    x  = m.pos.x;
    y  = m.pos.y;
    z  = m.pos.z;
    
}

INLINE
void ulOrientation::GetPosition(Vec3 & pos)
{
    GetPosition();
    pos = m.pos;
}



INLINE
void ulOrientation::SetTarget(const real &x, const real &y, const real &z)
{
    m.target.x = x;
    m.target.y = y; 
    m.target.z = z;
    
    m.bViewParametersChanged = true;
    
}
INLINE
void ulOrientation::SetTarget(const Vec3 & target)
{
    m.target.x = target.x;
    m.target.y = target.y; 
    m.target.z = target.z;
    
    m.bViewParametersChanged = true;
}


INLINE
Vec3 ulOrientation::GetTarget()
{
    //RefreshOrientation();
    
    GetPosition();
    
    Vec3 target = m.pos + GetViewNormal() * m.dist;
    return target;
}


INLINE
void ulOrientation::GetTarget(Float64 &x, Float64 &y, Float64 &z)
{
    //RefreshOrientation();
    
    GetPosition();
    
    Vec3 target = m.pos + GetViewNormal() * m.dist;
    x  = target.x;
    y  = target.y;
    z  = target.z;
}



INLINE
void ulOrientation::GetTarget(Float32 &x, Float32 &y, Float32 &z)
{
    //RefreshOrientation();
    
    GetPosition();
    
    Vec3 target = m.pos + GetViewNormal() * m.dist;
    
    x  = target.x;
    y  = target.y;
    z  = target.z;
}

INLINE
void ulOrientation::RotateXDeg( real degrees_to_rotate )
{
    Vec3 targ_to_pos;
    Mat4 mat;
    
    targ_to_pos = m.pos - m.target;
    mat.SetRotateXDeg( degrees_to_rotate );
    targ_to_pos = targ_to_pos * mat;
    SetPosition( m.target + targ_to_pos );
}

INLINE
void ulOrientation::RotateYDeg( real degrees_to_rotate )
{
    Vec3 targ_to_pos;
    Mat4 mat;
    
    targ_to_pos = m.pos - m.target;
    mat.SetRotateYDeg( degrees_to_rotate );
    targ_to_pos = targ_to_pos * mat;
    SetPosition( m.target + targ_to_pos );
}

INLINE
void ulOrientation::RotateZDeg( real degrees_to_rotate )
{
    Vec3 targ_to_pos;
    Mat4 mat;
    
    targ_to_pos = m.pos - m.target;
    mat.SetRotateZDeg( degrees_to_rotate );
    targ_to_pos = targ_to_pos * mat;
    SetPosition( m.target + targ_to_pos );
}



/*

  Direction
*/


INLINE
ulOrientation::ulOrientation()
{
    memset(&m, 0, sizeof(m));
    
    m.pos.set(0,0,0);
    m.target.set(0,0,10);
    m.Up.set(0, 1, 0);
    SetIdentity();
    m.bViewParametersChanged = false;
    
}

INLINE
void ulOrientation::SetUp(const real &x, const real &y, const real &z)
{
    m.Up.set(x, y, z);
    m.bViewParametersChanged = true;
}

INLINE
void ulOrientation::SetUp(const Vec3 & vec)
{
    m.Up.set(vec.x, vec.y, vec.z);
    m.bViewParametersChanged = true;
}



INLINE
ulOrientation::~ulOrientation()
{
}
INLINE
void ulOrientation::SetDirection(const real &x, const real &y, const real &z)
{
    GetPosition();
    
    m.target.x = m.pos.x + x;
    m.target.y = m.pos.y + y;
    m.target.z = m.pos.z + z;
    
    m.bViewParametersChanged = true;
}
INLINE
void ulOrientation::SetDirection(const Vec3 & dir)
{
    GetPosition();
    m.target = m.pos + dir;
    
    m.bViewParametersChanged = true;
}


INLINE
void ulOrientation::GetDirection(Float64 &x, Float64 &y, Float64 &z)
{
    RefreshOrientation();
    
    Vec3 vpn = GetViewNormal();
    x = vpn.x;
    y = vpn.y;
    z = vpn.z;
}


INLINE
void ulOrientation::GetDirection(Float32 &x, Float32 &y, Float32 &z)
{
    RefreshOrientation();
    
    Vec3 vpn = GetViewNormal();
    x = vpn.x;
    y = vpn.y;
    z = vpn.z;
}


INLINE
Vec3 ulOrientation::GetDirection()
{
    RefreshOrientation();
    
    return GetViewNormal();
    
}
INLINE
void ulOrientation::GetDirection(Vec3 & dir)
{
    RefreshOrientation();
    
    dir = GetViewNormal();
}




INLINE
void ulOrientation::SetRoll(const real &roll ) 
{
    m.roll = roll;
    m.bViewParametersChanged = true;
}




INLINE
void ulOrientation::CreateMatrixFromViewParameters()
{
    Vec3 vpn, vup, vr;
    
    
    vpn = m.target - m.pos;
    m.dist = vpn.GetMagnitude();
    
    vpn.normalize();
    
    vr = m.Up CP vpn;
    
    if (vr.GetMagnitude() < UL_EPS)
        vr = Vec3(0,0,1) CP vpn;
    
    vr.normalize();
    
    vup = vpn CP vr;
    vup.normalize();
    
    CalcRotationMatrixFromAllVectors(vpn, vup, vr, m.pos);
    
    if (m.roll != 0.0f)       
        RotateZDeg(-m.roll);
    
    m.bViewParametersChanged = false;
}



INLINE
bool ulOrientation::RefreshOrientation()
{
    if (m.bViewParametersChanged)
    {
        CreateMatrixFromViewParameters();
        
        return true;
    }
    
    return false;
}

INLINE
void ulOrientation::MoveAlong(const Vec3 & n, const real &x, const real &y, const real &z)
{ 
    Vec3 p;
    p.x = n.x * x;
    p.y = n.y * y;
    p.z = n.z * z;
    
    Strafe(p.x, p.y, p.z);
    
}




INLINE
Mat4 ulOrientation::GetOrientationMatrix()
{
    RefreshOrientation();
    return GetMat4();
}



INLINE
void ulOrientation::SetOrientationMatrix(Mat4 & mat)
{
    
    GetMat4().set(mat);
    m.bViewParametersChanged = false;
}




INLINE
void ulOrientation::Strafe(const real &tx, const real &ty, const real &tz)
{
    
    GetPosition();
    
    m.pos.x += tx;
    m.pos.y += ty;
    m.pos.z += tz;
    
    
    SetTranslationPoint(m.pos);
}



INLINE
Vec3 ulOrientation::GetPosition()
{
    
    RefreshOrientation();
    
    Mat4 mat;
    Vec3 v(0,0,0);
    Vec3 p;
    mat = GetMat4();
    mat.Invert();
    
    m.pos = mat * v;
    
    return m.pos;
}



//////// pivot

INLINE
Vec3 ulOrientation::GetPivotPoint()
{
    return m.pivot;
}



INLINE
void ulOrientation::SetPivotPoint(const Vec3 & pivot)
{
    m.pivot = pivot;
}





INLINE
void ulOrientation::SetPivotPoint(const real &x, const real &y, const real &z)
{
    m.pivot.x = x;
    m.pivot.y = y;
    m.pivot.z = z;
    
}




INLINE
void ulOrientation::GetPivotPoint(Float64 &x, Float64 &y, Float64 &z)
{
    GetPivotPoint();
    x  = m.pivot.x;
    y  = m.pivot.y;
    z  = m.pivot.z;
}

INLINE
void ulOrientation::GetPivotPoint(Float32 &x, Float32 &y, Float32 &z)
{
    GetPivotPoint();
    
    x  = m.pivot.x;
    y  = m.pivot.y;
    z  = m.pivot.z;
    
}

INLINE
void ulOrientation::GetPivotPoint(Vec3 & pivot)
{
    GetPivotPoint();
    pivot = m.pivot;
}



INLINE
void ulOrientation::RotateXDegLocal( real degrees_to_rotate, bool post )
{
    Mat4::RotateXDeg( degrees_to_rotate, post );
}

INLINE
void ulOrientation::RotateYDegLocal( real degrees_to_rotate, bool post )
{
    Mat4::RotateYDeg( degrees_to_rotate, post  );
}

INLINE
void ulOrientation::RotateZDegLocal( real degrees_to_rotate, bool post )
{
    Mat4::RotateZDeg( degrees_to_rotate, post  );
}







#endif   // #ifndef __NVIDIA_ULORIENTATION_H__
