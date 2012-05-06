#include "Hbond.h"
#include <algorithm>

void HbondMap::
push_pair (const Identity & i,
	   const Identity & j)
{
  unsigned iidx, jidx;
  bool find = false;
  for (unsigned ii = 0; ii < nodes.size(); ++ii){
    if (i == nodes[ii]){
      find = true;
      iidx = ii;
      break;
    }
  }
  if (find == false){
    iidx = nodes.size();
    nodes.push_back (i);
    neighbors_nodes.push_back (std::vector<unsigned >());
  }
  find = false;
  for (unsigned ii = 0; ii < nodes.size(); ++ii){
    if (j == nodes[ii]){
      find = true;
      jidx = ii;
      break;
    }
  }
  if (find == false){
    jidx = nodes.size();
    nodes.push_back (j);
    neighbors_nodes.push_back (std::vector<unsigned >());
  }  
  
  neighbors_nodes[iidx].push_back (jidx);
  neighbors_nodes[jidx].push_back (iidx);
}

bool HbondMap::
findNeighbors (const Identity & me,
	       std::vector<Identity > & neighbors) const
{
  neighbors.clear ();
  bool find = false;
  for (unsigned ii = 0; ii < nodes.size(); ++ii){
    if (me == nodes[ii]){
      find = true;
      for (unsigned jj = 0; jj < neighbors_nodes.size(); ++jj){
	neighbors.push_back (nodes[neighbors_nodes[ii][jj]]);
      }
      break;
    }
  }
  return find;	
}



