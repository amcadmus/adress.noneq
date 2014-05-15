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
#include <boost/program_options.hpp>
#include "Functions.h"

#define MaxLineLength 2048

namespace po = boost::program_options;
using namespace std;
using namespace ZeroMultipole;

int main(int argc, char * argv[])
{
  double alpha, rc, qo;
  int ll;

  po::options_description desc ("\nTo calcualte the energy correction for one water molecule.\nPlz provide the charge of the oxygen.\n\nAllow options");
  desc.add_options()
      ("help,h", "print this message")
      ("alpha,a",  po::value<double > (&alpha)->default_value (1.2), "alpha")
      ("rc,c",  po::value<double > (&rc)->default_value (1.5), "cut-off")
      ("order,l",  po::value<int > (&ll)->default_value (0), "order")
      ("charge-o,q",  po::value<double > (&qo)->default_value (-0.834), "charge of oxygen");
    
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  Potential potzm (ll, alpha, rc);

  double qh = -0.5 * qo;
  vector<double > charges;
  charges.push_back (qo);
  charges.push_back (qh);
  charges.push_back (qh);

  cout << potzm.energyCorr (charges) << endl;
}
