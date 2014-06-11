#ifndef __Floque_h_wanghan__
#define __Floque_h_wanghan__

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <queue>
#include <vector>
#include <cmath>
#include <complex> 

using namespace std;

class Floque 
{
  double period;
  int nstate;
  int nsize;
  int nmode;
  int nshift;
  int ffnsize;
  int ffnmode;
  int ffnshift;
  int floqueDOF;
  // ffnsize * nstate * nstate
  vector<vector<vector<complex<double > > > > ffTransitionMatrix;
  // floqueDOF * floqueDOF
  vector<vector<complex<double > > > floqueMatrix;
  void naiveFourier (const vector<double > & in,
		     const int & nn,
		     complex<double > & out);
public:
  void fourierTransitionMatrix (const vector<vector<vector<double > > > & in);
  void checkFourierTransitionMatrix (const vector<vector<vector<double > > > & in);
public:
  Floque (const int & nsize,
	  const double & period);
public:
  void buildFloqueMatrix (const vector<vector<vector<double > > > & in);
  void convert2FloqueVector (const vector<double > & in,
			     vector<complex<double > > & out);
  void convert2NormalVector (const vector<complex<double > > & in,
			     const double & tt,
			     vector<complex<double > > & out);
  void applyFloqueMatrix (const vector<complex<double > > & in,
			  vector<complex<double > > & out);
public:
  void printFloqueMatrix (const string & filename);
  void printFloqueVector (const string & filename,
			  const vector<complex<double > > & vec );
  void printFfTransitionMatrix (const string & filename);
}
    ;



#endif
