#ifndef __Rdf_h_wanghan__
#define __Rdf_h_wanghan__

#include "Defines.h"
#include "CellList.h"

class Rdf 
{
  int nbins;
  ValueType rup;
  ValueType binSize;
  ValueType offset;
  int nframe;
  ValueType rho;
  ValueType natom;
  std::vector<ValueType > hist;
public:
  const ValueType & getValue (const int & i) const {return hist[i];}
  unsigned getN () const {return hist.size();}
  void reinit (const ValueType rup,
	       const ValueType refh);
  void deposit (const std::vector<std::vector<ValueType> > & coord,
		const VectorType & box,
		const CellList & clist);
  void calculate ();
}
    ;


#endif
