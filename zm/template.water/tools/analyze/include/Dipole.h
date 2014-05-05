#ifndef __Dipole_h_wanghan__
#define __Dipole_h_wanghan__

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

#include "Defines.h"
#include "CellList.h"
#include "BlockAverage.h"

class KirkwoodFactor
{
  int nbins;
  ValueType rup;
  ValueType binSize;
  ValueType offset;
  int nframe;
  std::vector<BlockAverage_acc > hist;
public:
  void reinit		 (const ValueType rup_,
			  const ValueType refh_,
			  const unsigned nDataBlock = 1);
  void deposit		 (const std::vector<std::vector<ValueType> > & coord,
			  const CellList & clist,
			  const std::vector<std::vector<ValueType> > & dipole,
			  const VectorType & box);
  void calculate	 ();
  double getAvg		 (const unsigned & ii) const;
  double getAvgError	 (const unsigned & ii) const;
}
    ;



#endif
