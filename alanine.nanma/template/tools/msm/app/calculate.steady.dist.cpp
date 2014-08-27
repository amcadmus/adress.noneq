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
#include "StringSplit.h"
#include "BlockAverage.h"
#include "MetaSet.h"

#define MaxLineLength 65536

namespace po = boost::program_options;
using namespace std;


int main(int argc, char * argv[])
{
  std::string ofile, ifile, idfile, isfile;
  double aup, alow;
  unsigned nDataInBlock, nbin;
  
  po::options_description desc ("Calculates the steady distribution. The user should be sure that the trajs are in steady state.\nAllow options");
  desc.add_options()
      ("help,h", "print this message")
      ("num-data-block", po::value<unsigned > (&nDataInBlock)->default_value (1), "number of data in each block.")
      ("num-bin,n", po::value<unsigned > (&nbin)->default_value (20), "number of blocks.")
      ("angle-up", po::value<double > (&aup)->default_value (180.), "upper bond of the angle.")
      ("angle-low", po::value<double > (&alow)->default_value (-180.), "lower bond of the angle.")      
      ("input-largest-set,s", po::value<std::string > (&isfile)->default_value ("largestSet"), "the input file of largest set.")
      ("input-traj-dir", po::value<string > (&idfile)->default_value ("success.dir.name"), "the traj dir name.")
      ("input-disc-traj", po::value<string > (&ifile)->default_value ("disc.traj"), "the traj file name.")
      ("output", po::value<string > (&ofile)->default_value ("steady.dist.out"), "the output of forward commitor.");
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  vector<int > setMap ;
  {
    FILE * fp  = fopen (isfile.c_str(), "r");
    if (fp == NULL){
      cerr << "cannot open file " << isfile << endl;
      return 1;
    }
    int tmpint;
    while (fscanf(fp, "%d", &tmpint) == 1){
      setMap.push_back (tmpint);
    }
    fclose (fp);
  }
  unsigned nstate = setMap.size();

  unsigned nbin2 = nbin * nbin;

  vector<BlockAverage_acc > dist (nbin2);
  for (unsigned kk = 0; kk < nbin2; ++kk){
    dist[kk].reinit (nDataInBlock);
  }

  ifstream fpname (idfile.c_str());
  if (!fpname){
    std::cerr << "cannot open file " << idfile << std::endl;
    return 1;
  }
  char nameline [MaxLineLength];
  unsigned countFile = 0;
  
  while (fpname.getline(nameline, MaxLineLength)){
    if (nameline[0] == '#') continue;
    string filename = string(nameline) + string("/") + ifile;
    FILE *fp = fopen (filename.c_str(), "r");
    if (fp == NULL){
      std::cerr << "cannot open file " << filename << std::endl;
      return 1;
    }
    if (countFile % 100 == 0){
      cout << "# processing file " << filename << endl;
    }
    countFile ++;
    vector<unsigned > disc_traj;
    unsigned tmpread;
    while (1 == fscanf(fp, "%d", &tmpread)){
      disc_traj.push_back (tmpread);
    }
    fclose (fp);
    for (unsigned ii = 0; ii < disc_traj.size(); ++ii){
      for (unsigned jj = 0; jj < nbin2; ++jj) {
	if (jj == disc_traj[ii]){
	  dist[jj].deposite (1.);
	}
	else {
	  dist[jj].deposite (0.);
	}
      }
    } 
  }
  
  for (unsigned mm = 0; mm < dist.size(); ++mm){
    dist[mm].calculate ();
  } 

  FILE * fp = fopen (ofile.c_str(), "w");
  if (fp == NULL){
    cerr << "cannot open file " << ofile << endl;
    return 1;
  }
  for (unsigned ii = 0; ii < nbin; ++ii){
    double binSize = (aup - alow) / double(nbin);
    double phi = (ii+0.5) * binSize + alow;
    for (unsigned jj = 0; jj < nbin; ++jj){
      double psi = (jj+0.5) * binSize + alow;
      unsigned dihIndex = ii * nbin + jj;
      fprintf (fp, "%f \t %f \t %e %e\n", phi, psi, dist[dihIndex].getAvg(), dist[dihIndex].getAvgError());
    }
    fprintf (fp, "\n");
  }
  fclose (fp);

  string origname = ofile+".orig";
  fp = fopen ((origname).c_str(), "w");
  if (fp == NULL){
    cerr << "cannot open file " << ofile << endl;
    return 1;
  }
  for (unsigned ii = 0; ii < nstate; ++ii){
    fprintf (fp, "%e\n", dist[setMap[ii]].getAvg());
  }
  fclose (fp);

  double eps = 1e-10;
  double sum = 0.;
  for (unsigned ii = 0; ii < nbin2; ++ii){
    sum += dist[ii].getAvg();
  }
  if ( (sum > eps && sum < 1 - eps) ||
       (sum < -eps) ||
       (sum > 1+eps) ){
    cerr << "failed at uniformity test" << endl;
  }
  cout << "# passed test for dist" << endl;
  
  return 0;
}
