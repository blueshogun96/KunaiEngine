/*********************************************************************NVMH2****
Path:  c:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulGeom.cpp

  Copyright (C) 1999, 2000 NVIDIA Corporation
  This file is provided without support, instruction, or implied warranty of any
  kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
  not liable under any circumstances for any damages or loss whatsoever arising
  from the use or inability to use this file or items derived from it.
  
    Comments:
    
      
******************************************************************************/

#include <ul.h>
#include "limits.h"
#include "ulCommon.h"
#include "ulVec3.h"
#include "ulVec4.h"
#include "ulMat4.h"
#include "ulMat3.h"
#include "ulVec2.h"
#include "ulMatrixVec.h"


real ulDistance(real x1, real y1, real x2, real y2)
{
    real dx, dy;
    dx = x1 - x2;
    dy = y1 - y2;
    
    return ulsqrt(dx * dx + dy * dy);
}


real ulDistance(real x1, real y1, real z1,
                 real x2, real y2, real z2)
{
    real dx, dy, dz;
    dx = x1 - x2;
    dy = y1 - y2;
    dz = z1 - z2;
    
    return ulsqrt(dx * dx + dy * dy + dz * dz);
}

real ulDistance(const Vec3 & v1, const Vec3 & v2)
{
    Vec3 t = v1 - v2;
    return t.GetMagnitude();
    
}

real ulDistanceSquared(const Vec3 & v1, const Vec3 & v2)
{
    Vec3 t = v1 - v2;
    return t.GetMagnitudeSquared();
    
}


real ulDistance(real x, real y)
{
    return ulsqrt(x * x + y * y);
}


int ulPointInPolygon2D(int n, Vec2 & point, real poly[][2])
{
    int i,j, c = 0;
    for(i=0, j = n-1; i<n; j = i++)
    {
        if ((((poly[i][Y] <= point[Y]) && (point[Y] < poly[j][Y])) ||
            ((poly[j][Y] <= point[Y]) && (point[Y] < poly[i][Y]))) &&
            (point[X] < (poly[j][X] - poly[i][RX]) * (point[Y] - poly[i][Y]) / 
            (poly[j][Y] - poly[i][Y]) + poly[i][X]))
            c ^= 1;
    }
    return c;
}



real ulSegmentLength( const Vec3 & a,  const Vec3 & b)
{
    
    Vec3 d = a - b;
    return d.GetMagnitude();
}


// squared
real ulSegmentLengthSquared(Vec3 & a, Vec3 & b)
{
    Vec3 d = a - b;
    return d.GetMagnitudeSquared();
    
}







real ulFindCircle(Vec3 & v1, Vec3 & v2, Vec3 &v3, Vec3 & circle)
{
    real A, B, C, D, E, F, G, dx, dy;
    
    A = v2[RX] - v1[RX];
    B = v2[RY] - v1[RY];
    
    C = v3[RX] - v1[RX];
    D = v3[RY] - v1[RY];
    
    E = A * (v1[RX] + v2[RX]) + B * (v1[RY] + v2[RY]);
    F = C * (v1[RX] + v3[RX]) + D * (v1[RY] + v3[RY]);
    
    G = 2.0 * (A * (v3[RY] - v2[RY]) - B * (v3[RX] - v2[RX]));
    
    if (ulAbs(G) < UL_EPS) 
        return 0.0; // points are collinear, can't make a circle
    
    circle[RX] = (D * E - B * F) / G;
    circle[RY] = (A * F - C * E) / G;
    circle[RZ] = 0.0;
    
    dx = circle[RX] - v1[RX];
    dy = circle[RY] - v1[RY];
    return ulsqrt(dx * dx + dy * dy);
}


//  check coordiantes

real
ulFindSphere(Vec3 & p1, Vec3 & p2, Vec3 & p3, Vec3 & sphere)
{
    
    Vec3 v1, v2, v3, circle;
    Mat3 mat, matinv;
    real dx, dy, r;
    
    
    
    mat.CalcRotationMatrixFromPoints(p1, p2, p3);
    
    
    v1 = mat * (p1 - p1);
    v2 = mat * (p2 - p1);
    v3 = mat * (p3 - p1);
    
    ulFindCircle(v1, v2, v3, circle);
    
    if (ulAbs(circle[RX]) < UL_EPS)
        return 0.0;
    
    
    dx = v1[RX] - circle[RX];
    dy = v1[RY] - circle[RY];
    r = ulsqrt(dx * dx + dy * dy);
    
    
    mat.GetInverse(matinv);
    
    sphere = (mat * circle) + p1;
    
    return ulSegmentLength(sphere, p1);
    
}




/*
parallel flags limits distance to 

  ------------
       |
       |    this region
       |
       |
  ------------
  
    
      //  check coordiantes
*/
real
ulDistancePointToSegment( const Vec3 &p,  const Vec3 & p1,  const Vec3 & p2, int parallel)
{
    Vec3 v2, v;
    real t;
    int hr;
    Mat3 mat;
    
    
    hr = mat.CalcRotationMatrixFromPoints(p1, p2, p);
    
    if (hr < 0)
    {
        // probably colinear 
        real dx,dy;
        v = p2 - p1;
        
        if (ulAbs(v[RX]) > UL_EPS)
            t = (p[RX] - p1[RX]) / v[RX];
        else if (ulAbs(v[RY]) > UL_EPS)
            t = (p[RY] - p1[RY]) / v[RY];
        else if (ulAbs(v[RZ]) > UL_EPS)
            t = (p[RZ] - p1[RZ]) / v[RZ];
        else
            return ulSegmentLength(p, p1); 
        
        if (t >= 0 && t <= 1)
            return 0.0;
        else if (parallel)
            return UL_MAX_FLOAT;
        else if (t > 1)
        {
            dx = p2[RX] - p[RX];
            dy = p2[RY] - p[RY];        
        }
        else
        {
            dx = p1[RX] - p[RX];
            dy = p1[RY] - p[RY];
        }
        return ulsqrt(dx * dx + dy * dy);
    }
    
    
    v2 = mat * (p2- p1);
    v  = mat * (p - p1);
    
    if (parallel)
    {
        // if we extend past the segment
        if (v[RY] < 0 || v[RY] > v2[RY])
            return UL_MAX_FLOAT;
        else 
            // just return the x distance
            return v[RX];
    }
    else
    {
        // get distance to the endpoints
        if (v[RY] < 0)
            return ulsqrt(v[RX] * v[RX] + v[RY] * v[RY]);
        else if (v[RY] > v2[RY])
        {
            v[RY] -= v2[RY];
            return ulsqrt(v[RX] * v[RX] + v[RY] * v[RY]);
        }
        else 
            return v[RX];
    }
}


real ulDistancePointToLine(Vec3 &p, Vec3 & p1, Vec3 & p2)
{
    Vec3 v2, v;
    
    int hr;
    Mat3 mat;
    
    hr = mat.CalcRotationMatrixFromPoints(p1, p2, p);
    
    // colinear
    if (hr < 0)
        return 0;
    
    
    v2 = mat * (p2 - p1);
    v = mat * (p  - p1);
    
    return v[RX];
    
}




real ulDot2Deg(real dot)
{
    return acos(dot) * UL_RAD2DEG;
}



real ulAngleBetweenVectorsDeg(Vec3 &v1, Vec3 & v2)
{
    return ulDot2Deg(v1 & v2);
}




void ulReflect(Vec3 &L, Vec3 &n, Vec3 &r)
{
    real dot;
    dot = 2.0 * (n DOT L);
    
    r[X] = L[X] - n[X] * dot;
    r[Y] = L[Y] - n[Y] * dot;
    r[Z] = L[Z] - n[Z] * dot;
}


void ulReflect2(Vec3 &L, Vec3 &n, Vec3 &r)
{
    real s;
    
    s = L.GetMagnitudeSquared();
    r = L + n;
    
    r.normalize();
    
    r *= s;
    
}


//  check coordiantes

real ulFlatness(Vec3 &a, Vec3 & b, Vec3 & c, Vec3 & d, int absolute)
{
    
    HRESULT hr;
    Vec3 v;
    real theta;
    Mat3 mat;
    
    hr = mat.CalcRotationMatrixFromPoints(a, b, c);
    
    if (hr)
        return 0;
    
    v = mat * (d - a);
    
    theta = atan2(v[RY], v[RX]) * UL_RAD2DEG;
    
    if (absolute)
        return ulAbs(theta);
    else
        return theta;
}





int ulBoxRegion(real x, real y, 
                real x1, real y1, real x2, real y2)
{
    int region = 0;
    if (x < x1)
        region |= 1;
    if (x > x2)
        region |= 2;
    
    if (y < y1)
        region |= 4;
    if (y > y2)
        region |= 8;
    
    return region;
    
}

int ulClosestToBox(real x, real y, real &rx, real &ry,
                   real x1, real y1, real x2, real y2)
{
    int region = ulBoxRegion(x, y, x1, y1, x2, y2);
    
    if (region == 0)
    {
        rx = x;
        ry = y;
        return 0;
    }
    
    switch(region)
    { 
    case 1:
        rx = x1;
        ry = y;
        break;
        
    case 2:
        rx = x2;
        ry = y;
        break;
        
    case 8:
        rx = x;
        ry = y2;
        break;
        
    case 4:
        rx = x;
        ry = y1;
        break;
        
    case 5:
        rx = x1;
        ry = y1;
        break;
        
    case 6:
        rx = x2;
        ry = y1;
        break;    
        
    case 9:
        rx = x1;
        ry = y2;
        break;
        
    case 10:
        rx = x2;
        ry = y2;
        break;
        
    default:
        //printf("Illegal box\n");
        break;
    }
    return region;  
}





/*****************************************************************************

*****************************************************************************/

real ulDistancePointToSegment2(real x, real y, real x1, real y1, real x2, real y2, real linear_distance)
{
    real t, d, a2;
    
    a2 = (y - y1) * (x2 - x1) - (x - x1) * (y2 - y1);
    
    t = (x - x1) * (x2 - x1) + (y - y1) * (y2 - y1);
    
    if (t < 0)
        d = ulDistance(x, y, x1, y1);
    else
    {
        t = (x2 - x) * (x2 - x1) + (y2 - y) * (y2 - y1);
        if (t < 0)
            d = ulDistance(x, y, x2, y2);
        else
            d = linear_distance;
    }
    return d;
}
/*****************************************************************************

*****************************************************************************/

real ulDistancePointToLine(real x, real y, real x1, real y1, real x2, real y2)
{
    real dx, dy;
    real A, B, C, D;
    dx = x2 - x1;
    dy = y2 - y1;
    
    A = dy;
    B = -dx;
    C = -(y1 * B + x1 * A);
    
    D = ulrsqrt(A * A + B * B);
    
    A *= D;
    B *= D;
    C *= D;
    
    return A * (real) x + B * (real) y + C;
    
}
/*****************************************************************************

*****************************************************************************/
real ulDistancePointToSegment(real x, real y, real x1, real y1, real x2, real y2)
{
    real t;
    //real dx, dy;
    
    //dx = x2 - x1;
    //dy = y2 - y1;
    
    t = (x - x1) * (x2 - x1) + (y - y1) * (y2 - y1);
    
    if (t < 0)
        return ulDistance(x, y, x1, y1);
    else
    {
        t = (x2 - x) * (x2 - x1) + (y2 - y) * (y2 - y1);
        if (t < 0)
            return ulDistance(x, y, x2, y2);
        else
        {
        /*a2 = (y - y1) * (x2 - x1) - (x - x1) * (y2 - y1);
        d1 = ulsqrt((a2 * a2) / (dx * dx + dy * dy));
        d2 = d1;
            */
            return ulDistancePointToLine(x, y, x1, y1, x2, y2);
        }
    }
}



/*****************************************************************************

  Interpolate tensor
  
    z4    z3
    
      z1    z2
      
        
          x , y are [0, 1]
          
*****************************************************************************/

real ulTensor(real z1, real z2, real z3, real z4, real y, real x)
{
    
    if (y == 0.0 && x == 0.0)
        return z1;
    else if (x + y < 1.0)
        return z1 + y * (z4 - z1) + x * (z2 - z1);
    else
        return z3 + (1.0 - y) * (z2 - z3) + (1.0 - x) * (z4 - z3);
}





Int ulSamePoint(Vec3 & p1, Vec3& p2)
{
    if (p1[0] == p2[0] && p1[1] == p2[1] && p1[2] == p2[2])
        return 1;
    return 0;
}



void ulDmsToDec(real deg, real min, real sec, char hemisphere, real * decimal_number)
{
    *decimal_number = ulSgn(deg) * (ulAbs(deg) + (min + sec / 60.0f) / 60.0f);
    
    if (hemisphere == 'S' || hemisphere == 's' || hemisphere == 'W' || hemisphere == 'w')
        *decimal_number = -*decimal_number;
}

void ulDecToDms(real decimal_number, real * deg, real * min, real * sec)
{
    real decf, degt, minf, mint, secf, sign;
    
    if (decimal_number < 0.0)
    {
        decf = -decimal_number;
        sign = -1.0;
    }
    else
    {
        decf = decimal_number;
        sign = 1.0;
    }
    degt = (real) ((int) (decf));
    minf = (decf - degt) * 60.0f;
    mint = (real) ((int) (minf));
    secf = (minf - mint) * 60.0f;
    
    *deg = degt * sign;
    *min = mint;
    *sec = secf;
    
}


void ulXform3D(real h, real p, real r, real x0, real y0, real z0, real * x, real * y, real * z)
/* Angles in degrees */
{
    
    real x1, z1, y2;
    
    if (ulAbs(r) < UL_EPS)
    {
        x1 = x0;
        z1 = z0;
    }
    else
    {
        x1 = x0 * ulcos(r) + z0 * ulsin(r);
        z1 = z0 * ulcos(r) - x0 * ulsin(r);
    }
    
    y2 = y0 * ulcos(p) - z1 * ulsin(p);
    *z = z1 * ulcos(p) + y0 * ulsin(p);
    
    *x = x1 * ulcos(h) + y2 * ulsin(h);
    *y = y2 * ulcos(h) - x1 * ulsin(h);
    
}

/*****************************************************************************;
rotate2d - rotate Floating point x,y coordinates clockwise

*****************************************************************************/
void ulRotate2d(real angle, real x, real y, real * xr, real * yr)
{
    real s, c;
    
    if (ulAbs(angle) < UL_EPS)
    {
        *xr = x;
        *yr = y;
    }
    else
    {
        s = ulsin(angle);
        c = ulcos(angle);
        
        *xr = x * c - y * s;
        *yr = x * s + y * c;
    }
}






void ulRectToPolar(real x, real y, real * r, real * angle)
{
    *r = ulsqrt(x * x + y * y);
    *angle = ulatan2(y, x);
}

void ulPolarToRect(real r, real angle, real * x, real * y)
{
    if (ulAbs(r) < UL_EPS)
    {
        *x = 0.0;
        *y = 0.0;
    }
    else
    {
        *x = r * ulcos(angle);
        *y = r * ulsin(angle);
    }
    
}

void ulPolarToRect2(real rx, real ry, real angle, real * x, real * y)
{
    if (ulAbs(rx) < UL_EPS)
        *x = 0.0;
    else
        *x = rx * ulcos(angle);
    
    if (ulAbs(ry) < UL_EPS)
        *y = 0.0;
    else
        *y = ry * ulsin(angle);
}



/*
return fractional part
*/
real ulFrac(real a)
{
    Int b;
    
    b = (Int) a;
    
    a = a - (real) b;
    return a;
}





int ulWhichSegment(real x, real y, real z,
                   real x1, real y1, real z1, 
                   real x2, real y2, real z2)
{
    real t;
    
    t = (x - x1) * (x2 - x1) + (y - y1) * (y2 - y1) + (z - z1) * (z2 - z1);
    
    if (t <= 0)
        //return distance(x, y, x1, y1);
        return 0;
    else
    {
        t = (x2 - x) * (x2 - x1) + (y2 - y) * (y2 - y1) + (z2 - z) * (z2 - z1);
        if (t <= 0)
            //return distance(x, y, x2, y2);
            return 1;
        else
        {
            return 2;
            //return distancePointToLine(x, y, x1, y1, x2, y2);
        }
    }
}


int ulWhichSegment(real x, real y,
                   real x1, real y1, 
                   real x2, real y2)
{
    real t;
    
    t = (x - x1) * (x2 - x1) + (y - y1) * (y2 - y1);
    
    if (t <= 0)
        //return distance(x, y, x1, y1);
        return 0;
    else
    {
        t = (x2 - x) * (x2 - x1) + (y2 - y) * (y2 - y1);
        if (t <= 0)
            //return distance(x, y, x2, y2);
            return 1;
        else
        {
            return 2;
            //return distancePointToLine(x, y, x1, y1, x2, y2);
        }
    }
}

     
void ulClosestPoint2D(real x, real y, real x1, real y1, real x2, real y2, 
                      real &x3, real &y3, real &t)
{
    
    real dx, dy, invlen;
    
    dx = x2 - x1;
    dy = y2 - y1;
    
    //len = ulsqrt(dx * dx + dy * dy);
    //t = ((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) / (len * len);
    invlen = ulrsqrt(dx * dx + dy * dy);
    t = ((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) * (invlen * invlen);
    
    if (t <= 0)
    {
        x3 = x1;
        y3 = y1;
        
        //return distance(x, y, x1, y1);
        //return 0;
        
    }
    else if (t >= 1)
    {
        x3 = x2;
        y3 = y2;
        //return distance(x, y, x2, y2);
        //return 1;
        
        
    }
    else 
    {
        x3 = x1 + t * (x2 - x1);
        y3 = y1 + t * (y2 - y1);
        
        //return 2;
        //return distancePointToLine(x, y, x1, y1, x2, y2);
    }
}

/*
do these 2D segments intersect each other
*/


int ulSegmentIntersection(real x0, real y0, real x1, real y1,
                          real x2, real y2, real x3, real y3,
                          real &xa, real &ya, real &xb, real &yb,
                          real &r, real &s, int clamp)
{
    real a,b,c;
    int e = 1;
    
    c = ((x1-x0)*(y3-y2)-(y1-y0)*(x3-x2));
    if (ulAbs(c) < UL_EPS)
        return -1;
    
    a = ((y0-y2)*(x3-x2)-(x0-x2)*(y3-y2));
    b = ((y0-y2)*(x1-x0)-(x0-x2)*(y1-y0));
    
    r = a / c;
    s = b / c;
    
    
    if (r > 1 || r < 0 || s > 1 || s < 0)
        e = 0;  
    
    if (clamp && e == 0)
    {
        if (r > 1)
            r = 1;
        
        if (r < 0)
            r = 0;
        
        if (s > 1)
            s = 1;
        
        if (s < 0)
            s = 0;
    }
    
    
    xa = x0 + r * (x1-x0);
    ya = y0 + r * (y1-y0);
    
    xb = x2 + s * (x3-x2);
    yb = y2 + s * (y3-y2);
    
    return e;
    
    
    /*
    If 0<=r<=1 & 0<=s<=1, intersection exists
    r<0 or r>1 or s<0 or s>1 line segments do not intersect
    
      If the denominator in eqn 1 is zero, AB & CD are parallel
      If the numerator in eqn 1 is also zero, AB & CD are coincident
      
        If the intersection point of the 2 lines are needed (lines in this
        context mean infinite lines) regardless whether the two line
        segments intersect, then
        
          If r>1, I is located on extension of AB
          If r<0, I is located on extension of BA
          If s>1, I is located on extension of CD
          If s<0, I is located on extension of DC
    */
    
}




/*
    edge p1-p2 , v
*/


void ulClosestPointToSegment(Vec3 &p1, Vec3 &p2, Vec3 & v, Vec3 &p)
{
    Mat4 mat;
    Vec3 j2,j3;
    
    if (p1 == v || p2 == v)
    {
        p = v;
        return;
    }
    if (p1 == p2)
    {
        p = p2;
        return;
    }
    
    mat.CalcRotationMatrixFromPoints(p1, p2, v);
    
    //W2VM(p1, j1, p1, mat);
    j2 = mat * p2;
    j3 = mat * v;
    
    j3[RX] = 0;
    
    if (j3[RZ] > j2[RZ])
        j3[RZ] = j2[RZ];
    
    if (j3[RZ] < 0)
        j3[RZ] = 0;
    
    
    Mat4 matinv;
    mat.GetInverse(matinv);
    p = matinv * j3;
    
}






int ulAlmostEqual(Vec3 &a, Vec3 &b, real eps)
{
    Vec3 t;
    real l;
    t = a - b;
    l = t.GetMagnitude();
    if (l <= eps) // 
        return 1;
    else
        return 0;
    
}

// ordinary differential equations

void ulODE(void * data, real y[], real yout[], int n, real t0, real h, 
           dydt_func dydt, real dydx[], real *dym, real *dyt, real*yt)
{
    int i;
    real xh, h2, h6;
    
    h2 = h * 0.5f;
    h6 = h / 6.0f;
    
    xh = t0 + h2;
    for (i=0; i<n; i++) 
        yt[i] = y[i] + h2 * dydx[i];
    
    dydt(data, xh, yt, dyt);
    
    for (i=0; i<n; i++) 
        yt[i] = y[i] + h2 * dyt[i];
    
    dydt(data, xh, yt, dym);
    
    for (i=0; i<n; i++) 
    {
        yt[i] = y[i] + h * dym[i];
        dym[i] += dyt[i];
    }
    
    dydt(data, t0 + h, yt, dyt);
    
    for (i=0; i<n; i++)
        yout[i] = y[i] + h6 * (dydx[i] + dyt[i] + 2.0f * dym[i]);
}







/*
closest points on two edges

  p1-p2  p3-p4
*/


int ulClosestPoints(Vec3 &p1, Vec3 &p2, Vec3 &p3, Vec3 &p4,
                    Vec3 &o1, Vec3 &o2)
{
    Mat4 mat, matinv;
    Vec3 j1, j2, j3, j4;
    Vec3 t1, t2, t3, t4;
    Vec3 v1, v2; 
    int p;
    
    
    if (p1 == p3 || p2 == p3)
    {
        o1 = p1;
        o2 = p1;
        return 0;
    }
    if (p1 == p4 || p2 == p4)
    {
        o1 = p4;
        o2 = p4;
        return 0;
    }
    
    
    mat.CalcRotationMatrixFromPoints(p1, p2, p3);
    
    j1 = mat * p1;
    j2 = mat * p2;
    j3 = mat * p3;
    j4 = mat * p4;
    
    
    real theta;
    real y = j4[RZ] - j3[RZ];
    real x = j4[RX] - j3[RX];
    
    theta = (real) ulRad2Deg(atan2(y, x));
    mat.RotateZDeg(-theta); // DHR correct?
    
    mat.GetInverse(matinv);
    
    
    t3 = mat * p3;
    t4 = mat * p4;
    
    if (t3[RX] > t4[RX])
        ulSwap(t3,t4);
    
    real r, s;
    real xa, ya, xb, yb;
    
    p = ulSegmentIntersection(j1[RX], j1[RY], j2[RX], j2[RY],
        t3[RX], t3[RY], t4[RX], t4[RY],
        xa, ya, xb, yb, r, s, 1);
    
    
    if (r == -1)
    {
        if (t3[RY] > t4[RY])
            ulSwap(t3,t4);
        if (t3[RY] > j2[RY])
        {
            v1 = j2;
            v2 = t3;
        }
        else if (t4[RY] < j1[RY])
        {
            v1 = j1;
            v2 = t4;
        }
        else
        {
            if (j2[RY] < t4[RY])
                v1 = j2;
            else
                v1 = t4;
            
            if (j1[RY] > t3[RY])
                v2 = j1;
            else
                v2 = t4;
        }
        
        
    }
    else
    {
        v1[RX] = xa;
        v1[RY] = ya;
        v1[RZ] = 0;
        
        v2[RX] = xb;
        v2[RY] = yb;
        v2[RZ] = t3[RZ];
        
        
        o1 = matinv*v1;
        o2 = matinv*v2;
    }
    return 0;
    
    
}





void ulDisplacePoint(Vec3 &point, Vec3 &vect, real lambda, Vec3 &result)
{
    result[0] = point[0] + lambda * vect[0];
    result[1] = point[1] + lambda * vect[1];
    result[2] = point[2] + lambda * vect[2];
} 




real ulDotProduct (const Vec3& a, const Vec3& b)
{
    return (a.x*b.x + a.y*b.y + a.z*b.z); 
}


