/*********************************************************************NVMH2****
Path:  D:\Testbuild\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulMatrix.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/


#ifndef __ULMATRIX_H__
#define __ULMATRIX_H__



class ulMatrix 
{
public:
    typedef ulMatrix  * ulpMatrix;
public:
    ulVector  **m;
    ulMatrix *saved_matrix;
    // internal data fetch operators
    //const real & entry(const int &i, const int &j, const int & d) const;
    //real & entry(const int &i, const int &j, const int & d) ;
    real & entry(const int &i, const int &j, const int & d) const;
    
    //real & entry(const int &i, const int &j, const int & d);
    //real * entry_ptr(const int &i, const int &j, const int & d);
    
public:
    int rows, cols, dim;
    int alloc_row, alloc_col, alloc_dim;
    
    ulMatrix () { InitMatrix();}
    void InitMatrix();
    void ReInit();
    ~ulMatrix();
    ulMatrix (const int &row, const int &col, const int &dim);
    ulMatrix (const ulMatrix & from);
    
    void clear();
    void SaveMatrix();
    void RestoreMatrix();
    void SaveRow(int r);
    void RestoreRow(int r);
    
    real safe_entry(const int &i, const int &j, const int & d) const;
    
    //void CopyDimensionZero();
    //void CopyMatrix(const ulMatrix& src);
    
    
    
    //real & operator () (const int &i, const int &j, const int & dim) const;
    //real & operator () (const int &i, const int &j, const int & dim);
    real & operator () (const int &i, const int &j, const int & dim) const;
    //real & operator () (const int &i, const int &j, const int & dim) ;
    ulVector & operator () (const int &i);
    //ulMatrix operator * (const real &val);
    
    void GetTranspose(ulMatrix & trans);
    void SetTranspose (ulMatrix & trans) ;
    void SetIdentity ();
    void SetInverse();
    void CopyMatrix(const ulMatrix& src);
    
    
    void resize(const int &r, const int &c, const int &dim);
    void resize(ulMatrix & m);
    
    void Allocate(const int &r, const int &c, const int &dim);
    void Release();
    
    
    void set(const ulMatrix &mat);
    void seti( int r, int c, int dim, ...);
    void setd( int r, int c, int dim, ...);
    void setf( int r, int c, int dim, ...);
    
    //ulMatrix(const real &d);
    
    // Assignment operators
    
    ulMatrix& operator  = ( const ulMatrix & m );      // assignment of a ulMatrix
    //ulMatrix& operator += ( const ulMatrix & m );      // incrementation by a ulMatrix
    //ulMatrix& operator -= ( const ulMatrix & m );      // decrementation by a ulMatrix
    //ulMatrix& operator *= ( const real &d );      // multiplication by a constant
    //ulMatrix& operator /= ( const real &d );      // division by a constant
    
    // special functions
    
    Mat3 Rot();
    void SetRotation(const Mat3 & m);
    ulVector  Trans() ;
    
    ulVector  GetViewUp() ;
    void GetViewUp(ulVector  & v);
    
    ulVector  GetViewNormal() ;
    void GetViewNormal(ulVector  &v) ;
    
    ulVector  GetViewRight() ;
    void GetViewRight(ulVector  & v);
    
    //ulMatrix  & GetulMatrix1();
    
    //void Transpose();         // transpose
    
    void Invert();            // inverse
    HRESULT InvertArbitrary();            // inverse
    HRESULT InvertAffine();            // inverse
    void GetInverse(ulMatrix & q) ;
    HRESULT GetInverseArbitrary(ulMatrix  & q) ;
    HRESULT GetInverseAffine(ulMatrix  & q) ;
    void SetInverse(const ulMatrix  & q);
    HRESULT SetInverseArbitrary(const ulMatrix  & q);
    HRESULT SetInverseAffine(const ulMatrix  & q);
    
    //void GetTranspose(ulMatrix  & q) ;
    void SetToZero();           // make this zero
    
    void SetRotateXDeg(real degrees);
    void RotateXDeg(real degrees, bool post = false);
    
    void SetRotateYDeg(real degrees);
    void RotateYDeg(real degrees, bool post = false);
    
    void SetRotateZDeg(real degrees);
    void RotateZDeg(real degrees, bool post = false);
    
    
    void SetScale(real sx, real sy, real sz);
    void SetScale(const ulVector  & tvec);
    void Scale(real sx, real sy, real sz, bool post = false);
    void Scale(const ulVector  & tvec, bool post = false);
    
    
    
    void SetTranslate(const real &x, const real &y, const real &z);
    void SetTranslate(const ulVector  & tvec);
    
    void Translate(const real &tx, const real &ty, const real &tz, bool post = false);
    void Translate(const ulVector  & tvec, bool post = false);
    
    
    void SetTranslationPoint(const ulVector &pos);
    
    friend ulMatrix operator * (const ulMatrix & a, const ulMatrix & b);        // m1 * m2
    
    
    
    //ulMatrix & operator * (const ulMatrix & b);        // this * m2
    //ulVector & operator * (const ulVector & b);        // this * m2
    
    
    
    
    //friend ulMatrix operator - (const ulMatrix & a);         // -m1
    //friend ulMatrix operator + (const ulMatrix & a, const ulMatrix & b);      // m1 + m2
    //friend ulMatrix operator - (const ulMatrix & a, const ulMatrix & b);      // m1 - m2
    //friend ulMatrix operator * (const ulMatrix & a, const real d);      // m1 * d
    //friend ulMatrix operator * (const real d, const ulMatrix & a);      // d * m1
    //friend ulMatrix operator / (const ulMatrix & a, const real d);      // m1 / d
    //friend int operator == (const ulMatrix & a, const ulMatrix & b);      // m1 == m2 ?
    //friend int operator != (const ulMatrix & a, const ulMatrix & b);      // m1 != m2 ?
    friend ostream& operator << (ostream& s, ulMatrix & m);      // output to stream
    friend istream& operator >> (istream& s, ulMatrix & m);      // input from strm.
    
    
    real XformAxis(const ulVector  &v, int axis) ;
    void Print(char * title = 0, bool bydim = true) ;
};




INLINE void ulCopyMatrix(ulMatrix & dest, const ulMatrix& src)
{
    int i;
    dest.resize(src.rows, src.cols, src.dim);
    
    // copy all vectors
    for(i=0; i<src.rows; i++)
    {
        ulCopyVector(*dest.m[i], *src.m[i]);
    }
}

INLINE void ulMatrix::CopyMatrix(const ulMatrix& src)
{
    int i;
    resize(src.rows, src.cols, src.dim);
    
    // copy all vectors
    for(i=0; i<src.rows; i++)
    {
        ulCopyVector(*m[i], *src.m[i]);
    }
}

INLINE void ulMatrixMultiply(ulMatrix & c, const ulMatrix & a, const ulMatrix & b)
{
    real sum;
    c.resize(a.rows, b.cols, ulMax(a.dim, b.dim));
    //assert(a.row, b.col, ulMax(a.dim, b.dim));
    
    // special case of dim 1 and non dim 1
    
#if _DEBUG
    assert(a.cols == b.rows);
#endif
    int	d, i, j, k;
    
    if (a.dim == 1 && b.dim > 1)
    {
        // dimension a is duplicated
        for(d = 0; d < b.dim; d++)
        {
            for(i = 0; i < a.rows; i++)
            {
                for(j = 0; j < b.cols; j++)
                {
                    sum = 0;
                    for(k = 0; k < a.cols; k++)
                        sum += a(i, k, 0) * b(k, j, d);
                    
                    c(i, j, d) = sum;
                }
            } 
        }
        
    }
    else if (a.dim > 1 && b.dim == 1)
    {
        
        for(d = 0; d < a.dim; d++)
        {
            for(i = 0; i < a.rows; i++)
            {
                for(j = 0; j < b.cols; j++)
                {
                    sum = 0;
                    for(k = 0; k < a.cols; k++)
                        sum += a(i, k, d) * b(k, j, 0);
                    
                    c(i, j, d) = sum;
                }
            } 
        }
    }
    else
    {
        
#if _DEBUG
        assert(a.dim == b.dim);
#endif
        
        for(d = 0; d < a.dim; d++)
        {
            for(i = 0; i < a.rows; i++)
            {
                for(j = 0; j < b.cols; j++)
                {
                    sum = 0;
                    for(k = 0; k < a.cols; k++)
                        sum += a(i, k, d) * b(k, j, d);
                    
                    c(i, j, d) = sum;
                }
            } 
        }
    }
}









INLINE real & ulMatrix::entry(const int &i, const int &j, const int & d) const
{
#if _DEBUG
    assert (i < rows);
    assert (j < cols);
    assert (d < dim);
#endif  
    
    ulVector &p = *m[i];
    return p(j, d);
    //return m[i].v[j * dim + d];
    
}



INLINE ulVector & ulMatrix::operator () (const int &i) 
{
    return *m[i];
}


INLINE real ulMatrix::safe_entry(const int &i, const int &j, const int & d) const
{
    if (i < 0 || i >= rows)
        return 0;
    
    else if (j < 0 || j >= cols)
        return 0;
    
    else if (d < 0 || d >= dim)
        return 0;
    
    else 
        return entry(i, j, d);
}


INLINE real & ulMatrix::operator () (const int &i, const int &j, const int & d) const
{
    return entry(i, j, d);
}






INLINE void ulMatrix::resize(const int &r, const int &c, const int &d)
{       
    int i;
    int nr, nc, nd;
    
    if (r > alloc_row || ((c * d) > (alloc_col * alloc_dim)))
    {
        nr = ulMax(r, alloc_row);
        nc = ulMax(c, alloc_col);
        nd = ulMax(d, alloc_dim);
        
        Release();
        
        Allocate(nr, nc, nd);
    }
    
    rows = r;
    cols = c;     
    dim = d;   
    
    for(i=0; i<rows; i++)
        m[i]->resize(cols, dim);
    
}


INLINE void ulMatrix::resize(ulMatrix & src)
{
    resize(src.rows, src.cols, src.dim);
}


INLINE void ulMatrix::Allocate (const int &r, const int &c, const int &d)
{
    int i;

    alloc_row = rows = r;
    alloc_col = cols = c;
    alloc_dim = dim = d;

    // allocate rows and column
    m = new ulVector *[alloc_row];
    for (i = 0; i < alloc_row; i++)
        m[i] = new ulVector (alloc_col, alloc_dim);


}


INLINE void ulMatrix::Release()
{
    int i;

    for (i = 0; i < alloc_row; i++)
    {
        delete m[i];
        m[i] = 0;
    }

    if (alloc_row)
    {
        SAFE_ARRAY_DELETE (m);

    }

    ReInit ();
    //alloc_row = 0;
    //alloc_col = 0;
    //alloc_dim = 0;

}


INLINE ulMatrix & ulMatrix::operator = (const ulMatrix & m)
{
    CopyMatrix (m);
    return *this;
}


INLINE ulMatrix operator *(const ulMatrix & a, const ulMatrix & b)
{
    ulMatrix temp;

    ulMatrixMultiply (temp, a, b);
    return temp;
}

INLINE void ulMatrix::GetTranspose (ulMatrix & a)
{
    int d, i, j;

    a.resize (cols, rows, dim);

    for (d = 0; d < dim; d++)
        for (i = 0; i < rows; i++)
            for (j = 0; j < cols; j++)
                a (j, i, d) = entry (i, j, d);
}



INLINE int BigInvert (const ulMatrix & mat, ulMatrix & out)
{
    int i, j, k, l, m;
    double temp;
    int d;
    int n = mat.rows;

    ulMatrix ainv (n, n, mat.dim);
    ulMatrix c (n, n, mat.dim);

    for (d = 0; d < mat.dim; d++)
    {
        //out.cols = out.rows = n;  

        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                ainv (i, j, d) = 0.0;
                c (i, j, d) = mat (i, j, d);
            }

            ainv (i, i, d) = 1.0;
        }

        for (i = 0; i < n; i++)
        {
            if (c (i, i, d) == 0.0)
            {
                return i + 1;
            }
        }

        for (m = 0; m < n; m++)
        {
            for (k = 0; k < n; k++)
            {
                temp = 1.0 / c (k, k, d);

                for (j = k; j < n; j++)
                    c (k, j, d) *= temp;

                ainv (k, m, d) *= temp;

                for (j = 0; j < n; j++)
                {
                    if (k != j)
                    {
                        temp = c (j, k, d);

                        for (l = k; l < n; l++)
                            c (j, l, d) -= c (k, l, d) * temp;

                        ainv (j, m, d) -= ainv (k, m, d) * temp;
                    }
                }
            }

            for (i = 0; i < n; i++)
                for (j = 0; j < n; j++)
                    c (i, j, d) = mat (i, j, d);

        }

        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                out (i, j, d) = ainv (i, j, d);
    }
    return 0;
}


INLINE void ulMatrix::ReInit ()
{
    m = NULL;
    rows = 0;
    alloc_row = 0;
    cols = 0;
    alloc_col = 0;
    dim = 0;
    alloc_dim = 0;
}

                         
INLINE void ulMatrix::InitMatrix()
{
    m = NULL;
    rows = 0;
    alloc_row = 0;
    cols = 0;
    alloc_col = 0;
    dim = 0;
    alloc_dim = 0;
    saved_matrix = 0;
}



INLINE ulMatrix::ulMatrix (const ulMatrix & other)
{
    int i;
    InitMatrix();
    Allocate (other.rows, other.cols, other.dim);
    //*this = other;
    // copy all vectors

    for (i = 0; i < other.rows; i++)
        ulCopyVector (*m[i], *other.m[i]);

}

INLINE ulMatrix::ulMatrix (const int &r, const int &c, const int &d)
{
    InitMatrix();
    Allocate (r, c, d);
}


INLINE ulMatrix::~ulMatrix ()
{
    Release ();

    if (saved_matrix)
        delete saved_matrix;

}


INLINE void ulMatrix::Print (char *title, bool bydim)
{
#if 0
    int i, j, d;
    if (title)
        dprintf ("%s\n", title);

    if (bydim)
    {
        for (d = 0; d < dim; d++)
        {
            for (i = 0; i < rows; i++)
            {
                dprintf ("[%d] ", i);
                for (j = 0; j < cols; j++)
                    dprintf (FLOAT_FORMAT, entry (i, j, d));

                dprintf ("\n");
            }
            dprintf ("\n");
        }

        dprintf ("\n");
    }
    else
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < cols; j++)
            {
                dprintf ("[%d][%d] ", i, j);
                for (d = 0; d < dim; d++)
                {
                    dprintf (FLOAT_FORMAT, entry (i, j, d));
                }
                dprintf ("\n");
            }
            dprintf ("\n");
        }
        dprintf ("\n");
    }
    fflush (stdout);
#endif
}


INLINE void ulMatrix::SetIdentity ()
{
    clear ();
    int i, d;
    for (d = 0; d < dim; d++)
        for (i = 0; i < ulMin (rows, cols); i++)
            entry (i, i, d) = 1;
}






////////////////// ulMatrix ///////////////////

INLINE void ulMatrixSubtract (ulMatrix & ret, const ulMatrix & a, const ulMatrix & b)
{
    int d;
    int r, c;

#if _DEBUG
    assert (a.dim == b.dim);
    assert (a.rows == b.rows);
    assert (a.cols == b.cols);
#endif

    ret.resize (a.rows, a.cols, a.dim);

    for (d = 0; d < a.dim; d++)
        for (r = 0; r < a.rows; r++)
            for (c = 0; c < a.cols; c++)
                ret (r, c, d) = a (r, c, d) - b (r, c, d);
}













// must be square
// Gauss-Jordan elimination with partial pivoting
INLINE int BigInvert2 (ulMatrix & a, ulMatrix & b)
{
    int k, n;
    a.SaveMatrix ();
    // As a evolves from original mat into identiy
    // b evolves from identity into inverse(a)
    int d, i, j, i1;


#if _DEBUG
    assert (a.rows == a.cols);
    assert (b.rows == b.cols);
    assert (a.rows == b.rows);
    assert (a.dim == b.dim);
#endif

    for (d = 0; d < a.dim; d++)
    {
        n = a.rows;
        //b.rows = b.cols = n;

        b.SetIdentity ();

        // Loop over cols of a from left to right, eliminating above and below diag
        for (j = 0; j < n; j++)
        {
            // Find largest pivot in column j among rows j..3
            i1 = j;
            // rows with largest pivot candidate
            for (i = j + 1; i < n; i++)
                if (fabs (a (i, j, d)) > fabs (a (i1, j, d)))
                    i1 = i;

            // Swap rows i1 and j in a and b to put pivot on diagonal
            for (k = 0; k < n; k++)
            {
                ulSwap (a (i1, k, d), a (j, k, d));
                ulSwap (b (i1, k, d), b (j, k, d));
            }

            // Scale rows j to have a unit diagonal
            if (a (j, j, d) == 0.0)
            {
                a.RestoreMatrix ();
                return 1;
            }
            //V_ERROR("mat4::inverse: singular matrix; can't invert\n");

            for (k = 0; k < n; k++)
            {
                b (j, k, d) /= a (j, j, d);
                a (j, k, d) /= a (j, j, d);
            }
            // Eliminate off-diagonal elems in cols j of a, doing identical ops to b
            for (i = 0; i < n; i++)
                if (i != j)
                {
                    for (k = 0; k < n; k++)
                    {
                        b (i, k, d) -= a (i, j, d) * b (j, k, d);
                        a (i, k, d) -= a (i, j, d) * a (j, k, d);
                    }
                }
        }

    }
    a.RestoreMatrix ();
    return 0;
}

/*
void ulMatrix::SetInverse()
{
ulMatrix a(rows, rows, dim);

  // invert m into a
  BigInvert(a, *this);
  
    //copy a back onto m
    *this = a;
    
} */

INLINE void ulMatrix::clear()
{
    int i;
    for (i = 0; i < rows; i++)
        m[i]->clear();
}






INLINE void ulMatrix::SaveRow (int r)
{
    m[r]->SaveVector ();
}


INLINE void ulMatrix::RestoreRow (int r)
{
    m[r]->RestoreVector ();

}


INLINE void ulMatrix::SaveMatrix ()
{

    if (saved_matrix == 0)
    {
        saved_matrix = new ulMatrix (rows, cols, dim);
    }
    saved_matrix->resize(rows, cols, dim);

    //*saved_matrix = *this;
    ulCopyMatrix (*saved_matrix, *this);

}


INLINE void ulMatrix::RestoreMatrix ()
{
#if _DEBUG
    assert (saved_matrix != 0);
#endif
    //*this = *saved_matrix;
    CopyMatrix (*saved_matrix);
}

  /*void ulMatrix::CopyDimensionZero()
     {
     int i,j,d;

     for(d=1; d<dim; d++)
     for(i=0; i<rows; i++)
     for(j=0; j<cols; j++)
     entry(i, j, d) = entry(i, j, 0);

     } 



     void CopyToDimensionZero(ulMatrix &dst, ulMatrix & src)
     {
     int r,c;
     for(r=0; r<src.rows; r++)
     for(c=0; c<src.cols; c++)
     dst(r, c, 0) = src(r, c, 0);

                                                         } */



INLINE void ulMatrix::seti (int _row, int _col, int _dim, ...)
{
    int r, c;
    //real t;

    resize (_row, _col, _dim);

    va_list list;


    va_start (list, _dim);      /* Initialize variable arguments. */

    for (r = 0; r < _row; r++)
    {
        for (c = 0; c < _col; c++)
            entry (r, c, 0) = va_arg (list, int);
    }

    va_end (list);              /* Reset variable arguments.      */
}



INLINE void ulMatrix::setf (int _row, int _col, int _dim, ...)
{
    int r, c;
    real t;

    resize (_row, _col, _dim);

    va_list list;


    va_start (list, _dim);      /* Initialize variable arguments. */

    for (r = 0; r < _row; r++)
    {
        for (c = 0; c < _col; c++)
        {
            entry (r, c, 0) = t = va_arg (list, real);
        }
    }

    va_end (list);              /* Reset variable arguments.      */
}



INLINE void ulMatrix::setd (int _row, int _col, int _dim, ...)
{
    int r, c;
    double t;

    resize (_row, _col, _dim);

    va_list list;


    va_start (list, _dim);      /* Initialize variable arguments. */

    for (r = 0; r < _row; r++)
    {
        for (c = 0; c < _col; c++)
            entry (r, c, 0) = t = va_arg (list, double);
    }

    va_end (list);              /* Reset variable arguments.      */
}







                                                      /*
                                                         void ulMatrix::Transpose()
                                                         {

                                                         int i,j,d;
                                                         ulMatrix trans(cols, rows, dim);

                                                         for(d=0; d<dim; d++)
                                                         for(i=0; i<rows; i++)
                                                         for(j=0; j<cols; j++)
                                                         trans(j, i, d) = entry(i, j, d);

                                                         CopyMatrix(trans);
                                                         }



                                                         ulMatrix ulMatrix::GetTranspose() 
                                                         {
                                                         int d,i, j;

                                                         ulMatrix a(cols, rows, dim);


                                                         for(d=0; d<dim; d++)
                                                         for(i=0; i<rows; i++)
                                                         for(j=0; j<cols; j++)
                                                         a(j, i, d) = entry(i, j, d);

                                                         return a;
                                                         }

                                                       */




                                                      /*

                                                         ulMatrix  ulMatrix::operator * (const real &val)
                                                         { 
                                                         int d,i,j;
                                                         ulMatrix b(rows, cols, dim);


                                                         for(i=0; i<rows; i++)
                                                         for(j=0; j<cols; j++)
                                                         for(d=0; d<dim; d++)
                                                         b(i,j,d) = entry(i, j, d) * val;

                                                         //return b;
                                                         }



                                                         ulVector& ulVector::operator = (const real &val)
                                                         { 
                                                         int i;
                                                         for(i=0; i<size; i++)
                                                         v[i] = val; 
                                                         return *this; 
                                                         }
                                                       */









#pragma warning(disable : 4305) // real to real
#pragma warning(disable : 4101)






////////////////// ulVector ///////////////////







#define MGG2_MAX_ORDER 7




// optimized function for multiplying A + T1*P*T2, where
// T1 is an upper-right triangular matrix (an FD matrix)
// P is a diagonal power matrix (powers of p along the diagonal)
// T2 is a lower-left triangular matrix (a Bezier matrix)

INLINE void _MGG2_MatrixMult_TPT (ulMatrix & result, const ulMatrix & T1, real P,
                           const ulMatrix & T2)
{

#if _DEBUG
    assert (T1.dim == T2.dim && T1.cols == T2.rows);
#endif

    int r, c, k, d;
    int order = T1.cols;

    real power[MGG2_MAX_ORDER];

    power[0] = 1.0f;
    for (k = 1; k < order; k++)
        power[k] = power[k - 1] * P;

    for (d = 0; d < T1.dim; d++)
    {
        for (c = 0; c < order; c++)
        {
            for (r = 0; r < order; r++)
            {
                real total = 0;
                // because of their triangular nature, we can start at max(c,r):
                for (k = (c > r) ? r : c; k < order; k++)
                    total += T1 (r, k, d) * T2 (k, c, d) * power[k];
                result (r, c, d) = total;
            }
        }
    }
}





INLINE void ulMatrixMultiplyVector (ulVector & av, const ulMatrix & a,
                             const ulVector & v)
{
    int r, c;

    av.resize (a.rows, v.dim);

#if _DEBUG
    assert (v.n == a.cols);
#endif
    // special case of flat matrix
    if (a.dim == 1 && v.dim > 1)
    {

        for (int d = 0; d < v.dim; d++)
        {
            for (r = 0; r < a.rows; r++)
            {
                av (r, d) = 0;
                for (c = 0; c < a.cols; c++)
                {
                    av (r, d) += a (r, c, 0) * v (c, d);
                }
            }
        }

    }
    else
    {
#if _DEBUG
        assert (v.dim == a.dim);
#endif

        for (int d = 0; d < v.dim; d++)
        {
            for (r = 0; r < a.rows; r++)
            {
                av (r, d) = 0;
                for (c = 0; c < a.cols; c++)
                {
                    av (r, d) += a (r, c, d) * v (c, d);
                }
            }
        }
    }
}




// reverse rows  & cols
INLINE void ulMatrixMultiplyVector (ulVector & av, const ulVector & v,
                             const ulMatrix & a)
{
    int r, c;
#if _DEBUG
    assert (v.n == a.rows);
    assert (v.dim == a.dim);
#endif

    av.resize (a.cols, v.dim);

    for (int d = 0; d < v.dim; d++)
    {
        for (c = 0; c < a.cols; c++)
        {
            av (c, d) = 0;
            for (r = 0; r < a.rows; r++)
            {
                av (c, d) += a (r, c, d) * v (r, d);
            }
        }
    }
}







INLINE ulVector operator *(const ulVector & v, const ulMatrix & a)
{
    ulVector temp;
    ulMatrixMultiplyVector (temp, v, a);
    return temp;

}






INLINE ulVector operator *(const ulMatrix & a, const ulVector & v)
{
    ulVector temp;
    ulMatrixMultiplyVector (temp, a, v);
    return temp;

}


#endif			// #define __ULMATRIX_H__


