/*********************************************************************NVMH2****
Path:  D:\Testbuild\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulVector.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/


#ifndef	__ULVECTOR_H__
#define	__ULVECTOR_H__


class ulVector;
class ulVector1;

class ulMatrix;
class ulMatrix1;




class ulVector
{
public:
    real * v; 
    real & entry(const int &i, const int &d) const;
    
    ulVector *saved_vector;
    void Allocate(const int &r, const int &dim);
    void Release();
    
public:
    
    int alloc_n, alloc_dim, alloc_size;
    
    int n, dim, size;
    ulVector(const int &n, const int &dim);
    
    ulVector(const ulVector & v);
    ulVector() { InitVector();}
    
    void InitVector();
    void ReInit();
    void resize(const int &r, const int &dim);
    void resize(ulVector & v);
    
    
    void SaveVector();
    void RestoreVector();
    
    void CopyVector(const ulVector & src);
    
    void seti( int n, int dim, ...);
    void setd( int n, int dim, ...);
    void setf( int n, int dim, ...);
    
    
    ~ulVector();
    void clear ();
    
    real & operator () (const int &i, const int & dim) const;
    //real & operator () (const int &i, const int & dim);
    
    //real & operator () (const int &i, const int & dim) ;
    
    //ulVector  operator * (const real &val);
    //ulVector  operator * (const ulVector & b);
    
    //ulVector  operator / (const real &val);
    //ulVector  operator + (const ulVector & a);
    //ulVector  operator - (const ulVector & a);
    
    ulVector & operator = (const ulVector & v);
    //ulVector & operator * (const ulMatrix & b);        // this * m2
    //ulVector & operator = (const real &val);
    
    //ulVector  operator & (const ulVector  & b);
    //friend ulVector  operator - (const ulVector  & v);			    // -v1
    
    friend ulVector  operator * (const ulMatrix & a, const ulVector & v);
    friend ulVector  operator * (const ulVector & v, const ulMatrix & a);
    
    //friend ulVector  operator * (const ulVector & a, const real d);
    //friend ulVector  operator * (const real &d, const ulVector & a);
    
    //friend real operator + (const ulVector & a, const ulVector & b);
    
    
    // Assignment operators
    
    
    ulVector & operator += ( const ulVector& v );     // incrementation by a ulVector
    ulVector & operator -= ( const ulVector& v );     // decrementation by a ulVector
    ulVector & operator *= ( const real d );     // multiplication by a constant
    ulVector & operator /= ( const real d );     // division by a constant
    
    // special functions
    
    real GetMagnitude();         // length of a ulVector
    real GetMagnitudeSquared();  // squared length of a ulVector    
    
    
    
    // //friends
    
    friend ulVector operator - (const ulVector& v);         // -v1
    friend ulVector operator + (const ulVector& v1, const ulVector& v2);      // v1 + v2
    friend ulVector operator - (const ulVector& v, const ulVector& v2);      // v1 - v2
    friend ulVector operator * (const ulVector& v, const real d);      // v1 * d
    friend ulVector operator * (const real d, const ulVector& a);      // d * v1
    
    friend ulVector operator * (const ulMatrix& m, const ulVector& v);      // M * v
    friend ulVector operator * (const ulVector& v, const ulMatrix& m);      // M(transpose) * v
    
    
    
    
    friend real operator & (const ulVector& v1, const ulVector& v2);    // dot product
    
    friend ulVector operator * (const ulVector& v1, const ulVector& b);    // V1 * V2
    friend ulVector operator / (const ulVector& v, const real d);      // v1 / 3.0
    friend ulVector operator CP (const ulVector& v1, const ulVector& v2);      // cross product
    
    friend int operator == (const ulVector& v1, const ulVector& v2);      // v1 == v2 ?
    friend int operator != (const ulVector& v1, const ulVector& v2);      // v1 != v2 ?
    friend ostream& operator << (ostream& s, ulVector& v);      // output to stream
    friend istream& operator >> (istream& s, ulVector& v);      // input from strm.
    
    ulVector& operator *= (const ulVector& v);
    ulVector& operator /= (const ulVector& v);
    
    // =====================================
    // Unary operators
    // =====================================
    
    friend ulVector operator + (const ulVector& v);
    friend ulVector operator - (const ulVector& v);
    
    
    // =====================================
    // Binary operators
    // =====================================
    
    // Addition and subtraction
    friend ulVector operator + (const ulVector& v1, const ulVector& v2);
    friend ulVector operator - (const ulVector& v1, const ulVector& v2);
    // Scalar multiplication and division
    friend ulVector operator * (const ulVector& v, real s);
    friend ulVector operator * (real s, const ulVector& v);
    friend ulVector operator / (const ulVector& v, real s);
    // Memberwise multiplication and division
    friend ulVector operator * (const ulVector& v1, const ulVector& v2);
    friend ulVector operator / (const ulVector& v1, const ulVector& v2);
    
    // Vector dominance
    friend int operator < (const ulVector& v1, const ulVector& v2);
    friend int operator <= (const ulVector& v1, const ulVector& v2);
    
    // Bitwise equality
    friend int operator == (const ulVector& v1, const ulVector& v2);
    
    
    void Print(char * title = 0) ;
};



INLINE void ulVector::ReInit ()
{
    n = 0;
    alloc_n = 0;

    //saved_vector = 0;
    v = 0;

    dim = 0;
    alloc_dim = 0;

    size = 0;
    alloc_size = 0;

}



INLINE void ulVector::InitVector()
{
    n = 0;
    alloc_n = 0;

    saved_vector = 0;
    v = 0;

    dim = 0;
    alloc_dim = 0;

    size = 0;
    alloc_size = 0;



}



INLINE real & ulVector::entry(const int &i, const int & d) const
{
#if _DEBUG
    assert (i < n);
    assert(d < dim);
    assert(v != 0);
#endif
    
    return v[i * dim + d];
} 


INLINE ulVector::ulVector (const int &elements, const int &d)
{
    InitVector();
    Allocate (elements, d);
}

INLINE ulVector::ulVector (const ulVector & in)
{
    InitVector();
    Allocate (in.n, in.dim);
    for (int i = 0; i < in.size; i++)
        v[i] = in.v[i];
}

INLINE void ulVector::Release ()
{

    if (n)
    {
        SAFE_ARRAY_DELETE (v);
    }


    ReInit ();
}

INLINE ulVector::~ulVector ()
{
    if (saved_vector)
        delete saved_vector;

    Release ();
}


INLINE void ulVector::seti (int size, int _dim, ...)
{
    int i;
    //real t;

    resize (size, _dim);

    va_list list;

    va_start (list, _dim);      /* Initialize variable arguments. */

    for (i = 0; i < n; i++)
    {
        entry (i, 0) = va_arg (list, int);
    }

    va_end (list);              /* Reset variable arguments.      */
}



/*
ulVector ulVector::operator * (const ulMatrix& a) 
{
int d,r, c;
assert(n == a.cols && dim == a.dim);

  ulVector av(n, dim);
  av.Clear();
  
    for(d=0; d<dim; d++)
    {
    for(r=0; r<a.rows; r++)
    {
    av.v[r] = 0;
    for(c=0; c<a.cols; c++)
    {
    av(r, d) += a(r,c, d) * vector(c, d);
    }
    }
    }
    return av;
} */



INLINE void ulCopyVector(ulVector & dst, const ulVector & src)
{
    int i;
    dst.resize(src.n, src.dim);
    
    for(i=0; i<src.size; i++)
        dst.v[i] = src.v[i]; 
} 


INLINE void ulVector::SaveVector ()
{
    if (saved_vector == 0)
    {
        saved_vector = new ulVector (n, dim);
    }
    saved_vector->resize (n, dim);

    //*saved_vector = *this;
    ulCopyVector (*saved_vector, *this);

}

INLINE void ulVector::RestoreVector ()
{
#if _DEBUG
    assert (saved_vector != 0);
#endif

    //*this = *saved_vector;
    CopyVector (*saved_vector);
}


INLINE void ulVector::clear()
{
    int i;
#if _DEBUG
    assert (v != 0);
#endif

    for (i = 0; i < size; i++)
        v[i] = 0;

}




INLINE ulVector & ulVector::operator = (const ulVector & v)
{
    CopyVector (v);
    return *this;
}


INLINE void ulVector::Allocate (const int &elements, const int &d)
{
    alloc_n = n = elements;
    alloc_dim = dim = d;
    alloc_size = size = n * d;
    v = new real[size];

}



INLINE real & ulVector::operator () (const int &i, const int & d) const
//INLINE real & ulVector::operator () (const int &i, const int & d) 
{
    return entry(i, d);
}



INLINE void ulVector::resize(ulVector & v)
{
    resize(v.n, v.dim);
}
INLINE void ulVector::resize(const int & elements, const int & d)
{
    if (elements * d > alloc_size)
    {
        Release();
        Allocate(elements, d);
    }
    else
    {
        n = elements;
        dim = d;
        size = n * d;
    } 
    
}


INLINE void ulVector::CopyVector(const ulVector & src)
{
    int i;
    resize(src.n, src.dim);
    
    for(i=0; i<src.size; i++)
        v[i] = src.v[i]; 
} 

INLINE void ulVector::Print (char *title)
{
    int i, d;

    if (title)
        dprintf ("%s\n", title);
    for (i = 0; i < n; i++)
    {
        dprintf ("[%d] ", i);
        for (d = 0; d < dim; d++)
        {
            dprintf (FLOAT_FORMAT, entry (i, d));
        }
        dprintf ("\n");
    }
    dprintf ("\n");

    //fflush(stdout);

}



#endif		// #define	__ULVECTOR_H__
