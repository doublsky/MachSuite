#include <stdio.h>
#include <stdlib.h>
#include "support.h"
#include "HLS/hls.h"

//Define input sizes
#define col_size 64
#define row_size 128
#define f_size 9

//Data Bounds
#define TYPE int32_t
#define MAX 1000
#define MIN 1

//Set number of iterations to execute
#define MAX_ITERATION 1

struct filter_t {
    TYPE data[f_size];
};

component void stencil( 
        hls_avalon_slave_memory_argument(row_size * col_size * sizeof(TYPE)) TYPE *orig,
        hls_avalon_slave_memory_argument(row_size * col_size * sizeof(TYPE)) TYPE *sol,
        filter_t filter );

////////////////////////////////////////////////////////////////////////////////
// Test harness interface code.

struct bench_args_t {
    TYPE orig[row_size*col_size];
    TYPE sol[row_size*col_size];
    filter_t filter;
};
