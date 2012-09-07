#ifndef __WaveCalculator_h_wanghan__
#define __WaveCalculator_h_wanghan__

#include <vector>
#include "Defines.h"
#include <fstream>

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
  FILE * fp;
  float time;
public:
  DensityWave ();
  ~DensityWave ();
  void reinit (const double & bin,
	       const VectorType & box);
  void clear ();
  void calculate (const float & time,
		  const vector<vector<double > > & com,
		  const vector<double > & mass);
public:
  void initFile (const char * fname);
  void write () const;
}
    ;



#endif
