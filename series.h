#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/* The time series consists of a data array and a corresponding count element */
struct Timeseries
{
  float *data;
  int count;
};



/* API's that help in time-series */
bool copy_series (struct Timeseries *, FILE *, int);
bool allocate_series (struct Timeseries *, int);


 







