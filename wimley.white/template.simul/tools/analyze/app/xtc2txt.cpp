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
#include <cmath>
#include <boost/program_options.hpp>
#include <vector>

#include "Trajectory.h"

#define MaxLineLength 2048

namespace po = boost::program_options;
using namespace std;



int main(int argc, char * argv[])
{
  std::string ofile, ifile;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input,f", po::value<std::string > (&ifile)->default_value ("traj.xtc"), "the input xtc file.")
      ("output,o", po::value<std::string > (&ofile)->default_value ("traj.txt"), "the output of the trajectory");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  XtcLoader xtcl;
  xtcl.reinit (ifile.c_str());

  FILE * fp = fopen (ofile.c_str(), "w");
  if (fp ==NULL){
    cerr << "cannot open file " << ofile << endl;
    return 1;
  }
  
  while (xtcl.load()){
    vector<vector<double > > frame;
    xtcl.getFrame (frame);
    for (unsigned ii = 0; ii < frame.size(); ++ii){
      for (unsigned dd = 0; dd < 3; ++dd){
	fprintf (fp, "%.3f\t", frame[ii][dd]);
      }
    }
    fprintf (fp, "\n");
  }
  fclose (fp);
  
  return 0;
}
