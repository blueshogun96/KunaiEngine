/******************************************************************************
ulMatQ.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:



******************************************************************************/
#include "ul.h"



void MatQ::GetInverse(MatQ &T) const
{
  T.q.s = -q.s;
  T.q.i =  q.i;
  T.q.j =  q.j;
  T.q.k =  q.k;

  T.q.xform(d, T.d);
  T.d.negate(T.d);
}


void MatQ::Invert()
{
  q.s = -q.s;
  q.xform(d);
  d.negate();
}



void MatQ::mult(const MatQ &T, const MatQ &U)
{
  q.mult(T.q, U.q);
  T.q.xform(U.d, d);
  d.add(d, T.d);
}


void MatQ::mult(const MatQ &T, bool post)
{
  if (post == false)
  {
    q.mult(T.q);
    T.q.xform(d);
    d.add(T.d);
  }
  else
  {
    Vec3 v;
    
    q.xform(T.d, v);
    d.add(v);
    q.mult(T.q, true);
  }
}

void MatQ::SetInverse(const MatQ &T)
{
  q.s = -T.q.s;
  q.i =  T.q.i;
  q.j =  T.q.j;
  q.k =  T.q.k;
  q.xform(T.d, d);
  d.negate(d);
}






void MatQ::xformVect(const Vec3 &v, Vec3 &xv) const
{
  q.xform(v, xv);
}


void MatQ::xformVect(Vec3 &v) const
{
  q.xform(v);
}


void MatQ::xformPoint(const Vec3 &p, Vec3 &xp) const
{
  q.xform(p, xp);
  xp.add(d);
}


void MatQ::xformPoint(Vec3 &p) const
{
  q.xform(p);
  p.add(d);
}


void MatQ::invXformVect(const Vec3 &v, Vec3 &xv) const
{
  q.invXform(v, xv);
}


void MatQ::invXformVect(Vec3 &v) const
{
  q.invXform(v);
}


void MatQ::invXformPoint(const Vec3 &p, Vec3 &xp) const
{
  xp.sub(p, d);
  q.invXform(xp);
}


void MatQ::invXformPoint(Vec3 &p) const
{
  p.sub(d);
  q.invXform(p);
}






ostream& MatQ::print(ostream &os) const
{
  return os << q << d;
}


   
void MatQ::SetIdentity()
{

  q.SetIdentity();
  d.set(0,0,0);
  
}
