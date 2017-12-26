/*********************************************************************NVMH1****
File:
ulSpline.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/

#include <ul.h>

/*
   Interpolating cubic spline function for equispaced points
   Input: Y1 is a list of equispaced data points with len1 entries
   Output: Y2 <- cubic spline sampled at len2 equispaced points 
 */



void ulComputeYDDerivative(float *Y, int len, float *YD);
void ulComputeYDDerivative(real64 *Y, int len, real64 *YD);
void ulTriDiagonal(float *D, int len);
void ulTriDiagonal(real64 *D, int len);
int ulComputeYDDerivativeIrregular(float *X, float *Y, float *YD, int len);
int ulComputeYDDerivativeIrregular(real64 *X, real64 *Y, real64 *YD, int len);
int ulTriDiagonalIrregular(float *A, float *B, float *C, float *D, int len);
int ulTriDiagonalIrregular(real64 *A, real64 *B, real64 *C, real64 *D, int len);


void ulCubicSpline(real64 * Y1, int len1, real64 *Y2, int len2)
{
    int i, ip, oip;
    real64 *YD, A0, A1, A2, A3, x, p, fctr;
    
    // compute 1st derivatives at each point -> YD 
    YD = new real64[len1];
    for(i=0; i<len1; i++)
        YD[i] = 0;
    
    
    //printf("Y1  len1 %d len2 %d\n", len1, len2);
    /*for(i=0; i<len1; i++)
    printf("%g ", Y1[i]);
    printf("\n");*/
    ulComputeYDDerivative(Y1, len1, YD);
    
    /*
    p is real-valued position into spline
    * ip is interval's left endpoint (integer)
    oip is left endpoint of last visited interval
    */
    oip = -1;                     // force coefficient initialization 
    
    fctr = (real64) (len1 - 1) / (len2 - 1);
    
    p = 0.0;
    for (i = ip = 0; i < len2; i++)
    {
        // check if in new interval 
        if (ip != oip)
        {
            /* update interval */
            oip = ip;
            
            // compute spline coefficients 
            A0 = Y1[ip];
            A1 = YD[ip];
            A2 =  3.0 * (Y1[ip + 1] - Y1[ip]) - 2.0 * YD[ip] - YD[ip + 1];
            A3 = -2.0 * (Y1[ip + 1] - Y1[ip]) +       YD[ip] + YD[ip + 1];
        }
        // use Homer's rule to calculate cubic polynomial 
        x = p - ip;
        Y2[i] = ((A3 * x + A2) * x + A1) * x + A0;
        
        // increment pointer
        ip = (int)(p += fctr);
    }
    SAFE_ARRAY_DELETE(YD);
}

/*
YD <- Computed 1st derivative of data in Y (len entries)
The not-a-knot boundary condition is used
*/
void ulComputeYDDerivative(real64 *Y, int len, real64 *YD)
{
    int i;
    
    YD[0] = -5.0 * Y[0] + 4.0 * Y[1] + Y[2];
    for (i = 1; i < len - 1; i++)
        YD[i] = 3.0 * (Y[i + 1] - Y[i - 1]);
    
    YD[len - 1] = -Y[len - 3] - 4.0 * Y[len - 2] + 5.0 * Y[len - 1];
    
    /* solve for the tridiagonal matrix: YD=YD*inv(tridiag matrix) */
    ulTriDiagonal(YD, len);
}
/*************************************************~*
Linear time Gauss Elimination with backsubstitution for 141
tridiagonal matrix with column vector D. Result goes into D
*************************************************************/

void ulTriDiagonal(real64 *D, int len)
{
    int i;
    real64 *C;
    
    /* init first two entries; C is right band of tridiagonal */
    C = new real64[len];
    for(i=0; i<len; i++)
        C[i] = 0;
    
    D[0] = 0.5 * D[0];
    D[1] = (D[1] - D[0]) / 2.0;
    
    C[1] = 2.0;
    
    /* Gauss elimination; forward substitution */
    for (i = 2; i < len - 1; i++)
    {
        C[i] = 1.0 / (4.0 - C[i - 1]);
        D[i] = (D[i] - D[i - 1]) / (4.0 - C[i]);
    }
    C[i] = 1.0 / (4.0 - C[i - 1]);
    D[i] = (D[i] - 4 * D[i - 1]) / (2.0 - 4 * C[i]);
    
    /* backsubstitution */
    for (i = len - 2; i >= 0; i--)
        D[i] -= (D[i + 1] * C[i + 1])
        ;
    SAFE_ARRAY_DELETE(C);
}

/*
   Interpolating cubic spline function for irregularly-spaced points
   Input: Y1 is a list of irregular data points (len1 entries)
   Their x-coordinates are specified in X1
   Output: Y2 <- cubic spline sampled according to X2 (len2 entries)
   Assume that X1,X2 entries are monotonically increasing
 */

int ulCubicSplineIrregular(real64 *X1, real64 *Y1, int len1, real64 *X2, real64 *Y2, int len2)
{
    int retval, i, j;
    real64 *YD, A0, A1, A2, A3, x, dx, dy, p1, p2, p3;
    /* compute 1 st derivatives at each point -> YD */
    YD = new real64[len1];
    for(i=0; i<len1; i++)
        YD[i] = 0;
    
    retval = ulComputeYDDerivativeIrregular(X1, Y1, YD, len1);
    if (retval)
    {
        SAFE_ARRAY_DELETE(YD);
        return retval;
    }
    /* error checking */
    if (X2[0] < X1[0] || X2[len2 - 1] > X1[len1 - 1])
    {
        //fprintf(stderr, "ispline_gen: Out of range\n");
        SAFE_ARRAY_DELETE(YD);
        return -1;
    }
    /*
    p1 is left endpoint of interval
    p2 is resampling position
    p3 is right endpoint of interval
    j is input index for current interval
    */
    
    p3 = X2[0] - 1;               /* force coefficient initialization */
    for (i = j = 0; i < len2; i++)
    {
        /* check if in new interval */
        p2 = X2[i];
        
        if (p2 > p3)
        {
            /* find the interval which contains p2 */
            for (; j < len1 && p2 > X1[j]; j++)
                ;
            if (p2 < X1[j])
                j++;
            p1 = X1[j];           /* update left endpoint */
            p3 = X1[j + 1];       /* update right endpoint */
            /* compute spline coefficients */
            dx = 1.0 / (X1[j + 1] - X1[j]);
            dy = (Y1[j + 1] - Y1[j]) * dx;
            A0 = Y1[j];
            A1 = YD[j];
            A2 = dx * (3.0 * dy - 2.0 * YD[j] - YD[j + 1]);
            A3 = dx * dx * (-2.0 * dy + YD[j] + YD[j + 1]);
        }
        /* use Homer's rule to calculate cubic polynomial */
        x = p2 - p1;
        Y2[i] = ((A3 * x + A2) * x + A1) * x + A0;
    }
    
    SAFE_ARRAY_DELETE(YD);
    return 0;
}

/**************************************************
YD <- Computed 1 st derivative of data in X,Y (fen entries)
The not-a-knot boundary condition is used
**********************************************************/
int ulComputeYDDerivativeIrregular(real64 *X, real64 *Y, real64 *YD, int len)

{
    int i, retval;
    real64 h0, h1, r0, r1, *A, *B, *C;
    // allocate memory for tridiagonal bands A,B,C 
    A = new real64[len];
    B = new real64[len];
    C = new real64[len];
    for(i=0; i<len; i++)
    {
        A[i] = 0;
        B[i] = 0;
        C[i] = 0;
    }
    
    
    
    /* inn' first row data */
    h0 = X[1] - X[0];
    h1 = X[2] - X[1];
    r0 = (Y[1] - Y[0]) / h0;
    r1 = (Y[2] - Y[1]) / h1;
    
    B[0] = h1 * (h0 + h1);
    C[0] = (h0 + h1) * (h0 + h1);
    YD[0] = r0 * (3 * h0 * h1 + 2 * h1 * h1) + r1 * h0 * h0;
    
    // inn' tridiagonal bands A, B, C, and column vector YD */
    // YD will later be used to return the derivatives */
    
    for (i = 1; i < len - 1; i++)
    {
        h0 = X[i] - X[i - 1];
        h1 = X[i + 1] - X[i];
        r0 = (Y[i] - Y[i - 1]) / h0;
        r1 = (Y[i + 1] - Y[i]) / h1;
        
        A[i] = h1;
        B[i] = 2 * (h0 + h1);
        C[i] = h0;
        YD[i] = 3 * (r0 * h1 + r1 * h0);
    }
    // last row*/
    A[i] = (h0 + h1) * (h0 + h1);
    B[i] = h0 * (h0 + h1);
    YD[i] = r0 * h1 * h1 + r1 * (3 * h0 * h1 + 2 * h0 * h0);
    
    //r solve for the tridiagonal matrix: YD=YD*inv(tridiag matrix) */
    retval = ulTriDiagonalIrregular(A, B, C, YD, len);
    
    SAFE_ARRAY_DELETE(A);
    SAFE_ARRAY_DELETE(B);
    SAFE_ARRAY_DELETE(C);
    
    return retval;
}

/*************************************************
Gauss Elimination with backsubstitution for general
tridiagonal matrix with bands A,B,C and column vector D.
*/

int ulTriDiagonalIrregular(real64 *A, real64 *B, real64 *C, real64 *D, int len)
{
    int i;
    real64 b, *F;
    
    //F = (real64 *) calloc(len, sizeof(real64));
    F = new real64[len];
    for(i=0; i<len; i++)
        F[i] = 0;
    
    // Gauss elimination; forward substitution */
    b = B[0];
    D[0] = D[0] / b;
    for (i = 1; i < len; i++)
    {
        F[i] = C[i - 1] / b;
        b = B[i] - A[i] * F[i];
        if (b == 0.0)
        {
            //fprintf(stderr, "getYD_gen: divide-by-zero\n");
            return -1;
        }
        D[i] = (D[i] - D[i - 1] * A[i]) / b;
        
        // backsubstitution */
        for (i = len - 2; i >= 0; i--)
            D[i] -= (D[i + 1] * F[i + 1]);
        SAFE_ARRAY_DELETE(F);
    }
    return 0;
}



/*
scale an array of doubles
*/
void scale_array(real64 * in, int r1, int c1, real64 * result, int r2, int c2)
{
    int i;
    int r,c;
    real64 *vert_in; // r1
    real64 *vert_out; // r2
    real64 *interim;
    
    
    //vert_in = (real64 *) calloc(r1, sizeof(real64));
    //vert_out = (real64 *) calloc(r2, sizeof(real64));
    //interim = (real64 *) calloc(r1 * c2, sizeof(real64));
    
    vert_in = new real64[r1];
    vert_out = new real64[r2];
    interim = new real64[r1 * c2];
    for(i=0; i<r1; i++)
        vert_in[i] = 0;
    for(i=0; i<r2; i++)
        vert_out[i] = 0;
    for(i=0; i<r1*c2; i++)
        interim[i] = 0;
    
    
    for (r=0; r<r1; r++)
        ulCubicSpline(&in[r * c1], c1, &interim[r * c2], c2);
    
    
        /*printf("Interim\n");
        for (r=0; r<r1; r++)
        {
        for (c=0; c<c2; c++)		
        {
        printf("%g ", interim[r * c2 + c]);
        }
        printf("\n");
        }
    */
    
    
    for (c=0; c<c2; c++)
    {
        for(r=0; r<r1; r++)
            vert_in[r] = interim[r * c2 + c];  // transfer row to linear
        
        
        ulCubicSpline(vert_in, r1, vert_out, r2);
        
        for(r=0; r<r2; r++)
            result[r * c2 + c] = vert_out[r];
        
    }
    
    
    
    SAFE_ARRAY_DELETE(vert_in);
    SAFE_ARRAY_DELETE(vert_out);
    SAFE_ARRAY_DELETE(interim);
    
}

///////////////////////   float


void ulCubicSpline(float * Y1, int len1, float *Y2, int len2)
{
    int i, ip, oip;
    float *YD, A0, A1, A2, A3, x, p, fctr;
    
    // compute 1st derivatives at each point -> YD 
    YD = new float[len1];
    for(i=0; i<len1; i++)
        YD[i] = 0;
    
    
    //printf("Y1  len1 %d len2 %d\n", len1, len2);
    /*for(i=0; i<len1; i++)
    printf("%g ", Y1[i]);
    printf("\n");*/
    ulComputeYDDerivative(Y1, len1, YD);
    
    /*
    p is real-valued position into spline
    * ip is interval's left endpoint (integer)
    oip is left endpoint of last visited interval
    */
    oip = -1;                     // force coefficient initialization 
    
    fctr = (float) (len1 - 1) / (len2 - 1);
    
    p = 0.0;
    for (i = ip = 0; i < len2; i++)
    {
        // check if in new interval 
        if (ip != oip)
        {
            /* update interval */
            oip = ip;
            
            // compute spline coefficients 
            A0 = Y1[ip];
            A1 = YD[ip];
            A2 =  3.0 * (Y1[ip + 1] - Y1[ip]) - 2.0 * YD[ip] - YD[ip + 1];
            A3 = -2.0 * (Y1[ip + 1] - Y1[ip]) +       YD[ip] + YD[ip + 1];
        }
        // use Homer's rule to calculate cubic polynomial 
        x = p - ip;
        Y2[i] = ((A3 * x + A2) * x + A1) * x + A0;
        
        // increment pointer
        ip = (int)(p += fctr);
    }
    SAFE_ARRAY_DELETE(YD);
}

/*
YD <- Computed 1st derivative of data in Y (len entries)
The not-a-knot boundary condition is used
*/
void ulComputeYDDerivative(float *Y, int len, float *YD)
{
    int i;
    
    YD[0] = -5.0 * Y[0] + 4.0 * Y[1] + Y[2];
    for (i = 1; i < len - 1; i++)
        YD[i] = 3.0 * (Y[i + 1] - Y[i - 1]);
    
    YD[len - 1] = -Y[len - 3] - 4.0 * Y[len - 2] + 5.0 * Y[len - 1];
    
    /* solve for the tridiagonal matrix: YD=YD*inv(tridiag matrix) */
    ulTriDiagonal(YD, len);
}
/*************************************************~*
Linear time Gauss Elimination with backsubstitution for 141
tridiagonal matrix with column vector D. Result goes into D
*************************************************************/

void ulTriDiagonal(float *D, int len)
{
    int i;



    D[0] = 0.5 * D[0];
    if (len == 1)
        return;

    D[1] = (D[1] - D[0]) / 2.0;
    if (len == 2)
        return;


    float *C;
    
    /* init first two entries; C is right band of tridiagonal */
    C = new float[len];
    for(i=0; i<len; i++)
        C[i] = 0;
    

    C[1] = 2.0;
    
    /* Gauss elimination; forward substitution */
    for (i = 2; i < len - 1; i++)
    {
        C[i] = 1.0 / (4.0 - C[i - 1]);
        D[i] = (D[i] - D[i - 1]) / (4.0 - C[i]);
    }
    C[i] = 1.0 / (4.0 - C[i - 1]);
    D[i] = (D[i] - 4 * D[i - 1]) / (2.0 - 4 * C[i]);
    
    /* backsubstitution */
    for (i = len - 2; i >= 0; i--)
        D[i] -= (D[i + 1] * C[i + 1]);

    SAFE_ARRAY_DELETE(C);
}

/*
   Interpolating cubic spline function for irregularly-spaced points
   Input: Y1 is a list of irregular data points (len1 entries)
   Their x-coordinates are specified in X1
   Output: Y2 <- cubic spline sampled according to X2 (len2 entries)
   Assume that X1,X2 entries are monotonically increasing
 */

int ulCubicSplineIrregular(float *X1, float *Y1, int len1, float *X2, float *Y2, int len2)
{
    int retval, i, j;
    float *YD, A0, A1, A2, A3, x, dx, dy, p1, p2, p3;
    /* compute 1 st derivatives at each point -> YD */
    YD = new float[len1];
    for(i=0; i<len1; i++)
        YD[i] = 0;
    
    retval = ulComputeYDDerivativeIrregular(X1, Y1, YD, len1);
    if (retval)
    {
        SAFE_ARRAY_DELETE(YD);
        return retval;
    }
    /* error checking */
    if (X2[0] < X1[0] || X2[len2 - 1] > X1[len1 - 1])
    {
        //fprintf(stderr, "ispline_gen: Out of range\n");
        SAFE_ARRAY_DELETE(YD);
        return -1;
    }
    /*
    p1 is left endpoint of interval
    p2 is resampling position
    p3 is right endpoint of interval
    j is input index for current interval
    */
    
    p3 = X2[0] - 1;               /* force coefficient initialization */
    for (i = j = 0; i < len2; i++)
    {
        /* check if in new interval */
        p2 = X2[i];
        
        if (p2 > p3)
        {
            /* find the interval which contains p2 */
            for (; j < len1 && p2 > X1[j]; j++)
                ;
            if (p2 < X1[j])
                j++;
            p1 = X1[j];           /* update left endpoint */
            p3 = X1[j + 1];       /* update right endpoint */
            /* compute spline coefficients */
            dx = 1.0 / (X1[j + 1] - X1[j]);
            dy = (Y1[j + 1] - Y1[j]) * dx;
            A0 = Y1[j];
            A1 = YD[j];
            A2 = dx * (3.0 * dy - 2.0 * YD[j] - YD[j + 1]);
            A3 = dx * dx * (-2.0 * dy + YD[j] + YD[j + 1]);
        }
        /* use Homer's rule to calculate cubic polynomial */
        x = p2 - p1;
        Y2[i] = ((A3 * x + A2) * x + A1) * x + A0;
    }
    
    SAFE_ARRAY_DELETE(YD);
    return 0;
}

/**************************************************
YD <- Computed 1 st derivative of data in X,Y (fen entries)
The not-a-knot boundary condition is used
**********************************************************/
int ulComputeYDDerivativeIrregular(float *X, float *Y, float *YD, int len)

{
    int i, retval;
    float h0, h1, r0, r1, *A, *B, *C;
    // allocate memory for tridiagonal bands A,B,C 
    A = new float[len];
    B = new float[len];
    C = new float[len];
    for(i=0; i<len; i++)
    {
        A[i] = 0;
        B[i] = 0;
        C[i] = 0;
    }
    
    
    
    /* inn' first row data */
    h0 = X[1] - X[0];
    h1 = X[2] - X[1];
    r0 = (Y[1] - Y[0]) / h0;
    r1 = (Y[2] - Y[1]) / h1;
    
    B[0] = h1 * (h0 + h1);
    C[0] = (h0 + h1) * (h0 + h1);
    YD[0] = r0 * (3 * h0 * h1 + 2 * h1 * h1) + r1 * h0 * h0;
    
    // inn' tridiagonal bands A, B, C, and column vector YD */
    // YD will later be used to return the derivatives */
    
    for (i = 1; i < len - 1; i++)
    {
        h0 = X[i] - X[i - 1];
        h1 = X[i + 1] - X[i];
        r0 = (Y[i] - Y[i - 1]) / h0;
        r1 = (Y[i + 1] - Y[i]) / h1;
        
        A[i] = h1;
        B[i] = 2 * (h0 + h1);
        C[i] = h0;
        YD[i] = 3 * (r0 * h1 + r1 * h0);
    }
    // last row*/
    A[i] = (h0 + h1) * (h0 + h1);
    B[i] = h0 * (h0 + h1);
    YD[i] = r0 * h1 * h1 + r1 * (3 * h0 * h1 + 2 * h0 * h0);
    
    //r solve for the tridiagonal matrix: YD=YD*inv(tridiag matrix) */
    retval = ulTriDiagonalIrregular(A, B, C, YD, len);
    
    SAFE_ARRAY_DELETE(A);
    SAFE_ARRAY_DELETE(B);
    SAFE_ARRAY_DELETE(C);
    
    return retval;
}

/*************************************************
Gauss Elimination with backsubstitution for general
tridiagonal matrix with bands A,B,C and column vector D.
*/

int ulTriDiagonalIrregular(float *A, float *B, float *C, float *D, int len)
{
    int i;
    float b, *F;
    
    //F = (float *) calloc(len, sizeof(float));
    F = new float[len];
    for(i=0; i<len; i++)
        F[i] = 0;
    
    // Gauss elimination; forward substitution */
    b = B[0];
    D[0] = D[0] / b;
    for (i = 1; i < len; i++)
    {
        F[i] = C[i - 1] / b;
        b = B[i] - A[i] * F[i];
        if (b == 0.0)
        {
            //fprintf(stderr, "getYD_gen: divide-by-zero\n");
            return -1;
        }
        D[i] = (D[i] - D[i - 1] * A[i]) / b;
        
        // backsubstitution */
        for (i = len - 2; i >= 0; i--)
            D[i] -= (D[i + 1] * F[i + 1]);
        SAFE_ARRAY_DELETE(F);
    }
    return 0;
}



/*
scale an array of doubles
*/
void scale_array(float * in, int r1, int c1, float * result, int r2, int c2)
{
    int i;
    int r,c;
    float *vert_in; // r1
    float *vert_out; // r2
    float *interim;
    
    
    //vert_in = (float *) calloc(r1, sizeof(float));
    //vert_out = (float *) calloc(r2, sizeof(float));
    //interim = (float *) calloc(r1 * c2, sizeof(float));
    
    vert_in = new float[r1];
    vert_out = new float[r2];
    interim = new float[r1 * c2];
    for(i=0; i<r1; i++)
        vert_in[i] = 0;
    for(i=0; i<r2; i++)
        vert_out[i] = 0;
    for(i=0; i<r1*c2; i++)
        interim[i] = 0;
    
    
    for (r=0; r<r1; r++)
        ulCubicSpline(&in[r * c1], c1, &interim[r * c2], c2);
    
    
        /*printf("Interim\n");
        for (r=0; r<r1; r++)
        {
        for (c=0; c<c2; c++)		
        {
        printf("%g ", interim[r * c2 + c]);
        }
        printf("\n");
        }
    */
    
    
    for (c=0; c<c2; c++)
    {
        for(r=0; r<r1; r++)
            vert_in[r] = interim[r * c2 + c];  // transfer row to linear
        
        
        ulCubicSpline(vert_in, r1, vert_out, r2);
        
        for(r=0; r<r2; r++)
            result[r * c2 + c] = vert_out[r];
        
    }
    
    
    
    SAFE_ARRAY_DELETE(vert_in);
    SAFE_ARRAY_DELETE(vert_out);
    SAFE_ARRAY_DELETE(interim);
    
}
