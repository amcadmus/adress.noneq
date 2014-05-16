#ifndef __AutoCorrelCalculator_h_wanghan__
#define __AutoCorrelCalculator_h_wanghan__

#include "PieceTraj.h"
#include "BlockAverage.h"

class AutoCorrelCalculator 
{
  vector<BlockAverage_acc> bas;
  PieceTraj<double > traj;
public:
  AutoCorrelCalculator ();
  AutoCorrelCalculator (const unsigned & ndata,
			const unsigned nDataInBlock = 1);
public:
  void reinit (const unsigned & ndata,
	       const unsigned nDataInBlock = 1);
  void push_back (const double & value);
  void calculate ();
public:
  unsigned nData () const {return bas.size();}
  double value (const unsigned & idx) const {return bas[idx].getAvg();}
  double error (const unsigned & idx) const {return bas[idx].getAvgError();}
  const BlockAverage_acc & getBlockAverage (const unsigned & ii) const {return bas[ii];}
}
    ;

#endif
