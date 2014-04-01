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

namespace po = boost::program_options;
using namespace std;

#define MaxLineLength 10240

int main(int argc, char * argv[])
{
  double xup;
  double bin;
  double xex;
  double xhy;
  double pot0;
  string ofile;
  
  po::options_description desc ("Allow options");
  desc.add_options()
    ("help,h", "print this message")
    ("xup,x", po::value<double > (&xup)->default_value (10.0), "up of x")
    ("bin,b", po::value<double > (&bin)->default_value (0.002), "size of the bin")
    ("xex", po::value<double > (&xex)->default_value (1.0), "xex")
    ("xhy", po::value<double > (&xhy)->default_value (2.0), "xhy")
    ("pot0,p", po::value<double > (&pot0)->default_value (10.0), "potential at x = 0")
    ("output,o",   po::value<std::string > (&ofile)->default_value ("tabletf_CG.xvg"), "output file");
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  int nx = int((xup + 0.5 * bin) / bin) + 1;
  
  FILE *fp = fopen (ofile.c_str(), "w");
  if (fp == NULL){
    std::cerr << "cannot open file " << ofile << std::endl;
    exit (1);
  }

  for (int ii = 0; ii < nx; ++ii){
    double tmpx = ii * bin;
    if (tmpx <= xex){      
      fprintf (fp, "%e   %e %e\n", tmpx, pot0, 0.);
    }
    else if (tmpx >= xhy){
      fprintf (fp, "%e   %e %e\n", tmpx, 0., 0.);
    }
    else {
      fprintf (fp, "%e   %e %e\n", tmpx, 
	       0.5 * (cos ((tmpx - xex) / (xhy - xex) * M_PI ) + 1) * pot0,
	       0.5 * (sin ((tmpx - xex) / (xhy - xex) * M_PI )) * pot0 * M_PI / (xhy - xex));
    }
  }
  
  fclose (fp);
  
  return 0;
}

