#include "WaveCalculator.h"
#include <cmath>
#include <iostream>
#include <stdlib.h>

using namespace std;

DensityWave::
DensityWave ()
    : fp(NULL)
{
}

DensityWave::
~DensityWave ()
{
  if (fp != NULL){
    fclose (fp);
  }
}

void DensityWave::
initFile (const char * fname)
{
  fp = fopen (fname, "w");
  if (fp == NULL){
    cerr << "cannot open file " << fname << endl;
    exit (2);
  }
  size_t wsize =  rgrid.size();
  float * rr = (float *) malloc (sizeof(float) * wsize);
  for (unsigned ii = 0; ii < unsigned(wsize); ++ii){
    rr[ii] = rgrid[ii];
  }
  fwrite (&wsize, sizeof(size_t), 1, fp);
  fwrite (rr, sizeof(float), wsize, fp);
  free (rr);
}

void DensityWave::
write () const
{
  size_t wsize =  rgrid.size();
  float * vv = (float *) malloc (sizeof(float) * wsize);
  for (unsigned ii = 0; ii < unsigned(wsize); ++ii){
    vv[ii] = value[ii];
  }
  fwrite (&time, sizeof(float), 1, fp);
  fwrite (vv, sizeof(float), wsize, fp);
  free (vv);
}


void DensityWave::
reinit (const double & bin_,
	const VectorType & box)
{
  rup = box.x;
  if (rup < box.y) rup = box.y;
  if (rup < box.z) rup = box.z;
  rup *= 0.5;
  
  nbins = int(rup / bin_);
  // bin = rup / nbins;
  bin = bin_;
  rup = bin * nbins;
  cout << "bin size is " << bin << endl;
  cout << "r up is " << rup << endl;
  cout << "number of bins is " << nbins << endl;

  boxCenter = box;
  boxCenter.x *= 0.5;
  boxCenter.y *= 0.5;
  boxCenter.z *= 0.5;
  
  rgrid.resize (nbins);
  volume.resize (nbins);
  value.resize (nbins, 0.);

  for (int ii = 0; ii < nbins; ++ii){
    rgrid[ii] = (ii) * bin;
    double tmpr = (ii+1) * bin;
    volume[ii] = 4./3. * M_PI * (tmpr * tmpr * tmpr - 
				 rgrid[ii] * rgrid[ii] * rgrid[ii]);
  }
  clear ();
}

void DensityWave::
clear ()
{
  for (int ii = 0; ii < nbins; ++ii){
    value[ii] = 0.;
  }
}

void DensityWave::
calculate (const float & time_,
	   const vector<vector<double > > & com,
	   const vector<double > & mass)
{
  time = time_;
  for (unsigned ii = 0; ii < com.size(); ++ii){
    VectorType diff;
    diff.x = com[ii][0] - boxCenter.x;
    diff.y = com[ii][1] - boxCenter.y;
    diff.z = com[ii][2] - boxCenter.z;
    double rr = sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
    int idx = int(rr / bin);
    if (idx < nbins) {
      value[idx] += mass[ii] / volume[idx];
    }
  }
}





MomentumWave::
MomentumWave ()
    : fp(NULL)
{
}

MomentumWave::
~MomentumWave ()
{
  if (fp != NULL){
    fclose (fp);
  }
}

void MomentumWave::
initFile (const char * fname)
{
  fp = fopen (fname, "w");
  if (fp == NULL){
    cerr << "cannot open file " << fname << endl;
    exit (2);
  }
  size_t wsize =  rgrid.size();
  float * rr = (float *) malloc (sizeof(float) * wsize);
  for (unsigned ii = 0; ii < unsigned(wsize); ++ii){
    rr[ii] = rgrid[ii];
  }
  fwrite (&wsize, sizeof(size_t), 1, fp);
  fwrite (rr, sizeof(float), wsize, fp);
  free (rr);
}

void MomentumWave::
write () const
{
  size_t wsize =  rgrid.size();
  float * vv = (float *) malloc (sizeof(float) * wsize);
  for (unsigned ii = 0; ii < unsigned(wsize); ++ii){
    vv[ii] = value[ii];
  }
  fwrite (&time, sizeof(float), 1, fp);
  fwrite (vv, sizeof(float), wsize, fp);
  free (vv);
}


void MomentumWave::
reinit (const double & bin_,
	const VectorType & box)
{
  rup = box.x;
  if (rup < box.y) rup = box.y;
  if (rup < box.z) rup = box.z;
  rup *= 0.5;
  
  nbins = int(rup / bin_);
  // bin = rup / nbins;
  bin = bin_;
  rup = bin * nbins;
  cout << "bin size is " << bin << endl;
  cout << "r up is " << rup << endl;
  cout << "number of bins is " << nbins << endl;

  boxCenter = box;
  boxCenter.x *= 0.5;
  boxCenter.y *= 0.5;
  boxCenter.z *= 0.5;
  
  rgrid.resize (nbins);
  volume.resize (nbins);
  value.resize (nbins, 0.);

  for (int ii = 0; ii < nbins; ++ii){
    rgrid[ii] = (ii) * bin;
    double tmpr = (ii+1) * bin;
    volume[ii] = 4./3. * M_PI * (tmpr * tmpr * tmpr - 
				 rgrid[ii] * rgrid[ii] * rgrid[ii]);
  }
  clear ();
}

void MomentumWave::
clear ()
{
  for (int ii = 0; ii < nbins; ++ii){
    value[ii] = 0.;
  }
}

void MomentumWave::
calculate (const float & time_,
	   const vector<vector<double > > & com,
	   const vector<double > & moment_r)
{
  time = time_;
  for (unsigned ii = 0; ii < com.size(); ++ii){
    VectorType diff;
    diff.x = com[ii][0] - boxCenter.x;
    diff.y = com[ii][1] - boxCenter.y;
    diff.z = com[ii][2] - boxCenter.z;
    double rr = sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
    int idx = int(rr / bin);
    if (idx < nbins) {
      value[idx] += moment_r[ii] / volume[idx];
    }
  }
}
