#include "WaveCalculator.h"
#include <cmath>

void DensityWave::
reinit (const double & bin_,
	const VectorType & box)
{
  rup = box.x;
  if (rup < box.y) rup = box.y;
  if (rup < box.z) rup = box.z;
  rup *= 0.5;
  
  nbins = int(rup / bin_);
  bin = rup / nbins;

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
calculate (const vector<vector<double > > & com,
	   const vector<double > & mass)
{
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


