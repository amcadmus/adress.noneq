#ifndef __Hbond_h_wanghan__
#define __Hbond_h_wanghan__

#include <vector>
#include "Defines.h"

class HbondMap
{
  std::vector<Identity > nodes;
  std::vector<std::vector<unsigned > > neighbors_nodes;
public:
  void push_pair (const Identity & i,
		  const Identity & j);
  bool findNeighbors (const Identity & me,
		      std::vector<Identity > & neighbors) const;
}
    ;


#endif
