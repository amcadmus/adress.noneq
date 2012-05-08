#include "HbondMap.h"
#include <algorithm>
#include <fstream>

void HbondMap::
push_pair (const Identity & i,
	   const Identity & j)
{
  if (i == j) return;
  
  unsigned iidx, jidx;
  bool ifind = false;
  for (unsigned ii = 0; ii < nodes.size(); ++ii){
    if (i == nodes[ii]){
      ifind = true;
      iidx = ii;
      break;
    }
  }
  if (ifind == false){
    iidx = nodes.size();
    nodes.push_back (i);
    neighbors_nodes.push_back (std::vector<unsigned >());
  }
  bool jfind = false;
  for (unsigned ii = 0; ii < nodes.size(); ++ii){
    if (j == nodes[ii]){
      jfind = true;
      jidx = ii;
      break;
    }
  }
  if (jfind == false){
    jidx = nodes.size();
    nodes.push_back (j);
    neighbors_nodes.push_back (std::vector<unsigned >());
  }  

  if (ifind && jfind){
    for (unsigned ii = 0; ii < neighbors_nodes[iidx].size(); ++ii){
      if (neighbors_nodes[iidx][ii] == jidx){
	return;
      }
    }
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
      for (unsigned jj = 0; jj < neighbors_nodes[ii].size(); ++jj){
	neighbors.push_back (nodes[neighbors_nodes[ii][jj]]);
      }
      break;
    }
  }
  return find;	
}

// bool HbondMap::
// findNeighbors_nodes (const Identity & me,
// 		     std::vector<Identity > & neighbors) const
// {
//   neighbors.clear ();
//   bool find = false;
//   for (unsigned ii = 0; ii < nodes.size(); ++ii){
//     if (me == nodes[ii]){
//       find = true;
//       for (unsigned jj = 0; jj < neighbors_nodes[ii].size(); ++jj){
// 	neighbors.push_back (neighbors_nodes[ii][jj]);
//       }
//       break;
//     }
//   }
//   return find;	
// }
  


void HbondMap::
buildSubMap (HbondMap & map) const
{
  map.neighbors_nodes.clear();
  map.neighbors_nodes.resize(map.nodes.size());
  for (unsigned ii = 0; ii < map.nodes.size(); ++ii){
    std::vector<Identity > neighbors;
    bool find = findNeighbors (map.nodes[ii], neighbors);
    if (find){
      for (unsigned jj = ii+1; jj < map.nodes.size(); ++jj){
	for (unsigned kk = 0; kk < neighbors.size(); ++kk){
	  if (map.nodes[jj] == neighbors[kk]){
	    map.neighbors_nodes[ii].push_back(jj);
	    map.neighbors_nodes[jj].push_back(ii);
	    break;
	  }
	}
      }	      
      // map.neighbors_nodes[ii] = neighbors;
    }
    else {
      // map.neighbors_nodes[ii].clear();
    }
  }
}




void HbondMap::
print () const
{
  std::vector<Identity > neigbors;
  for (unsigned i = 0; i < nNodes(); ++i){
    if (findNeighbors (nodes[i], neigbors)){
      printf ("%d      ", nodes[i]);
      for (unsigned jj = 0; jj < neigbors.size(); ++jj){
  	printf ("%d ", neigbors[jj]);
      }
      printf ("\n");
    }
  }
  printf ("\n");
}
