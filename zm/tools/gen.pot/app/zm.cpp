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
  std::string ofile;
  double hbin, xup, alpha, rc;
  int ll;

  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("bin,b",  po::value<double > (&hbin)->default_value (0.002), "bin size")
      ("xup,x",  po::value<double > (&xup)->default_value (2.), "xrange")
      ("alpha,a",  po::value<double > (&alpha)->default_value (1.2), "alpha")
      ("rc,c",  po::value<double > (&rc)->default_value (1.5), "cut-off")
      ("order,l",  po::value<int > (&ll)->default_value (0), "order")
      ("output,o", po::value<std::string > (&ofile)->default_value ("table.xvg"), "the output of the table");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  Potential potzm (ll, alpha, rc);
  
  int nbin = (xup + 0.5 * hbin) / hbin;
  hbin = xup / double(nbin);
  vector<double > xx (nbin+1), vv(nbin+1), ff(nbin+1);

  for (int ii = 0; ii < nbin+1; ++ii){
    xx[ii] = hbin * ii;
    if (ii == 0){
      vv[ii] = ff[ii] = 0.;
    }
    else { 
      vv[ii] = potzm.pot (xx[ii]);
      ff[ii] = potzm.mpotp (xx[ii]);
    }
  }

  for (int ii = 1; ii < nbin; ++ii){
    double tmp = -(vv[ii+1] - vv[ii-1]) / (2.*hbin);
    printf ("%f %e \t %e \t %e\n", xx[ii], vv[ii], ff[ii], fabs(tmp - ff[ii]));
  }
  

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // // check of the implementation of the derivatives:
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // double alpha = 1.2;
  // double xx = 1.4;
  // double hh = 1e-6;
  // double tmpn;
  // tmpn = (funcV(alpha,xx+hh) - funcV(alpha,xx-hh)) / (2.*hh);
  // printf ("check D1:  \tprec %e  \tnumer %e  \tdiff %e\n", funcD1V(alpha,xx), tmpn, fabs(funcD1V(alpha,xx) - tmpn));
  // tmpn = (funcD1V(alpha,xx+hh) - funcD1V(alpha,xx-hh)) / (2.*hh);
  // printf ("check D2:  \tprec %e  \tnumer %e  \tdiff %e\n", funcD2V(alpha,xx), tmpn, fabs(funcD2V(alpha,xx) - tmpn));
  // tmpn = (funcD2V(alpha,xx+hh) - funcD2V(alpha,xx-hh)) / (2.*hh);
  // printf ("check D3:  \tprec %e  \tnumer %e  \tdiff %e\n", funcD3V(alpha,xx), tmpn, fabs(funcD3V(alpha,xx) - tmpn));
  // tmpn = (funcD3V(alpha,xx+hh) - funcD3V(alpha,xx-hh)) / (2.*hh);
  // printf ("check D4:  \tprec %e  \tnumer %e  \tdiff %e\n", funcD4V(alpha,xx), tmpn, fabs(funcD4V(alpha,xx) - tmpn));
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // // end check of the implementation of the derivatives:
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  

  return 0;
}
