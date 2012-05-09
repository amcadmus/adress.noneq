#include "Analyzer.h"

using namespace CircleOperations;

void Analyzer::
readData (const std::vector<Hbond> & bonds)
{
  map.clear();
  tree.clear();
  for (unsigned ii = 0; ii < bonds.size(); ++ii){
    map.push_bond (bonds[ii]);
  }
  
  while (!map.empty()){
    tree.addRoot(map, 0);
    tree.addGenerations(map);
    // tree.print();
    Circles tmpc;
    tree.buildCircles (tmpc);
    tmpc.uniqueCircles();
    // std::cout << std::endl;
    // std::cout << "before simplified:" << std::endl;
    // tmpc.print();
    tmpc.simplifyCircles ();
    // std::cout << std::endl;
    // std::cout << "simplified circles:" << std::endl;
    // tmpc.print();    
    cs.add (tmpc);
    HbondMap newMap;
    tree.renewMap (map, newMap);
    map = newMap;
  }

  cs.uniqueCircles();
  cs.sortCircles();
}

