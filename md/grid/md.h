#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "support.h"
#include "HLS/hls.h"

#define TYPE float

// Problem Constants
#define nAtoms        256
#define domainEdge    20.0
#define blockSide     4
//#define blockSide     1
#define nBlocks       (blockSide*blockSide*blockSide)
#define blockEdge     (domainEdge/((TYPE)blockSide))
// Memory Bound
// This is an artifact of using statically-allocated arrays. We'll pretend that
// it doesn't exist and instead track the actual number of points.
#define densityFactor 10
// LJ coefficients
#define lj1           1.5f
#define lj2           2.0f

#define IDX3D(x,y,z) ( x * blockSide * blockSide + y * blockSide + z)
#define IDX4D(x,y,z,w) ( x * blockSide * blockSide * densityFactor + y * blockSide * densityFactor + z * densityFactor + w)

typedef struct {
  TYPE x, y, z;
} dvector_t;
typedef struct {
  int8_t x, y, z;
} ivector_t;

component void md( 
         hls_avalon_slave_memory_argument(blockSide * blockSide * blockSide * sizeof(int32_t)) int32_t *n_points,
         hls_avalon_slave_memory_argument(blockSide * blockSide * blockSide * densityFactor * sizeof(dvector_t)) dvector_t *force,
         hls_avalon_slave_memory_argument(blockSide * blockSide * blockSide * densityFactor * sizeof(dvector_t)) dvector_t *position
       );
////////////////////////////////////////////////////////////////////////////////
// Test harness interface code.

struct bench_args_t {
  int32_t n_points[blockSide * blockSide * blockSide];
  dvector_t force[blockSide * blockSide * blockSide * densityFactor];
  dvector_t position[blockSide * blockSide * blockSide * densityFactor];
};
