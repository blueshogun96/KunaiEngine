/*********************************************************************NVMH2****
Path:  D:\Testbuild\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulVector1.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/



#ifndef ULVECTOR1_H
#define ULVECTOR1_H



class ulVector1
{
public:
    int size() { return n; }

    int n;
    int alloc_n;
    
    real * v;
    void Allocate(const int &n);
    void Release();
    void clear();
    
    ulVector1(const int &n);
    ulVector1();
    
    // access with [] or ()
    real& operator[](int i) const;
    real & operator () (const int &i) const;
    
    void resize(const int &r);
    void resize(ulVector1 & v);
    void InitVector1();
    void seti( int n, ...);
    void print(char * title = 0) ;
    
    
    ulVector1 & operator = (const ulVector1 & v);
    
    
    real sum();
    
    // Assignment operators
    
    
    ulVector1 & operator += ( const ulVector1& v );     // incrementation by a ulVector1
    ulVector1 & operator -= ( const ulVector1& v );     // decrementation by a ulVector1
    ulVector1 & operator *= ( const real d );     // multiplication by a constant
    ulVector1 & operator += ( const real d );     // add constant
    ulVector1 & operator /= ( const real d );     // division by a constant
    
    // special functions
    
    real GetMagnitude();         // length of a ulVector1
    real GetMagnitudeSquared();  // squared length of a ulVector1    
    
    
    
    // //friends
    
    friend ulVector1 operator - (const ulVector1& v);         // -v1
    friend ulVector1 operator + (const ulVector1& v1, const ulVector1& v2);      // v1 + v2
    friend ulVector1 operator - (const ulVector1& v, const ulVector1& v2);      // v1 - v2
    friend ulVector1 operator * (const ulVector1& v, const real d);      // v1 * d
    friend ulVector1 operator * (const real d, const ulVector1& a);      // d * v1
    
    friend ulVector1 operator * (const ulMatrix1& m, const ulVector1& v);      // M * v
    friend ulVector1 operator * (const ulVector1& v, const ulMatrix1& m);      // M(transpose) * v
    
    
    
    
    friend real operator & (const ulVector1& v1, const ulVector1& v2);    // dot product
    
    friend ulVector1 operator * (const ulVector1& v1, const ulVector1& b);    // V1 * V2
    friend ulVector1 operator / (const ulVector1& v, const real d);      // v1 / 3.0
    friend ulVector1 operator CP (const ulVector1& v1, const ulVector1& v2);      // cross product
    
    friend int operator == (const ulVector1& v1, const ulVector1& v2);      // v1 == v2 ?
    friend int operator != (const ulVector1& v1, const ulVector1& v2);      // v1 != v2 ?
    friend ostream& operator << (ostream& s, ulVector1& v);      // output to stream
    friend istream& operator >> (istream& s, ulVector1& v);      // input from strm.
    
    ulVector1& operator *= (const ulVector1& v);
    ulVector1& operator /= (const ulVector1& v);
    
    // =====================================
    // Unary operators
    // =====================================
    
    friend ulVector1 operator + (const ulVector1& v);
    friend ulVector1 operator - (const ulVector1& v);
    
    
    // =====================================
    // Binary operators
    // =====================================
    
    // Addition and subtraction
    friend ulVector1 operator + (const ulVector1& v1, const ulVector1& v2);
    friend ulVector1 operator - (const ulVector1& v1, const ulVector1& v2);
    // Scalar multiplication and division
    friend ulVector1 operator * (const ulVector1& v, real s);
    friend ulVector1 operator * (real s, const ulVector1& v);
    friend ulVector1 operator / (const ulVector1& v, real s);
    // Memberwise multiplication and division
    friend ulVector1 operator * (const ulVector1& v1, const ulVector1& v2);
    friend ulVector1 operator / (const ulVector1& v1, const ulVector1& v2);
    
    // Vector dominance
    friend int operator < (const ulVector1& v1, const ulVector1& v2);
    friend int operator <= (const ulVector1& v1, const ulVector1& v2);
    
    // Bitwise equality
    friend int operator == (const ulVector1& v1, const ulVector1& v2);
    
    
    
};

    
INLINE void ulVector1::resize(ulVector1 & v)
{
    resize(v.n);
}
INLINE void ulVector1::resize(const int & elements)
{
    if (elements > alloc_n)
    {
        Release();
        Allocate(elements);
    }
    else
        n = elements;
    
}




INLINE void ulVector1::InitVector1()
{
    n = 0;
    alloc_n = 0;
    v = 0;

}


INLINE void ulVector1::Allocate(const int &elements)
{
    alloc_n = n = elements;
    v = new real[n];
    if (v == 0)
    {
        char buff[1000];
        sprintf(buff, "Out of Memory [%d bytes]! Exitting", n);

        MessageBox(0, buff, "Error", MB_OK);
        exit(1);
    }
        
}



INLINE ulVector1::ulVector1 ()
{
    InitVector1();
} 

INLINE ulVector1::ulVector1 (const int &elements)
{
    InitVector1 ();
    Allocate (elements);
}




INLINE void ulVector1::Release ()
{

    if (n)
    {
        SAFE_ARRAY_DELETE (v);
    }


    InitVector1 ();
}


INLINE void ulVector1::seti (int _size, ...)
{
    int i;
    real t;

    resize (_size);

    va_list list;

    va_start (list, _size);     /* Initialize variable arguments. */

    for (i = 0; i < n; i++)
    {
        v[i] = (real) va_arg (list, int);
    }

    va_end (list);              /* Reset variable arguments.      */
}

INLINE real & ulVector1::operator[](int i) const
{ 
    //assert (i < n);
    if (i >= n || i < 0)
    {
        char buff[1000];
        sprintf(buff, "ulVector1 %d %d",i, n);
        MessageBox(0, buff, 0, MB_OK);
        exit(1);

    }
    return v[i];
}


INLINE real & ulVector1::operator () (const int &i) const
{
    assert (i < n);
    return v[i];
}



INLINE void ulVector1::print (char *title)
{
    int i, j;

    if (title)
        dprintf ("%s\n", title);
    for (i = 0; i < n; i++)
    {
        dprintf ("[%d] ", i);
        dprintf (FLOAT_FORMAT, v[i]);
        dprintf ("\n");
    }
    dprintf ("\n");

    fflush (stdout);

}






INLINE ulVector1 operator *(const ulVector1 & v1, const ulVector1 & v2)
{
    ulVector1 ret(v1.n);
    int i;
    for (i = 0; i < v1.n; i++)
    {
        ret (i) = v1[i] * v2[i];
    }
    return ret;


}

INLINE ulVector1 operator + (const ulVector1 & v1, const ulVector1 & v2)
{
    ulVector1 ret(v1.n);
    int i;
    for (i = 0; i < v1.n; i++)
    {
        ret (i) = v1[i] + v2[i];
    }
    return ret;


}

INLINE real ulVector1::sum()
{
    real ret = 0;
    for (int i = 0; i < n; i++)
    {
        ret+= v[i];
    }

    return ret;

}


INLINE ulVector1 & ulVector1::operator = (const ulVector1 & other)
{
    int i;

    // assignment 
    resize(other.n);
    for (i = 0; i < n; i++)
    {
        v[i] = other[i];
    }
    return *this;
}





INLINE void ulVector1::clear ()
{
    int i;

    for (i = 0; i < size (); i++)
        v[i] = 0;

}



INLINE ulVector1 & ulVector1::operator += (const ulVector1 & other)
{
    int i;

    for (i = 0; i < size (); i++)
        v[i] += other.v[i];

    return *this;
}

INLINE ulVector1 operator *(const ulVector1 & other, real s)
{
    ulVector1 ret (other.n);

    int i;

    for (i = 0; i < other.n; i++)
        ret.v[i] = other.v[i] * s;

    return ret;
}

INLINE ulVector1 operator /(const ulVector1 & other, real s)
{
    ulVector1 ret (other.n);

    int i;

    for (i = 0; i < other.n; i++)
        ret.v[i] = other.v[i] / s;

    return ret;
}


INLINE ulVector1 operator *(real s, const ulVector1 & other)
{
    ulVector1 ret (other.n);

    int i;

    for (i = 0; i < other.n; i++)
        ret.v[i] = other.v[i] * s;

    return ret;
}



INLINE ulVector1 & ulVector1::operator += (const real d)
{
    int i;

    for (i = 0; i < size (); i++)
        v[i] += d;

    return *this;
}

INLINE ulVector1 operator - (const ulVector1& v1, const ulVector1& v2)
{
    assert(v1.n == v2.n);

    ulVector1 ret(v1.n);
    for(int i=0; i<v1.n; i++)
        ret(i) = v1(i) - v2(i);
    
    return ret;
}


INLINE ulVector1 operator / (const ulVector1& v1, const ulVector1& v2)
{
    assert(v1.n == v2.n);

    ulVector1 ret(v1.n);
    for(int i=0; i<v1.n; i++)
        ret(i) = v1(i) / v2(i);
    
    return ret;
}

INLINE real operator DOT (const ulVector1& a, const ulVector1& b)
{
    real dot = 0;

    assert(a.n == b.n);

    for(int i=0; i<a.n; i++)
        dot += a[i] * b[i];
    return dot;
}



#endif
