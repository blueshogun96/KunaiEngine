/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulPlane.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/

#include <ul.h>


//The Plane class 

Int Plane::FindPlanePos(
                   real x0, real y0, real z0,
                   real x1, real y1, real z1,
                   real x2, real y2, real z2,
                   Int unitize)
{
  //real u;
  Vec3 v0,v1;
  
  v0.x = x0 - x1;
  v0.y = y0 - y1;
  v0.z = z0 - z1;

  v1.x = x0 - x2;
  v1.y = y0 - y2;
  v1.z = z0 - z2;
  
  vec3.cross(v0, v1);

  //a = (v0.y * v1.z - v0.z * v1.y);
  //b = (v0.z * v1.x - v0.x * v1.z);
  //c = (v0.x * v1.y - v0.y * v1.x);
  
  if (unitize)
    vec3.normalize();

  /* make this face up */
  if (c < 0.0)
  {
    a = -a;
    b = -b;
    c = -c;
  }
  
  d = -(a * x1 + b * y1 + c * z1);
  return 0;
}


Int ulFindNormal(const Vec3 & p0, const Vec3 & p1, const Vec3 & p2, Vec3 & n, int unitize)
{
  Vec3 v0,v1;

  v0 = p1 - p0;
  v1 = p1 - p2;

  n.cross(v0, v1);

  //n.x = v0.y * v1.z - v0.z * v1.y;
  //n.y = v0.z * v1.x - v0.x * v1.z;
  //n.z = v0.x * v1.y - v0.y * v1.x;

  if (unitize)
    n.normalize();

  return 0;

}

Int ulFindNormal (real x0, real y0, real z0, 
                  real x1, real y1, real z1, 
                  real x2, real y2, real z2, 
                  Vec3 &n , int unitize )
{
  Vec3 v0,v1;

  v0.x = x1 - x2;
  v0.y = y1 - y2;
  v0.z = z1 - z2;

  v1.x = x1 - x0;
  v1.y = y1 - y0;
  v1.z = z1 - z0;

  n.cross(v0, v1);

  //n.x = v0.y * v1.z - v0.z * v1.y;
  //n.y = v0.z * v1.x - v0.x * v1.z;
  //n.z = v0.x * v1.y - v0.y * v1.x;

  if (unitize)
    n.normalize();


  return 0;


}



Int 
Plane::FindPlane(real x0, real y0, real z0,
            real x1, real y1, real z1,
            real x2, real y2, real z2,
            int unitize)
{
  Vec3 v0,v1;

  v0.x = x1 - x2;
  v0.y = y1 - y2;
  v0.z = z1 - z2;

  v1.x = x1 - x0;
  v1.y = y1 - y0;
  v1.z = z1 - z0;

  vec3.cross(v0, v1);

  if (unitize)
    vec3.normalize();

  d = -(a * x1 + b * y1 + c * z1);

  return 0;

}






Int Plane::FindPlane(const Vec3 & vp0, const Vec3 & vp1, const Vec3 & vp2, int unitize)
{
  Vec3 v0, v1;

  v0 = vp1 - vp0;
  v1 = vp1 - vp2;

  vec3.cross(v0, v1);

  if (unitize)
    vec3.normalize();

  d = -(a * vp0.x + b * vp0.y + c * vp0.z);
  return 0;
}






real Plane::DistanceToPoint(const Vec3 &point) const
{
  return a * point[0] + b * point[1] + c * point[2] + d;
} 



void Plane::set(const Vec3 &vec, const Vec3 &thruPoint)
{
  vec3 = vec; 
  D = -ulDotProduct(thruPoint, vec);
}



// Output a Plane
ostream& Plane::print(ostream &os) const
{
  int oldFlags;

  oldFlags = os.setf(ios::showpos);
  os << a << " a " << b << " b  "
     << c << " c " << d << " >= 0";
  os.flags(oldFlags);
  return os;
}


Plane::Plane(const Vec3 & _vec3, real _d)
{
  a = _vec3.x;
  b = _vec3.y;
  c = _vec3.z;
  d = _d;
}
Plane::Plane(const Vec4 & _vec4)
{
  a = _vec4.x;
  b = _vec4.y;
  c = _vec4.z;
  d = _vec4.w;
}
Plane::Plane()
{
  a = 0;
  b = 0;
  c = 0;
  d = 0;
}


