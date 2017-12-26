#include <ul.h>
//#include <ulBig.h>

#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))


//static ulVector1 rv1;

real pythag(real a, real b)
{
    real absa, absb;
    absa = fabs(a);
    absb = fabs(b);
    if (absa > absb)
        //return absa * sqrt(1.0 + ulSqr(absb / absa));
        return absa * ulsqrt(1.0 + ulSqr(absb / absa));
    else
        //return (absb == 0.0 ? 0.0 : absb * sqrt(1.0 + ulSqr(absa / absb)));
        return (absb == 0.0 ? 0.0 : absb * ulsqrt(1.0 + ulSqr(absa / absb)));
}

void svbksb(ulMatrix1 & u, ulVector1 & w, ulMatrix1 & v, int m, int n, const ulVector1 & b, ulVector1 & x)
{
    int jj, j, i;
    real s;
    
    ulVector1 tmp(n);
    
    if (x.n < n)
        x.resize(n);
    
    for (j = 0; j < n; j++)
    {
        s = 0.0;
        if (w[j])
        {
            for (i = 0; i < m; i++)
                s += u(i, j) * b[i];
            s /= w[j];
        }
        tmp[j] = s;
    }
    
    for (j = 0; j < n; j++)
    {
        s = 0.0;
        for (jj = 0; jj < n; jj++)
            s += v(j, jj) * tmp[jj];
        x[j] = s;
    }
    
}


HRESULT svdcmp(ulMatrix1 & a, int m, int n, ulVector1 &w, ulMatrix1 & v)
{
    
    int flag, i, its, j, jj, k, l, nm;
    real anorm, c, f, g, h, s, scale, x, y, z;
    
    ulVector1 rv1(n);
    w.n = v.rows = v.cols = n;
    
    g = scale = anorm = 0.0;
    for (i = 0; i < n; i++)
    {
        l = i + 1;
        rv1[i] = scale * g;
        g = s = scale = 0.0;
        if (i <= m)
        {
            for (k = i; k < m; k++)
                scale += fabs(a(k, i));
            if (scale)
            {
                for (k = i; k < m; k++)
                {
                    a(k, i) /= scale;
                    s += a(k, i) * a(k, i);
                }
                f = a(i, i);
                //g = -SIGN(sqrt(s), f);
                g = -SIGN(ulsqrt(s), f);
                h = f * g - s;
                a(i, i) = f - g;
                for (j = l; j < n; j++)
                {
                    for (s = 0.0, k = i; k < m; k++)
                        s += a(k, i) * a(k, j);
                    f = s / h;
                    for (k = i; k < m; k++)
                        a(k, j) += f * a(k, i);
                }
                for (k = i; k < m; k++)
                    a(k, i) *= scale;
            }
        }
        w[i] = scale * g;
        g = s = scale = 0.0;
        if (i <= m && i != n)
        {
            for (k = l; k < n; k++)
                scale += fabs(a(i, k));
            if (scale)
            {
                for (k = l; k < n; k++)
                {
                    a(i, k) /= scale;
                    s += a(i, k) * a(i, k);
                }
                f = a(i, l);
                //g = -SIGN(sqrt(s), f);
                g = -SIGN(ulsqrt(s), f);
                h = f * g - s;
                a(i, l) = f - g;
                for (k = l; k < n; k++)
                    rv1[k] = a(i, k) / h;
                for (j = l; j < m; j++)
                {
                    for (s = 0.0, k = l; k < n; k++)
                        s += a(j, k) * a(i, k);
                    for (k = l; k < n; k++)
                        a(j, k) += s * rv1[k];
                }
                for (k = l; k < n; k++)
                    a(i, k) *= scale;
            }
        }
        anorm = ulMax(anorm, (real)(fabs(w[i]) + fabs(rv1[i])));
    }
    for (i = n-1; i >= 0; i--)
    {
        if (i < n)
        {
            if (g)
            {
                for (j = l; j < n; j++)
                    v(j, i) = (a(i, j) / a(i, l)) / g;
                for (j = l; j < n; j++)
                {
                    for (s = 0.0, k = l; k < n; k++)
                        s += a(i, k) * v(k, j);
                    for (k = l; k < n; k++)
                        v(k, j) += s * v(k, i);
                }
            }
            for (j = l; j < n; j++)
                v(i, j) = v(j, i) = 0.0;
        }
        v(i, i) = 1.0;
        g = rv1[i];
        l = i;
    }
    for (i = ulMin(m, n)-1; i >= 0; i--)
    {
        l = i + 1;
        g = w[i];
        for (j = l; j < n; j++)
            a(i, j) = 0.0;
        if (g)
        {
            g = 1.0 / g;
            for (j = l; j < n; j++)
            {
                for (s = 0.0, k = l; k < m; k++)
                    s += a(k, i) * a(k, j);
                f = (s / a(i, i)) * g;
                for (k = i; k < m; k++)
                    a(k, j) += f * a(k, i);
            }
            for (j = i; j < m; j++)
                a(j, i) *= g;
        }
        else
            for (j = i; j < m; j++)
                a(j, i) = 0.0;
            ++a(i, i);
    }
    for (k = n-1; k >= 0; k--)
    {
        for (its = 1; its <= 30; its++)
        {
            flag = 1;
            for (l = k; l >= 0; l--)
            {
                nm = l - 1;
                if ((real) (fabs(rv1[l]) + anorm) == anorm)
                {
                    flag = 0;
                    break;
                }
                if ((real) (fabs(w[nm]) + anorm) == anorm)
                    break;
            }
            if (flag)
            {
                c = 0.0;
                s = 1.0;
                for (i = l; i <= k; i++)
                {
                    f = s * rv1[i];
                    rv1[i] = c * rv1[i];
                    if ((real) (fabs(f) + anorm) == anorm)
                        break;
                    g = w[i];
                    h = pythag(f, g);
                    w[i] = h;
                    h = 1.0 / h;
                    c = g * h;
                    s = -f * h;
                    for (j = 0; j < m; j++)
                    {
                        y = a(j, nm);
                        z = a(j, i);
                        a(j, nm) = y * c + z * s;
                        a(j, i) = z * c - y * s;
                    }
                }
            }
            z = w[k];
            if (l == k)
            {
                if (z < 0.0)
                {
                    w[k] = -z;
                    for (j = 0; j < n; j++)
                        v(j, k) = -v(j, k);
                }
                break;
            }
            if (its == 30)
                return -1;
            //nrerror("no convergence in 30 svdcmp iterations");
            x = w[l];
            nm = k - 1;
            y = w[nm];
            g = rv1[nm];
            h = rv1[k];
            f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
            g = pythag(f, 1.0);
            f = ((x - z) * (x + z) + h * ((y / (f + SIGN(g, f))) - h)) / x;
            c = s = 1.0;
            for (j = l; j <= nm; j++)
            {
                i = j + 1;
                g = rv1[i];
                y = w[i];
                h = s * g;
                g = c * g;
                z = pythag(f, h);
                rv1[j] = z;
                c = f / z;
                s = h / z;
                f = x * c + g * s;
                g = g * c - x * s;
                h = y * s;
                y *= c;
                for (jj = 0; jj < n; jj++)
                {
                    x = v(jj, j);
                    z = v(jj, i);
                    v(jj, j) = x * c + z * s;
                    v(jj, i) = z * c - x * s;
                }
                z = pythag(f, h);
                w[j] = z;
                if (z)
                {
                    z = 1.0 / z;
                    c = f * z;
                    s = h * z;
                }
                f = c * g + s * y;
                x = c * y - s * g;
                for (jj = 0; jj < m; jj++)
                {
                    y = a(jj, j);
                    z = a(jj, i);
                    a(jj, j) = y * c + z * s;
                    a(jj, i) = z * c - y * s;
                }
            }
            rv1[l] = 0.0;
            rv1[k] = f;
            w[k] = x;
        }
    }
    return 0;
}

/*
    A * x = b
*/
HRESULT svd(ulMatrix1 & a, const ulVector1 & b, ulVector1 & x)
{
    int n, k;
    n = a.rows;

    ulVector1 w(n);
    ulVector1 c(n);
    ulMatrix1 u(n, n);
    ulMatrix1 v(n, n);
    ulMatrix1 tv(n, n);
    //ulMatrix1 test(n, n);
    //ulMatrix1 test3(n, n);
    
    real wmax, wmin;
    
    u = a;
    
    /* decompose matrix a */
    if (svdcmp(u, n, n, w, v) < 0)
    {
        return -1;
    }
    
    /*ulMatrix1 test2(n, n);
    test2.SetIdentity();
    
    for (int i = 0; i < n; i++)
        test2(i, i) = w[i];
    
    test = u * test2;
    tv = v.SetTranspose();
    test3 = test * tv; */
        
    /* find maximum singular value */
    if (n > 1)
        wmax = 0;
    
    wmax = 0.0;
    for (k = 0; k < n; k++)
    {
        if (w[k] > wmax)
            wmax = w[k];
    }
    
    /* define "small" */
    wmin = wmax * 1e-12;
    /* zero the "small" singular values */
    
    for (k = 0; k < n; k++)
    {
        if (w[k] < wmin)
            w[k] = 0.0;
    }
    
    
    svbksb(u, w, v, n, n, b, x);
    
    return 0;  
}





/*
In a, n, p, b
output x

*/

/*
void cholesky_solution(ulMatrix1 &a, int n, ulVector1 &p, ulVector1 & b, ulVector1 &x)
{
    int i,k;
    real sum;
    
    x.n = n;
    
    for (i=0;i<n;i++) 
    {
        sum = b[i];
        for (k=i-1; k>=0; k--) 
            sum -= a(i, k) * x[k];
        
        x[i]=sum/p[i];
    }
    for (i=n-1;i>=0;i--) 
    {
        sum=x[i];
        for (k=i+1;k<n;k++) 
            sum -= a(k, i)*x[k];
        
        x[i]=sum/p[i];
    }
}

/*
out p
*/
/*HRESULT cholesky_decomp(ulMatrix1 &a, int n, ulVector1 &p)
{
    int i,j,k;
    real sum;
    
    p.n = n;
    
    for (i=0;i<n;i++) 
    {
        for (j=i; j<n; j++) 
        {
            sum = a(i, j);
            for (k=i-1; k>=0; k--) 
                sum -= a(i, k) * a(j ,k);
            
            if (i == j) 
            {
                if (sum <= 0.0)
                    return -1;
                else
                    p[i]=sqrt(sum);
            } 
            else 
                a(j, i) = sum / p[i];
        }
    }

    return 0;
}    */


//ulMatrix1 ainv;
ulMatrix1 a;
//ulMatrix1 chol;
//ulVector1 checkb;

/*
A * x = b

  solve for x
*/
HRESULT Solve(const ulMatrix1 & aorig, const ulVector1 & b, ulVector1 & x)
{
    int n;
    
    
    n = aorig.rows;
    a = aorig;
    
    ulVector1 p(n);
    a = aorig;

    if (svd(a, b, x) < 0)
        return -1;
    
    return 0;
    
}
