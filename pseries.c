#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pseries.h"
#include "Matrix_ops.h"
#include <math.h>
#include "omp.h"

#define N 1600

double time[10][2];


bool
allocate_series (struct Timeseries * series, int count)
{
   if (series == NULL)
     {
       printf ("Bad ptr");
       return false;
     }
 
   series->data = malloc (sizeof (float) * count);
   series->count = count;
 
    return true;

}

double
a (void)
{
  return omp_get_wtime ();
}

bool
copy_series (struct Timeseries *series, FILE *fp , int count)
{
   int i;
   
   if (!series || !fp)
      return false;

   for ( i = 0; i < count; i++)
     fscanf (fp, "%f", &series->data[i]);

   fclose (fp);

   return true;

}  


float
mean (struct Timeseries *series)
{
     
  if (series == NULL) 
     return 0;
 
  int i;
  float sum = 0.0;
  time[0][0] = a ();
#pragma omp parallel for reduction(+: sum)
   for (i = 0; i < series->count; i++)
      sum += series->data[i];
  
  time[0][1] = a();
  //printf ("timehello :%f", end - start);
  return (sum/series->count);

}


float
var_mean (float *data, int start, int end)
{
   if (data == NULL)
      return 0;
   
   int i;
   float sum = 0.0;

#pragma omp parallel for reduction(+: sum)
   for (i = start; i <= end; i++)
      sum += data[i];
 
   return (sum / (end - start));

}

float
variance (struct Timeseries *series, float mean)
{
  float *var;
  
  if (series == NULL)
      return -1.0;
    
  var = malloc (sizeof (float) * series->count);
  
  int i;

  time [varian][0] = a();
#pragma omp parallel for private (i)
  for (i = 0; i < series->count; i++)
    {
       var[i] = (series->data[i] - mean) * (series->data[i] - mean);
    }

  float sum = 0.0;
#pragma omp parallel for reduction(+: sum)
  for ( i = 0; i < series->count; i++)
    {
       sum += var[i];
    }

   time [varian][1] = a();
   return (sum / (series->count));

}

struct Autoarray *
Autocorrelation_array_gen (struct Timeseries *series, int lag)
{
    struct Autoarray *array;

    array = calloc (sizeof (struct Autoarray), 1);
     
    if (array == NULL)
      return NULL; 
   
    array->auto_data = calloc (sizeof (float), lag); 
    array->lag_count = lag; 
 
    time[2][0] = a ();
    int i;
#pragma omp parallel for private (i)
    for (i = 0; i < lag; i++)
      {
        array->auto_data [i] = auto_correlate (series, i);
      }
 
    time [2][1] = a();
    return array;  
}

float
auto_correlate (struct Timeseries *series, int lag)
{
   if (series == NULL || series->data == NULL)
     return  ERROR_CORR;

   int i,j;
   i = 0;
   j = lag;
  
   float mean1, mean2;
   
   time [3][0] = a ();
   mean1 = var_mean (series->data, 0, series->count - lag -1);
   mean2 = var_mean (series->data, lag, series->count);

   double numerator = 0.0;
 
#pragma omp parallel for reduction (+: numerator)
   for (i = 0; i < (series->count  - lag); i++)
     {
        numerator += (series->data[i] - mean1) * (series->data[i + lag]
                                                              - mean2);
     }

   double d1 = 0.0;
   double d2 = 0.0; 
        
   double temp1, temp2;


#pragma omp parallel for private (temp1, temp2) reduction (+: d1,d2)
   for (i = 0; i  < (series->count - lag); i++)
     {
       
         temp1 = (series->data[i] - mean1);
         temp2 = (series->data[i + lag] - mean2);
         
         d1 = d1 + (temp1 * temp1);
         d2 = d2 + (temp2 * temp2);
     }

   double denominator = pow (d1, 0.5) * pow (d2, 0.5);

 
    time [3][1] = a();
   return (float) (numerator/ denominator);  
} 

float**
pacf_array (struct Autoarray *acf, int lag)
{

  float **pacf;
  pacf = Matrix_Alloc (lag +1, lag+1);
 
  int i,j;
  time [5][0] = a();
  pacf [0][0] = acf->auto_data [0];
  for (i = 1; i <= lag; i++)
   { 
     #pragma omp parallel
      {
      #pragma omp single
        {
         #pragma omp task
          {
           for (j = 1; j <= i; j++)
            {
              pacf[i][j] = pacf_func (acf,pacf, i, j);
            }
          #pragma omp taskwait
          }
        }
      }  
   }
  
   time [5][1] = a();
   return pacf; 

}


float
pacf_func (struct Autoarray *acf, float **pacf, int k1, int k2)
{

  float n_sum = 0.0, d_sum = 0.0;
  int i,j;
  if (k1 == k2)
   {
    time [4][0] += a();
    #pragma omp parallel for reduction (+:n_sum,d_sum)
     for (i = 0; i < k1; i++)
       {
          n_sum = n_sum + (pacf[k1-1][i] * acf->auto_data [k1 - i]);
          d_sum = d_sum + (pacf[k1 -1][i] * acf->auto_data [i]);
       } 
   
     float result;
 
     if (d_sum != 1)   
        result = (acf->auto_data [k1] - n_sum)/ (1 - d_sum);

     else result = (acf->auto_data [k1] - n_sum);

     time [4][1] += a();
     return result;

   }


  else {
      float result;
      time [6][0] += a();
      #pragma omp task private (result)
      result = pacf[k1 - 1][k2] -(pacf_func (acf, pacf, k1, k1) 
                                        * pacf[k1-1][k1-k2]);
      #pragma omp taskwait
      time [6][1] += a();
      return result;
       }
}



float *
Moving_average_filter (struct Timeseries *series, int window)
{

   if (series == NULL)

      return NULL;

   float *MA_data;
   MA_data = calloc (sizeof (float), series->count);
  
   if (MA_data == NULL)
     return NULL;

   int i;
   for (i = 0; i < (series->count) - window; i++)
     {
        MA_data[i + window] = var_mean (series->data, i, i + window);
     }

   return MA_data;
}
 

void
print_timearray (double k[10][2])
{
   double diff;
   int i;
   for (i = 0; i < 10; i++)
    {
       diff = k[i][1] - k[i][0];
       printf(" time %d: %f \n", i, diff); 
    }
}


print_array (float *a, int count)
{
  int i;
  
  for (i =0; i < count; i++)
   {
      printf ("autoarray [%d] %f\n",i,a[i]);
   }
}


int
main (int argc, char **argv)
{

  struct Timeseries *series;
  FILE *fp;
 

  fp = fopen ("close.txt","r");
  
  series = (struct Timeseries *) malloc (sizeof (struct Timeseries));
  
  if (!allocate_series (series, N))
     return 0;
 
  if (!copy_series (series, fp, N))
     return 0;

  time [7][0] = a();
  printf ("mean :%f", mean (series));
  printf ("\nvariance :%f",variance (series, mean (series)));
  float **z;
  struct Autoarray *b;
  b = Autocorrelation_array_gen (series, 100);
  float **pf;
  pf = pacf_array (b, 10);
  
  float **A, **B;
  A = Matrix_Alloc (N/16,N/16);
  B = Matrix_Alloc (N/16, N/16);

  Matrix_init (A, N/16, N/16);
  Matrix_init (B, N/16, N/16);

  float **C = Matrix_Mul (A, B, N/16, N/16, N/16);
  time[9][0] = a();
#pragma omp parallel
{
  #pragma omp single
   {
    determinant (A, (float) (N/16));
   }
}
  time[9][1] = a();
 
  time [7][1] = a(); 
  print_timearray (time);

  return 1;
}
    
  











