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
#include "Analyzer.h"

using namespace CircleOperations;

int main(int argc, char * argv[])
{
  // HbondMap map;

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

  // // map.push_pair (0, 1);
  // // map.push_pair (0, 2);
  // // map.push_pair (0, 3);
  // // map.push_pair (1, 3);
  // // map.push_pair (2, 3);
  // // map.push_pair (1, 4);
  // // map.push_pair (2, 5);
  // // map.push_pair (4, 6);
  // // map.push_pair (5, 6);

  // // map.push_pair (101, 102);
  // // map.push_pair (101, 103);
  // // map.push_pair (102, 103);

  // // map.push_pair (202, 203);

  // Tree tree;
  // Circles cs;

  // while (!map.empty()){
  //   tree.addRoot(map, 0);
  //   tree.addGenerations(map);
  //   // tree.print();
  //   Circles tmpc;
  //   tree.buildCircles (tmpc);
  //   tmpc.uniqueCircles();
  //   // std::cout << std::endl;
  //   // std::cout << "before simplified:" << std::endl;
  //   // tmpc.print();
  //   tmpc.simplifyCircles ();
  //   // std::cout << std::endl;
  //   // std::cout << "simplified circles:" << std::endl;
  //   // tmpc.print();    
  //   cs.add (tmpc);
  //   HbondMap newMap;
  //   tree.renewMap (map, newMap);
  //   map = newMap;
  // }

  // cs.uniqueCircles();
  // cs.sortCircles();
  // std::cout << std::endl;
  // std::cout << "simplified circles:" << std::endl;
  // cs.print ();


  std::vector<Hbond> bonds;
  bonds.push_back (Hbond(0,1));
  bonds.push_back (Hbond(1,2));
  bonds.push_back (Hbond(2,3));
  bonds.push_back (Hbond(3,4));
  bonds.push_back (Hbond(4,5));
  bonds.push_back (Hbond(5,0));
  bonds.push_back (Hbond(0,2));
  bonds.push_back (Hbond(1,3));
  bonds.push_back (Hbond(2,4));
  bonds.push_back (Hbond(3,5));
  bonds.push_back (Hbond(4,0));
  bonds.push_back (Hbond(5,1));


  // bonds.push_back (Hbond(0,1));
  // bonds.push_back (Hbond(0,2));
  // bonds.push_back (Hbond(1,3));
  // bonds.push_back (Hbond(2,3));
  // bonds.push_back (Hbond(2,4));
  // bonds.push_back (Hbond(1,4));

  Analyzer ana;
  ana.readData (bonds);
  std::cout << std::endl;
  std::cout << "simplified circles:" << std::endl;
  ana.getCircles().print ();
  

  return 0;
}
