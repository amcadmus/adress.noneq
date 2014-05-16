#include "AutoCorrelCalculator.h"

AutoCorrelCalculator::
AutoCorrelCalculator ()
{
}

AutoCorrelCalculator::
AutoCorrelCalculator (const unsigned & ndata,
		      const unsigned nDataInBlock)
    : bas (ndata), traj (ndata)
{
  for (unsigned ii = 0; ii < ndata; ++ii){
    bas[ii].reinit (nDataInBlock);
  }  
}

void AutoCorrelCalculator::
reinit (const unsigned & ndata,
	const unsigned nDataInBlock)
{
  traj.reinit (ndata);  
  bas.resize (ndata);
  for (unsigned ii = 0; ii < ndata; ++ii){
    bas[ii].reinit (nDataInBlock);
  }
}


void AutoCorrelCalculator::
push_back (const double & value)
{
  traj.push_back (value);

  int start = traj.tailPosi();
  double startValue = traj.getValue(start);
  
  for (int ii = 0; ii < traj.getNValid(); ++ii){
    int idx = traj.normalizedIdx (start - ii);
    bas[ii].deposite (traj.getValue(idx) * startValue);
  }

  // int end = traj.normalizedIdx (traj.tailPosi() - traj.getNValid());

  // int ii = start;
  // bas[0].deposite (traj.getValue(start) * traj.getValue(start));
  // --ii;
  // ii = traj.normalizedIdx (ii);
  // int count = 1;

  // while (ii != end){
  //   bas[count].deposite (traj.getValue(ii) * traj.getValue(start));
  //   --ii;
  //   ++count;
  //   ii = traj.normalizedIdx (ii);
  // }  
}

void AutoCorrelCalculator::
calculate () 
{
  for (unsigned ii = 0; ii < bas.size(); ++ii){
    bas[ii].calculate ();
  }
}



