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
		    vector<vector<BlockAverage_acc > > & fht)
{
  if (stopPosi < int(traj.size())){
    for (unsigned mm = 0; mm < fht.size(); ++mm){
      if ((mm+1) == clusterMap[traj[stopPosi]]){
	for (int ii = startPosi+1; ii < stopPosi; ++ii){
	  fht[mm][traj[ii]].deposite (stopPosi - ii);
	}
      }
    }
  }
}

int main(int argc, char * argv[])
{
  std::string ofhtfile, ifile, idfile, isfile, ismfile;
  double aup, alow;
  unsigned nDataInBlock, nbin;
  
  po::options_description desc ("Calculates the average first hitting time in unit of T.\nAllow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input-largest-set,s", po::value<std::string > (&isfile)->default_value ("largestSet"), "the input file of largest set.")
      ("num-data-block", po::value<unsigned > (&nDataInBlock)->default_value (1), "number of data in each block.")
      ("num-bin,n", po::value<unsigned > (&nbin)->default_value (20), "number of blocks.")
      ("angle-up", po::value<double > (&aup)->default_value (180.), "upper bond of the angle.")
      ("angle-low", po::value<double > (&alow)->default_value (-180.), "lower bond of the angle.")      
      ("input-cluster-map", po::value<string > (&ismfile)->default_value ("cluster.map.out"), "the input of cluster map.")
      ("input-traj-dir", po::value<string > (&idfile)->default_value ("success.dir.name"), "the traj dir name.")
      ("input-disc-traj", po::value<string > (&ifile)->default_value ("disc.traj"), "the traj file name.")
      ("output", po::value<string > (&ofhtfile)->default_value ("fht.out"), "the output of avg first hitting time.");
  
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
  map<unsigned,unsigned> mymap;
  for (unsigned ii = 0; ii < setMap.size(); ++ii){
    mymap[setMap[ii]] = ii;
  }

  double binSize = (aup - alow) / double(nbin);
  unsigned nbin2 = nbin * nbin;

  vector<unsigned > clusterMap;
  unsigned numCluster = load_cluster_map (ismfile, nbin, clusterMap);

  vector<vector<BlockAverage_acc > > fht (numCluster);
  for (unsigned ii= 0; ii < numCluster; ++ii){
    fht[ii].resize (nbin2);
    for (unsigned kk = 0; kk < nbin2; ++kk){
      fht[ii][kk].reinit (nDataInBlock);
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
      deposite_traj (disc_traj, startPosi, stopPosi, clusterMap, fht);
    }
  }
  
  for (unsigned mm = 0; mm < fht.size(); ++mm){
    for (unsigned ii = 0; ii < fht[mm].size(); ++ii){
      fht[mm][ii].calculate ();
    }
  } 

  FILE * fpfht = fopen (ofhtfile.c_str(), "w");
  if (fpfht == NULL){
    cerr << "cannot open file " << ofhtfile << endl;
    return 1;
  }
  // double veryLarge=1e10;
  for (unsigned ii = 0; ii < nbin; ++ii){
    double phi = (ii+0.5) * binSize + alow;
    for (unsigned jj = 0; jj < nbin; ++jj){
      double psi = (jj+0.5) * binSize + alow;
      unsigned dihIndex = ii * nbin + jj;
      fprintf (fpfht, "%f \t %f \t ", phi, psi);
      map<unsigned, unsigned> :: const_iterator it = mymap.find(dihIndex);
      // if (it != mymap.end()){
	for (unsigned mm = 0; mm < fht.size(); ++mm){
	  // if (clusterMap[dihIndex] != (mm+1)) {
	    fprintf (fpfht, " %f %f ", fht[mm][dihIndex].getAvg(), fht[mm][dihIndex].getAvgError());
	  // }
	  // else{
	  //   fprintf (fpfht, " %f %f ", 0., 0.);
	  // }
	}
      // }
      // else {
      // 	for (unsigned mm = 0; mm < fht.size(); ++mm){
      // 	  fprintf (fpfht, " %f %f ", veryLarge, 0.);
      // 	}
      // }
      fprintf (fpfht, "\n");
    }
    fprintf (fpfht, "\n");
  }

  fclose (fpfht);
  
  return 0;
}
