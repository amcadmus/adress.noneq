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
#define MaxLineLength 10240

#include "StringSplit.h"
#include "BlockAverage.h"

using namespace std;

int main(int argc, char * argv[])
{
  float begin, end;
  std::string afile, ffile, ofile;
  float time_prec = .01;
  unsigned nblock;
  double corrLength;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("begin,b", po::value<float > (&begin)->default_value(0.f), "start time")
      ("end,e",   po::value<float > (&end  )->default_value(0.f), "end   time")
      ("corr-length,l",   po::value<double > (&corrLength)->default_value(1.), "correlation length")
      ("nblock",   po::value<unsigned > (&nblock)->default_value(30), "number of blocks")
      ("angle-file,a",po::value<std::string > (&afile)->default_value ("angaver.psi.xvg"), "the angle file")
      ("flux-file,f",  po::value<std::string > (&ffile)->default_value ("gxs.out"), "the flux file")
      ("output,o",   po::value<std::string > (&ofile)->default_value ("corr.out"), "out put corr for angle value");
      
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }
  
  std::cout << "###################################################" << std::endl;
  std::cout << "# begin->end:  " << begin << " " << end << std::endl;
  std::cout << "# angle file:  " << afile << std::endl;
  std::cout << "# flux file:   " << ffile << std::endl;
  std::cout << "# corr length: " << corrLength << std::endl;
  std::cout << "# out file:    " << ofile << std::endl;
  std::cout << "###################################################" << std::endl;  

  ifstream fpa (afile.c_str());
  ifstream fpf (ffile.c_str());
  if (!fpa){
    std::cerr << "cannot open file " << afile << std::endl;
    return 1;
  }
  if (!fpf){
    std::cerr << "cannot open file " << ffile << std::endl;
    return 1;
  }

  char line [MaxLineLength];
  vector<double > times;
  vector<double > angles;
  vector<double > fluxes;

  while (fpa.getline(line, MaxLineLength)){
    if (line[0] == '#') continue;
    if (line[0] == '@') continue;
    vector<std::string > words;
    StringOperation::split (string(line), words);
    if (end != 0 && atof(words[0].c_str()) > end + time_prec) {
      break;
    } 
    times.push_back (atof(words[0].c_str()));
    angles.push_back (atof(words[1].c_str()));
  }
  while (fpf.getline(line, MaxLineLength)){
    if (line[0] == '#') continue;
    if (line[0] == '@') continue;
    vector<std::string > words;
    StringOperation::split (string(line), words);
    if (end != 0 && atof(words[0].c_str()) > end + time_prec) {
      break;
    } 
    fluxes.push_back (atof(words[1].c_str()));
  }

  double dt = times[1] - times[0];
  int ncorr = int((corrLength + 0.5 * dt) / dt + 1);
  vector<BlockAverage_acc > corrs (ncorr);
  for (int ii = 0; ii < ncorr; ++ii){
    corrs[ii].reinit (nblock);
  }

  for (int ii = ncorr; ii < int(times.size()); ++ii){
    if (ii % 1000 == 0){
      printf ("# compute at time %f    \r", times[ii]);
      fflush (stdout);
    }
    if (end != 0.f) {
      if (times[ii] < begin - time_prec){
        continue;
      }
      else if (times[ii] > end + time_prec) {
        break;
      } 
    }
    else {
      if (times[ii] < begin - time_prec) continue;
    }
    for (int jj = 0; jj < ncorr; ++jj){
      corrs[jj].deposite (angles[ii] * fluxes[ii-jj]);
    }
  }
  printf ("\n");

  for (int ii = 0; ii < ncorr; ++ii){
    corrs[ii].calculate();
  }

  FILE * fp = fopen (ofile.c_str(), "w");
  for (int ii = 0; ii < ncorr; ++ii){
    fprintf (fp, "%f %e %e\n", dt * ii, corrs[ii].getAvg(), corrs[ii].getAvgError());
  }
  fclose (fp);
  
  return 0;
}

