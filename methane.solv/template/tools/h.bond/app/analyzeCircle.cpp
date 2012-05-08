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

using namespace CircleOperations;

int main(int argc, char * argv[])
{
  HbondMap map, newMap;

  map.push_pair (0,1);
  map.push_pair (1,2);
  map.push_pair (2,3);
  map.push_pair (3,4);
  map.push_pair (4,5);
  map.push_pair (5,0);
  map.push_pair (0,2);
  map.push_pair (1,3);
  map.push_pair (2,4);
  map.push_pair (3,5);
  map.push_pair (4,0);
  map.push_pair (5,1);
  
  map.push_pair (101, 102);
  map.push_pair (101, 103);
  map.push_pair (102, 103);

  map.push_pair (202, 203);

  Tree tree;
  Circles cs;

  while (!map.empty()){
    tree.addRoot(map, 0);
    tree.addGenerations(map);
    Circles tmpc;
    tree.buildCircles (tmpc);
    tmpc.uniqueCircles();
    tmpc.simplifyCircles ();
    cs.add (tmpc);
    HbondMap newMap;
    tree.renewMap (map, newMap);
    map = newMap;
  }

  cs.uniqueCircles();
  cs.sortCircles();
  std::cout << std::endl;
  std::cout << "simplified circles:" << std::endl;
  cs.print ();


}
