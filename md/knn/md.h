/*
Implemenataion based on:
A. Danalis, G. Marin, C. McCurdy, J. S. Meredith, P. C. Roth, K. Spafford, V. Tipparaju, and J. S. Vetter.
The scalable heterogeneous computing (shoc) benchmark suite.
In Proceedings of the 3rd Workshop on General-Purpose Computation on Graphics Processing Units, 2010.
*/

#include <stdlib.h>
#include <stdio.h>
#include "support.h"
#include "HLS/hls.h"

#define TYPE float  // double is too costly on FPGA

// Problem Constants
#define nAtoms        256
#define maxNeighbors  16
// LJ coefficients
#define lj1           1.5f
#define lj2           2.0f

component void md_kernel(
    hls_avalon_slave_memory_argument(nAtoms * sizeof(TYPE)) TYPE * force_x,
    hls_avalon_slave_memory_argument(nAtoms * sizeof(TYPE)) TYPE * force_y,
    hls_avalon_slave_memory_argument(nAtoms * sizeof(TYPE)) TYPE * force_z,
    hls_avalon_slave_memory_argument(nAtoms * sizeof(TYPE)) TYPE * position_x,
    hls_avalon_slave_memory_argument(nAtoms * sizeof(TYPE)) TYPE * position_y,
    hls_avalon_slave_memory_argument(nAtoms * sizeof(TYPE)) TYPE * position_z,
    hls_avalon_slave_memory_argument(nAtoms * maxNeighbors * sizeof(int32_t)) int32_t * NL);
////////////////////////////////////////////////////////////////////////////////
// Test harness interface code.

struct bench_args_t {
  TYPE force_x[nAtoms];
  TYPE force_y[nAtoms];
  TYPE force_z[nAtoms];
  TYPE position_x[nAtoms];
  TYPE position_y[nAtoms];
  TYPE position_z[nAtoms];
  int32_t NL[nAtoms*maxNeighbors];
};
