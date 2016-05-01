#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define ERROR_CORR -2
#define varian 1


/* The time series consists of a data array and a corresponding count element */
struct Timeseries
{
  float *data;
  int count;
};


struct Autoarray
{
   float *auto_data;
   int lag_count;
};

/* API's that help in time-series */
bool copy_series (struct Timeseries *, FILE *, int);
bool allocate_series (struct Timeseries *, int);
float mean (struct Timeseries *);
float variance (struct Timeseries *, float mean);
float *Moving_average_filter (struct Timeseries *, int window); 
float auto_correlate (struct Timeseries *, int);
float pacf_func (struct Autoarray *, float **, int , int);
double a (void);
void print_timearray (double a[10][2]);



