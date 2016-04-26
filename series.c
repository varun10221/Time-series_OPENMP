#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "series.h"

#define N 16000

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
  for (i = 0; i < series->count; i++)
    sum + = series->data[i];

 return (sum/series->count);

}


float
var_mean (float *data, int start, int end)
{
   if (data == NULL)
      return 0;
   
   int i;
   float sum - 0.0;

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
 
  for (i = 0; i < series->count; i++)
    {
       var[i] = (series->data[i] - mean) * (series->data[i] - mean);
    }

  float sum = 0.0;
  for ( i = 0; i < series->count; i++)
    {
       sum += var[i];
    }

   return (sum / (series->count));

}

struct Auto_array *
Autocorrelation_array_gen (struct Timeseries *series, int lag)
{
    struct Auto_array *array;

    array = calloc (sizeof (struct Auto_array), 1);
     
    if (array == NULL)
      return NULL; 
   
    array->auto_data = calloc (sizeof (float), lag); 
    array->lag_count = lag; 

    for (i = 0; i < lag; i++)
      {
        array->auto_data [i] = auto_correlate (series, i);
        if (auto_data[i] >= ERROR_CORR)
          return NULL;
      }
 
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
   
   mean1 = var_mean (series->data, 0, series->count - lag -1);
   mean2 = var_mean (series->data, lag, series->count);

   double numerator = 0.0;
   for (i = 0; i < (series->count  - lag); i++)
     {
        numerator += (series->data[i] - mean1) + (series->data[i + lag]
                                                              - mean2);
     }
 
   double d1 = 0.0;
   double d2 = 0.0; 
        
   double temp1, temp2;

   for (i = 0; i  < (series->count - lag); i++)
     {
       
         temp1 = (series->data[i] - mean1);
         temp2 = (series->data[i + lag] - mean2);
         
         d1 = d1 + (temp1 * temp1);
         d2 = d2 + (temp2 * temp2);
     }

   double denominator = sqrt (d1) * sqrt (d2);

 
   return (float) (numerator/ denominator);  
} 

float
pacf (struct Timeseries *series, int lag)
{


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

  printf ("success");

  return 1;
}
    
  











