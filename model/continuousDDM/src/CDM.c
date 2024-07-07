#include <errno.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <R.h>
#include <Rmath.h>


int CDM(int *N, double *threshold, double *theta_z,
        double *zMag, double *theta_v, double *vScale,
        double *out_x, double *out_y, double *out_rt) {
  // Output
  // out_x, out_y, out_rt
  // [Response(x pos), Response(y pos), RT (in seconds)]
  //
  // Params:
  // -------
  // threshold: threshold 
  // zDir: direction of starting point (phase angle)
  // zMag: magnitude of starting point 
  // v: drift rate (phase angle)
  // vScale: scaling of drift 
  // 
  // Returns:
  // --------
  // x: updated states
  
  // Declare variables
  // iterations
  int i;
  int iN = *N;
  
  // parameters
  double sigmaS = 1; // within trial noise
  double stepSize = 0.01; // step size (seconds)
  int nSteps; // number of steps
  // starting point position, drift direction, evidence state
  double z_x, z_y, drift_x, drift_y, pos_x, pos_y; 
  
  // starting point position
  z_x = cos(*theta_z); 
  z_y = sin(*theta_z);
  // drift direction
  drift_x = cos(*theta_v); 
  drift_y = sin(*theta_v); 
  
  // Get R RND state
  GetRNGstate();
  
  for (i=0; i<(iN); i++){ // simulation iteration
    nSteps = 0;
    pos_x = z_x*(*zMag)*(*threshold); 
    pos_y = z_y*(*zMag)*(*threshold); 
    while ((pow(pos_x,2)+pow(pos_y,2)) < pow((*threshold),2)) {
      nSteps = nSteps + 1;
      pos_x = pos_x + (*vScale)*drift_x*stepSize + rnorm(0,sigmaS)*sqrt(stepSize); 
      pos_y = pos_y + (*vScale)*drift_y*stepSize + rnorm(0,sigmaS)*sqrt(stepSize); 
      if (pos_x<0) {pos_x=0;}
      if (pos_y<0) {pos_y=0;}
    }
    out_x[i]= pos_x;
    out_y[i]= pos_y;
    out_rt[i] = nSteps * stepSize;
  }
  
  // return RNG
  PutRNGstate();
  
}


