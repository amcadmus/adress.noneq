#ifndef __Hbond_h_wanghan__
#define __Hbond_h_wanghan__

#include <vector>
#include "Defines.h"

class HbondMap
{
public:
  std::vector<Identity > nodes;
  std::vector<std::vector<unsigned > > neighbors_nodes;
  bool findNeighbors_nodes (const Identity & me,
			    std::vector<Identity > & neighbors) const;
public:
  void clear ();
  void push_pair (const Identity & i,
		  const Identity & j);
  unsigned nNodes () const {return nodes.size();}
  bool findNeighbors (const Identity & me,
		      std::vector<Identity > & neighbors) const;
  void buildSubMap (HbondMap & map) const;
  bool empty () const;
  void print () const;
}
    ;

inline bool HbondMap::
empty () const 
{
  return nodes.empty();
}

inline void HbondMap::
clear () 
{
  nodes.clear();
  neighbors_nodes.clear();
}



#endif
