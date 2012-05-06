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

#include "Tree.h"
#include "HbondMap.h"


int main(int argc, char * argv[])
{
  HbondMap map;
  map.push_pair (0, 1);
  map.push_pair (0, 2);
  map.push_pair (1, 2);
  map.push_pair (1, 3);
  map.push_pair (2, 3);

  std::vector<Identity > neigbors;
  for (int i = 0; i < 3; ++i){
    if (map.findNeighbors (i, neigbors)){
      printf ("%d      ", i);
      for (unsigned jj = 0; jj < neigbors.size(); ++jj){
	printf ("%d ", neigbors[jj]);
      }
      printf ("\n");
    }
  }
  
  return 0;
}
