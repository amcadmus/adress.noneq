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
  std::string ofile, ifile, idfile, isfile, ismfile;
  double aup, alow, dt;
  unsigned nDataInBlock, nbin;
  
  po::options_description desc ("Calculates the steady distribution. The user should be sure that the trajs are in steady state.\nAllow options");
  desc.add_options()
      ("help,h", "print this message")
      ("dt", po::value<double > (&dt)->default_value (0.5), "feq of conf printing.")
      ("num-data-block", po::value<unsigned > (&nDataInBlock)->default_value (1), "number of data in each block.")
      ("num-bin,n", po::value<unsigned > (&nbin)->default_value (20), "number of blocks.")
      ("angle-up", po::value<double > (&aup)->default_value (180.), "upper bond of the angle.")
      ("angle-low", po::value<double > (&alow)->default_value (-180.), "lower bond of the angle.")      
      ("input-cluster-map", po::value<string > (&ismfile)->default_value ("cluster.map.out"), "the input of cluster map.")
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

  unsigned nbin2 = nbin * nbin;

  vector<vector<BlockAverage_acc > > dists (nbin2);

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
    vector<unsigned > disc_traj;
    unsigned tmpread;
    while (1 == fscanf(fp, "%d", &tmpread)){
      disc_traj.push_back (tmpread);
    }
    fclose (fp);
    if (countFile == 0){
      dists.resize(disc_traj.size());
      for (unsigned ii = 0; ii < disc_traj.size(); ++ii){
	for (unsigned jj = 0; jj < nbin2; ++jj){
	  dists[ii][jj].reinit (nDataInBlock);
	}
      }
    }
    for (unsigned ii = 0; ii < disc_traj.size(); ++ii){
      for (unsigned jj = 0; jj < nbin2; ++jj) {
	if (jj == disc_traj[ii]){
	  dists[ii][jj].deposite (1.);
	}
	else {
	  dists[ii][jj].deposite (0.);
	}
      }
    } 
    countFile ++;
  }
  
  for (unsigned mm = 0; mm < dists.size(); ++mm){
    for (unsigned nn = 0; nn < dists[mm].size(); ++nn){
      dists[mm][nn].calculate ();
    } 
  }

  FILE * fp = fopen (ofile.c_str(), "w");
  if (fp == NULL){
    cerr << "cannot open file " << ofile << endl;
    return 1;
  }
  for (unsigned mm = 0; mm < dists.size(); ++mm){
    fprintf (fp, "%f ", dt * mm);
    for (unsigned nn = 0; nn < dists[mm].size(); ++nn){
      fprintf (fp, "%e ", dists[mm][nn].getAvg());
    } 
  }
  fclose (fp);

  return 0;
}
