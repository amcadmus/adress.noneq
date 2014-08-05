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

unsigned load_cluster_map (const string & file,
			   const unsigned & nbin,
			   vector<unsigned > & clusterMap)
{
  clusterMap.resize(nbin*nbin);
  FILE * fp = fopen (file.c_str(), "r");
  if (fp == NULL){
    cerr << "cannot open file " << file <<  endl;
    exit(1);
  }
  unsigned max = 0;
  for (unsigned ii = 0; ii < nbin * nbin; ++ii){
    if (fscanf (fp, "%d", &clusterMap[ii]) != 1){
      cerr << "error reading " << file << endl;
      exit(1);
    }
    if (clusterMap[ii] > max) max = clusterMap[ii];
  }
  return max;
}

void deposite_traj (const vector<unsigned > & traj,
		    const int & startPosi,
		    const int & stopPosi,
		    const vector<unsigned > & clusterMap,
		    vector<vector<BlockAverage_acc > > & fwCommitor,
		    vector<vector<BlockAverage_acc > > & bwCommitor)
{
  if (startPosi >= 0){
    for (unsigned mm = 0; mm < bwCommitor.size(); ++mm){
      if ((mm+1) == clusterMap[traj[startPosi]]){
	for (int ii = startPosi+1; ii < stopPosi; ++ii){
	  bwCommitor[mm][traj[ii]].deposite (1.);
	}
      }
      else {
	for (int ii = startPosi+1; ii < stopPosi; ++ii){
	  bwCommitor[mm][traj[ii]].deposite (0.);
	}
      }
    }
  }
  if (stopPosi < int(traj.size())){
    for (unsigned mm = 0; mm < fwCommitor.size(); ++mm){
      if ((mm+1) == clusterMap[traj[stopPosi]]){
	for (int ii = startPosi+1; ii < stopPosi; ++ii){
	  fwCommitor[mm][traj[ii]].deposite (1.);
	}
      }
      else {
	for (int ii = startPosi+1; ii < stopPosi; ++ii){
	  fwCommitor[mm][traj[ii]].deposite (0.);
	}
      }
    }
  }
}

int main(int argc, char * argv[])
{
  std::string ofwfile, obwfile, ifile, idfile, isfile, ismfile;
  double aup, alow;
  unsigned nDataInBlock, nbin;
  
  po::options_description desc ("Calculates the forward and backward commitor.\nAllow options");
  desc.add_options()
      ("help,h", "print this message")
      ("num-data-block", po::value<unsigned > (&nDataInBlock)->default_value (1), "number of data in each block.")
      ("num-bin,n", po::value<unsigned > (&nbin)->default_value (20), "number of blocks.")
      ("angle-up", po::value<double > (&aup)->default_value (180.), "upper bond of the angle.")
      ("angle-low", po::value<double > (&alow)->default_value (-180.), "lower bond of the angle.")      
      ("input-cluster-map", po::value<string > (&ismfile)->default_value ("cluster.map.out"), "the input of cluster map.")
      ("input-traj-dir", po::value<string > (&idfile)->default_value ("success.dir.name"), "the traj dir name.")
      ("input-disc-traj", po::value<string > (&ifile)->default_value ("disc.traj"), "the traj file name.")
      ("output-fw", po::value<string > (&ofwfile)->default_value ("commitor.fw.out"), "the output of forward commitor.")
      ("output-bw", po::value<string > (&obwfile)->default_value ("commitor.bw.out"), "the output of backward commitor.");
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  double binSize = (aup - alow) / double(nbin);
  unsigned nbin2 = nbin * nbin;

  vector<unsigned > clusterMap;
  unsigned numCluster = load_cluster_map (ismfile, nbin, clusterMap);

  vector<vector<BlockAverage_acc > > fwCommitor (numCluster);
  vector<vector<BlockAverage_acc > > bwCommitor (numCluster);
  for (unsigned ii= 0; ii < numCluster; ++ii){
    fwCommitor[ii].resize (nbin2);
    bwCommitor[ii].resize (nbin2);
    for (unsigned kk = 0; kk < nbin2; ++kk){
      fwCommitor[ii][kk].reinit (nDataInBlock);
      bwCommitor[ii][kk].reinit (nDataInBlock);
    }
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
    int startPosi = 0;
    int stopPosi = 0;
    while (stopPosi < int(disc_traj.size())){
      while (stopPosi < int(disc_traj.size()) && clusterMap[disc_traj[stopPosi]] != 0){
	stopPosi ++;
      }
      startPosi = stopPosi - 1;
      while (stopPosi < int(disc_traj.size()) && clusterMap[disc_traj[stopPosi]] == 0){
	stopPosi ++;
      }
      deposite_traj (disc_traj, startPosi, stopPosi, clusterMap, fwCommitor, bwCommitor);
    }
  }
  
  for (unsigned mm = 0; mm < fwCommitor.size(); ++mm){
    for (unsigned ii = 0; ii < fwCommitor[mm].size(); ++ii){
      fwCommitor[mm][ii].calculate ();
      bwCommitor[mm][ii].calculate ();
    }
  } 

  FILE * fpfw = fopen (ofwfile.c_str(), "w");
  if (fpfw == NULL){
    cerr << "cannot open file " << ofwfile << endl;
    return 1;
  }
  FILE * fpbw = fopen (obwfile.c_str(), "w");
  if (fpbw == NULL){
    cerr << "cannot open file " << obwfile << endl;
    return 1;
  }
  for (unsigned ii = 0; ii < nbin; ++ii){
    double phi = (ii+0.5) * binSize + alow;
    for (unsigned jj = 0; jj < nbin; ++jj){
      double psi = (jj+0.5) * binSize + alow;
      unsigned dihIndex = ii * nbin + jj;
      fprintf (fpfw, "%f \t %f \t ", phi, psi);
      fprintf (fpbw, "%f \t %f \t ", phi, psi);
      if (clusterMap[dihIndex] == 0) {
	for (unsigned mm = 0; mm < fwCommitor.size(); ++mm){
	  fprintf (fpfw, " %f %f ", fwCommitor[mm][dihIndex].getAvg(), fwCommitor[mm][dihIndex].getAvgError());
	  fprintf (fpbw, " %f %f ", fwCommitor[mm][dihIndex].getAvg(), fwCommitor[mm][dihIndex].getAvgError());
	}
      }
      else {
	for (unsigned mm = 0; mm < fwCommitor.size(); ++mm){
	  if ((mm+1) == clusterMap[dihIndex]) {
	    fprintf (fpfw, " %f %f ", 1., 0.);
	    fprintf (fpbw, " %f %f ", 1., 0.);
	  }
	  else {
	    fprintf (fpfw, " %f %f ", 0., 0.);
	    fprintf (fpbw, " %f %f ", 0., 0.);
	  }	    
	}
      }
      fprintf (fpfw, "\n");
      fprintf (fpbw, "\n");
    }
    fprintf (fpfw, "\n");
    fprintf (fpbw, "\n");
  }

  fclose (fpfw);
  fclose (fpbw);

  double eps = 1e-10;
  for (unsigned ii = 0; ii < nbin2; ++ii){
    double sum = 0.;
    for (unsigned mm = 0; mm < numCluster; ++mm){
      sum += fwCommitor[mm][ii].getAvg();
    }
    if ( (sum > eps && sum < 1 - eps) ||
	 (sum < -eps) ||
	 (sum > 1+eps) ){
      cerr << "failed at fw commitor test" << endl;
    }
    sum = 0.;
    for (unsigned mm = 0; mm < numCluster; ++mm){
      sum += bwCommitor[mm][ii].getAvg();
    }
    if ( (sum > eps && sum < 1 - eps) ||
	 (sum < -eps) ||
	 (sum > 1+eps) ){
      cerr << "failed at bw commitor test" << endl;
    }
  }
  cout << "# passed test for commitors" << endl;
  
  return 0;
}
