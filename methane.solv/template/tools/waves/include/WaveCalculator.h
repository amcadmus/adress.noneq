#ifndef __WaveCalculator_h_wanghan__
#define __WaveCalculator_h_wanghan__

#include <vector>
#include "Defines.h"

using namespace std;

class DensityWave
{
  double bin;
  double rup;
  int nbins;
  VectorType boxCenter;
  vector<double > rgrid;
  vector<double > volume;
  vector<double > value;
public:
  void reinit (const double & bin,
	       const VectorType & box);
  void clear ();
  void calculate (const vector<vector<double > > & com,
		  const vector<double > & mass);
}
    ;



#endif
