#include "md.h"

#define MIN(x,y) ( (x)<(y) ? (x) : (y) )
#define MAX(x,y) ( (x)>(y) ? (x) : (y) )

component void md( 
  hls_avalon_slave_memory_argument(blockSide * blockSide * blockSide * sizeof(int32_t)) int32_t *n_points,
  hls_avalon_slave_memory_argument(blockSide * blockSide * blockSide * densityFactor * sizeof(dvector_t)) dvector_t *force,
  hls_avalon_slave_memory_argument(blockSide * blockSide * blockSide * densityFactor * sizeof(dvector_t)) dvector_t *position
)
{
  ivector_t b0, b1; // b0 is the current block, b1 is b0 or a neighboring block
  dvector_t p, q; // p is a point in b0, q is a point in either b0 or b1
  int8_t p_idx, q_idx;  // max of idx is densityFactor (10)
  TYPE dx, dy, dz, r2inv, r6inv, potential, f;

  // Iterate over the grid, block by block
  loop_grid0_x: for( b0.x=0; b0.x<blockSide; b0.x++ ) {
  loop_grid0_y: for( b0.y=0; b0.y<blockSide; b0.y++ ) {
  loop_grid0_z: for( b0.z=0; b0.z<blockSide; b0.z++ ) {
    // For all points in b0
    for( p_idx=0; p_idx<n_points[IDX3D(b0.x,b0.y,b0.z)]; p_idx++ ) {
      p = position[IDX4D(b0.x,b0.y,b0.z,p_idx)];
      TYPE sum_x = 0;
      TYPE sum_y = 0;
      TYPE sum_z = 0;
      // Iterate over the 3x3x3 (modulo boundary conditions) cube of blocks around b0
      loop_grid1_x: for( b1.x=MAX(0,b0.x-1); b1.x<MIN(blockSide,b0.x+2); b1.x++ ) {
      loop_grid1_y: for( b1.y=MAX(0,b0.y-1); b1.y<MIN(blockSide,b0.y+2); b1.y++ ) {
      loop_grid1_z: for( b1.z=MAX(0,b0.z-1); b1.z<MIN(blockSide,b0.z+2); b1.z++ ) {
        TYPE inner_sum_x = 0;
        TYPE inner_sum_y = 0;
        TYPE inner_sum_z = 0;
        // For all points in b1
        #pragma unroll
        for( q_idx=0; q_idx< densityFactor; q_idx++ ) {
          if (q_idx < n_points[IDX3D(b1.x,b1.y,b1.z)]) {
            q = position[IDX4D(b1.x,b1.y,b1.z,q_idx)];
  
            // Don't compute our own
            if( q.x!=p.x || q.y!=p.y || q.z!=p.z ) {
              // Compute the LJ-potential
              dx = p.x - q.x;
              dy = p.y - q.y;
              dz = p.z - q.z;
              r2inv = 1.0/( dx*dx + dy*dy + dz*dz );
              r6inv = r2inv*r2inv*r2inv;
              potential = r6inv*(lj1*r6inv - lj2);
              // Update forces
              f = r2inv*potential;
              inner_sum_x += f*dx;
              inner_sum_y += f*dy;
              inner_sum_z += f*dz;
            }
          }
        } // loop_q
        sum_x += inner_sum_x;
        sum_y += inner_sum_y;
        sum_z += inner_sum_z;
      }}} // loop_grid1_*
      force[IDX4D(b0.x,b0.y,b0.z,p_idx)].x = sum_x ;
      force[IDX4D(b0.x,b0.y,b0.z,p_idx)].y = sum_y ;
      force[IDX4D(b0.x,b0.y,b0.z,p_idx)].z = sum_z ;
    } // loop_p
  }}} // loop_grid0_*
}
