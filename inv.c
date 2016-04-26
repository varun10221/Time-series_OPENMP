#include <stdio.h>
#include <math.h>
#include "Matrix_ops.h"

int main()
{
  float **a, k, d;
  int i, j;
  printf("Enter the order of the Matrix : ");
  scanf("%f", &k);
  
  a = Matrix_Alloc (k, k);
  printf("Enter the elements of %.0fX%.0f Matrix : \n", k, k);
  
  Matrix_init (a, k, k);
  print_Mat (a, k, k);
 
  d = determinant(a, k);
  if (d == 0)
    printf("\nInverse of Entered Matrix is not possible\n");
  else
    {
     float **b;
     b = cofactor(a, k);
     printf ("Inverse: \n");
     print_Mat (b, (int) k, (int) k);
    }

return 0;

}
 


