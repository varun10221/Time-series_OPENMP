#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "Matrix_ops.h"
#include "pseries.h"

/* Allocates a 2-d matrix with m rows and n-cols of type 'float' */

float **
Matrix_Alloc (int m, int n)
{
     float **X;
     X = (float **) calloc (m , sizeof (float *));
     X[0] = (float *) calloc (sizeof (float) , m * n);
      int i;
      for (i = 1 ; i < m; i++)
        X[i] =  (*X + n * i);

     return X;
}


/* Initialises a random matrix */
void
Matrix_init (float **X, int m , int n)
{
  int i, j;
  for (i= 0; i < m; i++)
    {
      for (j=0; j < n; j ++)
         {
            X[i][j] = (float) (rand () % 10);
         }
    }

}


/* Returns a pointer to the transpose of the input 
   matrix obtained */
float **
Transpose (float **X, int m, int n) 
{

   float **Y;
   Y = Matrix_Alloc (m,n);
   int i,j;
   for (i = 0; i < m; i++)
     for (j = 0; j < n; j++)
       {
          Y[i][j] = X[j][i];   
       }

    return Y;
}


/* Multiplies the matrix X and Y and compares it against
   the matrix Z, a succesful verification will result in 
   a value of zero sum */

void
Verify (float **X, float **Y, float **Z, int m)
{
  float **Q;
  Q = Matrix_Alloc (m,m); 
 
  int i,j,k;

   for (i=0; i<m; i++)
    {
     for (j = 0; j <m; j++)
       {
         for (k=0; k<m; k++)
           Q[i][j] = Q[i][j] + X[i][k]*Y[k][j];
       }

    }

   float sum = 0.0;
   for (j = 0; j <m; j++)
     {
       for (k=0; k<m; k++)
          sum  = Q[j][k] - Z[j][k];
     }

   printf ("sum: %f",sum);

}


float **
Matrix_Mul (float **X, float **Y, int m , int n, int q)
{
  float **Q;
  Q = Matrix_Alloc (m,m);

  int i,j,k;
#pragma omp parallel shared(Q,X,Y) private(i,j,k)
{
 #pragma omp for schedule (static)
   for (i=0; i<m; i++)
    {
     for (j = 0; j <n; j++)
       {
         for (k=0; k<q; k++)
           Q[i][j] = Q[i][j] + X[i][k]*Y[k][j];
       }

    }
}
  return Q;

}


void
print_Mat (float **X, int m, int n)
{

   int i,j;

   for (i=0; i<m; i++)
    {
     for (j = 0; j <n; j++)
       {
         printf ("%f\t",X[i][j]);
       }
     printf ("\n");
    }
}

float **
cholesky (float **A, int n) {
    float **L = Matrix_Alloc (n, n);
    if (L == NULL)
        exit(EXIT_FAILURE);

    for (int i = 0; i < n; i++)
       {
        for (int j = 0; j < (i+1); j++)
           {
            double s = 0;
            for (int k = 0; k < j; k++)
              {
                s += L[i][k] * L[j][k];
              }
             L[i][j] = (i == j) ? sqrt(A[i][i] - s) :
                           (1.0 / L[j][j] * (A[i][j] - s));
            }
       }
    return L;
}






/*For calculating Determinant of the Matrix 
  Make sure the values are allocated */
float determinant(float **a, float k)
{
  float s = 1, det = 0, **b;
  b = Matrix_Alloc (k,k);

  int i, j, m, n, c;
  if (k == 1)
    {
     return (a[0][0]);
    }
  else
    {
     det = 0;
     for (c = 0; c < k; c++)
       {
        m = 0;
        n = 0;
        for (i = 0;i < k; i++)
          {
            for (j = 0 ;j < k; j++)
              {
                b[i][j] = 0;
                if (i != 0 && j != c)
                 {
                   b[m][n] = a[i][j];
                   if (n < (k - 2))
                    n++;
                   else
                    {
                     n = 0;
                     m++;
                     }
                   }
               }
            }
          det = det + s * (a[0][c] * determinant(b, k - 1));
          s = -1 * s;
          }
    }
 
    return (det);
}


/* Find the co-factor Matrix and gives the inverse pre-division by a determinant */
float ** 
cofactor(float **num, float f)
{
  int k  = (int) f;

  float **b, **fac;

  b = Matrix_Alloc (k, k);
  fac = Matrix_Alloc (k , k);

  int p, q, m, n, i, j;
  for (q = 0; q < f; q++)
  {
   for (p = 0; p < f; p++)
    {
     m = 0;
     n = 0;
     for (i = 0;i < f; i++)
     {
       for (j = 0;j < f; j++)
        {
          if (i != q && j != p)
          {
            b[m][n] = num[i][j];
            if (n < (f - 2))
             n++;
            else
             {
               n = 0;
               m++;
             }
          }
        }
      }
      fac[q][p] = pow(-1, q + p) * determinant(b, f - 1);
    }
  }
  
  float **T;
  T = Transpose(fac, k, k);
  
  float d = determinant (num,f);

  float **inverse = Matrix_Alloc (k,k);
  for (i = 0;i < f; i++)
    {
     for (j = 0;j < f; j++)
       {
        inverse[i][j] = T[i][j] / d;
       }
    }

  return inverse;
  
}

