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
  HbondMap map;

  // map.push_pair (0, 1);
  // map.push_pair (0, 2);
  // map.push_pair (1, 2);
  // map.push_pair (1, 3);
  // map.push_pair (2, 3);

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

  // map.push_pair (0,1);
  // map.push_pair (0,2);
  // map.push_pair (1,3);
  // map.push_pair (2,3);
  // map.push_pair (2,4);
  // map.push_pair (3,5);
  // map.push_pair (4,5);
  // map.push_pair (4,6);
  // map.push_pair (5,7);
  // map.push_pair (6,7);
  // map.push_pair (6,7);

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
  // map.push_pair (11,5);
  // map.push_pair (11,6);
  // map.push_pair (11,7);
  // map.push_pair (11,8);

  // map.push_pair (0,1);
  // map.push_pair (1,2);
  // map.push_pair (2,3);
  // map.push_pair (3,4);
  // map.push_pair (4,5);
  // map.push_pair (5,6);
  // map.push_pair (6,7);
  // map.push_pair (7,0);
  // map.push_pair (8,1);
  // map.push_pair (8,5);
  // map.push_pair (9,1);
  // map.push_pair (9,5);
  
  map.push_pair (101, 102);
  map.push_pair (101, 103);
  map.push_pair (102, 103);

  
  Tree tree;
  tree.addRoot(map, 5);
  tree.addGenerations(map);
  // std::cout << "is circular: " << tree.isCircular() << std::endl;
  tree.print ();

  std::cout << std::endl;
  std::cout << "circles are: " << std::endl;
  Circles c;
  tree.buildCircles (c);
  c.print ();
  c.uniqueCircles();
  std::cout << std::endl;
  c.print ();

  std::cout << std::endl;
  std::cout << "simplified:" << std::endl;
  c.simplifyCircles ();
  c.print ();
  
  HbondMap newMap;
  tree.renewMap (map, newMap);
  std::cout << std::endl;
  std::cout << "new tree: " << std::endl;
  newMap.print();
  

  // std::vector<std::vector<unsigned > > p0, p1;
  // CircleOperations::findCommonPatterns (c.circles[0], c.circles[1],
  // 					p0, p1);

  // Circle c0, c1;
  // c0.push_back (0);
  // c0.push_back (1);
  // c0.push_back (2);
  // c0.push_back (3);
  // c0.push_back (4);
  // c0.push_back (11);
  // c0.push_back (12);
  // c0.push_back (13);
  // c0.push_back (14);
  // // c0.push_back (101);
  // // c0.push_back (102);
  // // c0.push_back (103);
  // // c0.push_back (104);
  // c0.push_back (31);
  // c0.push_back (32);
  // c0.push_back (33);
 
  // c1.push_back (0);
  // c1.push_back (1);
  // c1.push_back (2);
  // c1.push_back (3);
  // c1.push_back (4);
  // c1.push_back (21);
  // c1.push_back (22);
  // c1.push_back (23);
  // // c1.push_back (101);
  // // c1.push_back (102);
  // // c1.push_back (103);
  // // c1.push_back (104);
  // c1.push_back (41);
  // c1.push_back (42);
  // c1.push_back (43);
  // c1.push_back (44);
  // c1.push_back (45);
  // std::cout << std::endl;
  // std::cout << std::endl;  
  // // findCommonPatterns (c0, c1, p0, p1);
  // Circle newCircle = diffCircle (c0, c1);
  // for (unsigned ii = 0; ii < newCircle.size(); ++ii){
  //   printf ("%d ", newCircle[ii]);
  // }
  // printf ("\n");
  
  return 0;
}
