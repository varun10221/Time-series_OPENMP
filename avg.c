#include "omp.h"
#include <stdio.h>

void
init (float *a , int n)
{
     int i;
     for ( i =0; i < n; i++)
         a[i] = (float) i;

}


int
main ()
{


   float a[100000];
   init (a, 1000);

   float sum = 0.0;
   int i;
   double start,end;
   start = omp_get_wtime ();
#pragma omp parallel for reduction (+:sum)
   for (i= 0; i < 1000; i++)
       sum = sum + a[i];

   end = omp_get_wtime ();

   printf ("time :%f\t%f",(end - start),sum);
   printf("\n%d",omp_get_num_threads ());
   return 0;
} 
   


