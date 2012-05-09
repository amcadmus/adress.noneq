#ifndef __Analyzer_h_wanghan__
#define __Analyzer_h_wanghan__

#include "Tree.h"
#include "HbondMap.h"
#include "Circles.h"

class Analyzer 
{
  HbondMap map;  
  Tree tree;
  Circles cs;
public:
  void clear ();
  void readData (const std::vector<Hbond> & bonds);
  const Circles & getCircles () const;
}
    ;


inline void Analyzer::
clear ()
{
  cs.clear ();
}

inline const Circles &  Analyzer::
getCircles () const
{
  return cs;
}

#endif
