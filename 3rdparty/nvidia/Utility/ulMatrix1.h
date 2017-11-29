

class ulMatrix1
{
public:
    typedef ulMatrix1  * ulpMatrix1;
public:
    real  **m;
    real *saved_matrix;
    // internal data fetch operators
    //const real & entry(const int &i, const int &j, const int & d) const;
    //real & entry(const int &i, const int &j, const int & d) ;
    //real & entry(const int &i, const int &j, const int & d) const;
    
    //real & entry(const int &i, const int &j, const int & d);
    //real * entry_ptr(const int &i, const int &j, const int & d);
    
public:
    int rows, cols;
    int alloc_row, alloc_col;
    
    ulMatrix1 () { InitMatrix1();}
    void InitMatrix1();
    void ReInit();
    ~ulMatrix1();
    ulMatrix1 (const int &row, const int &col);
    ulMatrix1 (const ulMatrix1 & from);
    
    void clear();
    void SaveMatrix();
    void RestoreMatrix();
    void SaveRow(int r);
    void RestoreRow(int r);
    
    real safe_entry(const int &i, const int &j, const int & d) const;
    
    //void CopyMatrix(const ulMatrix1& src);
    
    
    
    real & operator () (const int &i, const int &j) const;
    ulVector & operator () (const int &i);
    
    void GetTranspose(ulMatrix1 & trans);
    void SetTranspose (ulMatrix1 & trans) ;
    void SetIdentity ();
    void SetInverse();
    void CopyMatrix(const ulMatrix1& src);
    
    
    void resize(const int &r, const int &c);
    void resize(ulMatrix1 & m);
    
    void Allocate(const int &r, const int &c);
    void Release();
    
    
    void set(const ulMatrix1 &mat);
    void seti( int r, int c, ...);
    void setd( int r, int c, ...);
    void setf( int r, int c, ...);
    
    //ulMatrix1(const real &d);
    
    // Assignment operators
    ulMatrix1& operator  = ( const ulMatrix1 & m );      // assignment of a ulMatrix1
    ulMatrix1& operator += ( const ulMatrix1 & m );      // incrementation by a ulMatrix1
    //ulMatrix1& operator -= ( const ulMatrix1 & m );      // decrementation by a ulMatrix1
    //ulMatrix1& operator *= ( const real &d );      // multiplication by a constant
    //ulMatrix1& operator /= ( const real &d );      // division by a constant
    
    // special functions
    
    Mat3 Rot();
    void SetRotation(const Mat3 & m);
    ulVector1  Trans() ;
    
    ulVector1  GetViewUp() ;
    void GetViewUp(ulVector1  & v);
    
    ulVector1  GetViewNormal() ;
    void GetViewNormal(ulVector1  &v) ;
    
    ulVector1  GetViewRight() ;
    void GetViewRight(ulVector1  & v);
    
    ulMatrix1  & GetulMatrix1();
    
    //void Transpose();         // transpose
    
    void Invert();            // inverse
    HRESULT InvertArbitrary();            // inverse
    HRESULT InvertAffine();            // inverse
    void GetInverse(ulMatrix1 & q) ;
    HRESULT GetInverseArbitrary(ulMatrix1  & q) ;
    HRESULT GetInverseAffine(ulMatrix1  & q) ;
    void SetInverse(const ulMatrix1  & q);
    HRESULT SetInverseArbitrary(const ulMatrix1  & q);
    HRESULT SetInverseAffine(const ulMatrix1  & q);
    
    //void GetTranspose(ulMatrix1  & q) ;
    void SetToZero();           // make this zero
    
    void SetRotateXDeg(real degrees);
    void RotateXDeg(real degrees, bool post = false);
    
    void SetRotateYDeg(real degrees);
    void RotateYDeg(real degrees, bool post = false);
    
    void SetRotateZDeg(real degrees);
    void RotateZDeg(real degrees, bool post = false);
    
    
    void SetScale(real sx, real sy, real sz);
    void SetScale(const ulVector1  & tvec);
    void Scale(real sx, real sy, real sz, bool post = false);
    void Scale(const ulVector1  & tvec, bool post = false);
    
    
    
    void SetTranslate(const real &x, const real &y, const real &z);
    void SetTranslate(const ulVector1  & tvec);
    
    void Translate(const real &tx, const real &ty, const real &tz, bool post = false);
    void Translate(const ulVector1  & tvec, bool post = false);
    
    
    void SetTranslationPoint(const ulVector1 &pos);
    
    friend ulMatrix1 operator + (const ulMatrix1 & a, const ulMatrix1 & b);        // m1 * m2
    
    
    
    //ulMatrix1 & operator * (const ulMatrix1 & b);        // this * m2
    //ulVector1 & operator * (const ulVector1 & b);        // this * m2
    
    
    friend ulMatrix1 operator * (const ulMatrix1 & a, const ulMatrix1 & b);      // m1 - m2
    
    
    //friend ulMatrix1 operator - (const ulMatrix1 & a);         // -m1
    //friend ulMatrix1 operator + (const ulMatrix1 & a, const ulMatrix1 & b);      // m1 + m2
    //friend ulMatrix1 operator * (const ulMatrix1 & a, const real d);      // m1 * d
    //friend ulMatrix1 operator * (const real d, const ulMatrix1 & a);      // d * m1
    //friend ulMatrix1 operator / (const ulMatrix1 & a, const real d);      // m1 / d
    //friend int operator == (const ulMatrix1 & a, const ulMatrix1 & b);      // m1 == m2 ?
    //friend int operator != (const ulMatrix1 & a, const ulMatrix1 & b);      // m1 != m2 ?
    friend ostream& operator << (ostream& s, ulMatrix1 & m);      // output to stream
    friend istream& operator >> (istream& s, ulMatrix1 & m);      // input from strm.
    
    
    real XformAxis(const ulVector1  &v, int axis) ;
    void Print(char * title = 0) ;
};




INLINE void ulMatrixMultiplyVector (ulVector1 & av, const ulVector1 & v,
                             const ulMatrix1 & a)
{
    int r, c;
#if _DEBUG
    assert (v.n == a.rows);
#endif

    av.resize (a.cols);

    for (c = 0; c < a.cols; c++)
    {
        av[c] = 0;
        for (r = 0; r < a.rows; r++)
        {
            av[c] += a (r, c) * v[r];
        }
    }
}


INLINE void ulMatrixMultiplyVector (ulVector1 & av, const ulMatrix1 & a,
                             const ulVector1 & v)
{
    int r, c;

    av.resize (a.rows);

#if _DEBUG
    assert (v.n == a.cols);
#endif

    for (r = 0; r < a.rows; r++)
    {
        av[r] = 0;
        for (c = 0; c < a.cols; c++)
        {
            av[r] += a (r, c) * v[c];
        }
    }
}
INLINE void ulMatrixMultiply (ulMatrix1 & c, const ulMatrix1 & a,
                       const ulMatrix1 & b)
{
    real sum;
    c.resize (a.rows, b.cols);


#if _DEBUG
    assert (a.cols == b.rows);
#endif
    int i, j, k;

    for (i = 0; i < a.rows; i++)
    {
        for (j = 0; j < b.cols; j++)
        {
            sum = 0;
            for (k = 0; k < a.cols; k++)
                sum += a (i, k) * b (k, j);

            c (i, j) = sum;
        }
    }
}






INLINE ulVector1 operator *(const ulMatrix1 & a, const ulVector1 & v)
{
    ulVector1 temp(v.n);
    ulMatrixMultiplyVector (temp, a, v);
    return temp;

}
INLINE ulVector1 operator *(const ulVector1 & v, const ulMatrix1 & a)
{
    ulVector1 temp(v.n);
    ulMatrixMultiplyVector (temp, v, a);
    return temp;

}



INLINE real & ulMatrix1::operator () (const int &i, const int &j) const
{
    return m[i][j];
}



INLINE ulMatrix1::ulMatrix1 (const int &r, const int &c)
{
    InitMatrix1();
    Allocate (r, c);
}



INLINE ulMatrix1::ulMatrix1 (const ulMatrix1 & other)
{
    InitMatrix1();
    Allocate (other.rows, other.cols);
    CopyMatrix (other);
}




INLINE void ulMatrix1::InitMatrix1()
{
    m = NULL;
    rows = 0;
    alloc_row = 0;
    cols = 0;
    alloc_col = 0;
    saved_matrix = 0;
}





INLINE void ulMatrix1::Allocate (const int &r, const int &c)
{
    int i;

    alloc_row = rows = r;
    alloc_col = cols = c;

    // allocate rows and column
    m = new real *[alloc_row];
    for (i = 0; i < alloc_row; i++)
        m[i] = new real[alloc_col];


}



INLINE ulMatrix1::~ulMatrix1 ()
{
    Release ();

    if (saved_matrix)
        delete saved_matrix;
}

INLINE void ulMatrix1::Release ()
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


INLINE void ulMatrix1::ReInit ()
{
    m = NULL;
    rows = 0;
    alloc_row = 0;
    cols = 0;
    alloc_col = 0;
}




INLINE ulMatrix1& ulMatrix1::operator += ( const ulMatrix1& src )
{ 
    int r, c;
    
    for (r = 0; r < src.rows; r++)
    {
        for (c = 0; c < src.cols; c++)
        {
            m[r][c] += src (r, c);
        }
    }
        
    return *this; 
}

INLINE ulMatrix1 & ulMatrix1::operator = (const ulMatrix1 & m)
{
    CopyMatrix (m);
    return *this;
}


INLINE void ulMatrix1::CopyMatrix (const ulMatrix1 & src)
{
    int r, c;
    resize (src.rows, src.cols);

    // copy all vectors
    for (r = 0; r < src.rows; r++)
        for (c = 0; c < src.cols; c++)
        {
            m[r][c] = src (r, c);
        }
}



INLINE void ulMatrix1::resize (const int &r, const int &c)
{
    //int i;
    int nr, nc;

    if (r > alloc_row || c > alloc_col)
    {
        nr = ulMax (r, alloc_row);
        nc = ulMax (c, alloc_col);

        Release ();

        Allocate (nr, nc);
    }

    rows = r;
    cols = c;

}


INLINE void ulMatrix1::seti (int _row, int _col, ...)
{
    int r, c;
    //real t;

    resize (_row, _col);

    va_list list;


    va_start (list, _col);      /* Initialize variable arguments. */

    for (r = 0; r < _row; r++)
    {
        for (c = 0; c < _col; c++)
            m[r][c] = va_arg (list, int);
    }

    va_end (list);              /* Reset variable arguments.      */
}



INLINE ulMatrix1 operator + (const ulMatrix1 & a, const ulMatrix1 & b)
{

    ulMatrix1 ret;

    ret.resize (a.rows, a.cols);
    int r, c;

    for (r = 0; r < a.rows; r++)
    {
        for (c = 0; c < a.cols; c++)
        {
            ret (r, c) = a (r, c) + b (r, c);
        }
    }



    return ret;

}
                                                      // copy constructor
                                                      /*ulVector1::ulVector1( const ulVector1& other) // Declare copy constructor.
                                                         { 
                                                         int i;
                                                         for(i=0; i<n; i++)
                                                         {
                                                         v[i] = other[i];
                                                         }
                                                         } */

INLINE void ulMatrix1::clear()
{
    int i, j;
    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            m[i][j] = 0;
}



