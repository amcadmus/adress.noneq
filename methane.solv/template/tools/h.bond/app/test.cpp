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
#include "Hbond.h"


int main(int argc, char * argv[])
{
  HbondMap map;
  map.push_pair (0, 1);
  map.push_pair (0, 2);
  map.push_pair (1, 2);
  map.push_pair (1, 3);
  map.push_pair (2, 3);

  return 0;
}
