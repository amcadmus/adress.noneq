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
  std::vector<Identity > neigbors;

  // map.push_pair (0, 1);
  // map.push_pair (0, 2);
  // map.push_pair (1, 2);
  // map.push_pair (1, 3);
  // map.push_pair (2, 3);
  // for (unsigned i = 0; i < map.nNodes(); ++i){
  //   if (map.findNeighbors (i, neigbors)){
  //     printf ("%d      ", i);
  //     for (unsigned jj = 0; jj < neigbors.size(); ++jj){
  // 	printf ("%d ", neigbors[jj]);
  //     }
  //     printf ("\n");
  //   }
  // }
  // printf ("\n");

  // Tree tree;
  // tree.addRoot(0);
  // tree.addGeneration (map);
  // tree.print ();
  // tree.addGeneration (map);
  // tree.print ();


  // map.push_pair (0,1);
  // map.push_pair (1,2);
  // map.push_pair (2,3);
  // map.push_pair (3,4);
  // map.push_pair (4,5);
  // map.push_pair (5,0);
  // map.push_pair (0,2);
  // map.push_pair (1,3);
  // map.push_pair (2,4);
  // map.push_pair (3,5);
  // map.push_pair (4,0);
  // map.push_pair (5,1);
  // for (unsigned i = 0; i < map.nNodes(); ++i){
  //   if (map.findNeighbors (i, neigbors)){
  //     printf ("%d      ", i);
  //     for (unsigned jj = 0; jj < neigbors.size(); ++jj){
  // 	printf ("%d ", neigbors[jj]);
  //     }
  //     printf ("\n");
  //   }
  // }
  // printf ("\n");

  map.push_pair (0,1);
  map.push_pair (0,2);
  map.push_pair (1,3);
  map.push_pair (2,3);
  map.push_pair (2,4);
  map.push_pair (3,5);
  map.push_pair (4,5);
  map.push_pair (4,6);
  map.push_pair (5,7);
  map.push_pair (6,7);
  map.push_pair (6,7);
  // for (unsigned i = 0; i < map.nNodes(); ++i){
  //   if (map.findNeighbors (i, neigbors)){
  //     printf ("%d      ", i);
  //     for (unsigned jj = 0; jj < neigbors.size(); ++jj){
  // 	printf ("%d ", neigbors[jj]);
  //     }
  //     printf ("\n");
  //   }
  // }
  // printf ("\n");

  // map.push_pair (0,1);
  // map.push_pair (0,2);
  // map.push_pair (1,3);
  // map.push_pair (2,3);
  // map.push_pair (4,3);
  // map.push_pair (4,5);
  // map.push_pair (4,6);
  // map.push_pair (5,7);
  // map.push_pair (6,8);
  // map.push_pair (7,8);
  // map.push_pair (7,9);
  // map.push_pair (8,10);

  Tree tree;
  tree.addRoot(0);
  while (tree.addGeneration (map)){
  }
  tree.buildCables();
  std::cout << "is circular: " << tree.isCircular() << std::endl;
  tree.print ();

  std::cout << std::endl;
  std::cout << "circles are: " << std::endl;
  Circles c;
  tree.buildCircles (c);
  c.uniqueCircles();
  c.print ();

  std::vector<std::vector<unsigned > > p0, p1;
  CircleOperations::findCommonPatterns (c.circles[0], c.circles[1],
					p0, p1);
  
  return 0;
}
