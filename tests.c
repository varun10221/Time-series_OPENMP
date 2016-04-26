#include <stdio.h>
#include <cstdlib>
#include <stdbool.h>

float
Box-pierce (struct Timeseries *series, int N, int count)
{

  float sum = 0.0;
  
  struct Auto_array Auto_corr_struct = Auto_correlation_array_gen
                                                        (series, count + 1);
  float *Auto_corr_array = Auto_corr_struct->auto_data;
                                        

  if (Auto_corr_array == NULL)
     return -1;
 
  int i;
  
  for (i = 1; i <= count; i++)
    {
       sum = sum + pow (Auto_corr_array [i], 2);
    }

  return N * sum;
   
}




float
Ljung_box (float *Auto_corr_array, int N, int count)
{

 float sum = 0.0;
 struct Auto_array Auto_corr_struct = Auto_correlation_array_gen
                                                        (series, count + 1);
 float *Auto_corr_array = Auto_corr_struct->auto_data;


  if (Auto_corr_array == NULL)
     return -1;

  int i;

  for (i = 1; i <= count; i++)
    {
       sum = sum + (pow (Auto_corr_array [i], 2)/ (N - i));
    }

  return ((N * (N + 2)) * sum);

}






