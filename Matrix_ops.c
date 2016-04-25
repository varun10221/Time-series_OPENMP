#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>



#define FROM_MASTER 1
#define FROM_WORKER 2
#define N_ROWS 32 
#define N_COL 32
#define root 0

MPI_Status status;


/* Define globally the matrices */
int **A, **B, **BT, **C, **CT;      /* A,B will be input matrices,
                                       BT will be B transpose
                                       C will be result */

int **A_row, **B_col;


int **
Matrix_Alloc (int m, int n)
{
     int **X;
     X = (int **) calloc (m , sizeof (int *));
     X[0] = (int *) calloc (sizeof (int) , m * n);
      int i;
      for (i = 0 ; i < m; i++)
        X[i] =  (*X + n * i);

     return X;
}


void
Matrix_init (int **X, int m , int n)
{
  int i, j;
  for (i= 0; i < m; i++)
    {
      for (j=0; j < n; j ++)
         {
            X[i][j] = rand () % 10;
         }
    }

}

int **
Transpose (int **X, int m, int n) 
{

   int **Y;
   Y = Matrix_Alloc (m,n);
   int i,j;
   for (i = 0; i < m; i++)
     for (j = 0; j < n; j++)
       {
          Y[i][j] = X[j][i];   
       }

    return Y;
}


void
Verify (int **X, int **Y, int **Z, int m)
{
  int **Q;
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

   int sum = 0;
   for (j = 0; j <m; j++)
     {
       for (k=0; k<m; k++)
          sum  = Q[j][k] - Z[j][k];
     }

       printf ("%d", Z[N_ROWS -1 ][N_COL - 1]);
}
