#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "series.h"

#define N 16000

bool
allocate_series (struct Timeseries * series, int count)
{
   series = malloc (sizeof (struct Timseries));
   if (series == NULL)
     {
       printf ("Error allocating memory");
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

   





int
main (int argc, char **argv)
{

  struct Timeseries *series;
  FILE *fp;
 

  fp = fopen ("close.txt","r");
  
  
  











