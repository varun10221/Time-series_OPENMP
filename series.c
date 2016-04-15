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

 return sum;

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
    
  











