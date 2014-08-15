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

void load_cluster (const string &ifile,
		   vector<double > & cluster)
{
  cluster.clear ();
  FILE * fp = fopen (ifile.c_str(), "r");
  if (fp == NULL){
    cerr << "cannot open angle file " << ifile << endl;
    exit( 1);
  }
  double tmp;
  while (1 == fscanf(fp, "%lf", &tmp) ){
    cluster.push_back (tmp);
  }
  fclose(fp);
}


int main(int argc, char * argv[])
{
  std::string ofile, ifile, isfile;
  double aup, alow;
  unsigned nbin;
  
  po::options_description desc ("Allow options");
  desc.add_options()
    ("help,h", "Convert set to dihedral")
    ("input-largest-set,s", po::value<std::string > (&isfile)->default_value ("largestSet"), "the input file of largest set.")
    ("num-bin,n", po::value<unsigned > (&nbin)->default_value (20), "number of blocks.")
    ("angle-up", po::value<double > (&aup)->default_value (180.), "upper bond of the angle.")
    ("angle-low", po::value<double > (&alow)->default_value (-180.), "lower bond of the angle.")      
    ("input,f", po::value<string > (&ifile)->default_value ("vector.in"), "the input of vector.")
    ("output,o", po::value<string > (&ofile)->default_value ("vector.out"), "the output of vector.");
  
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

  unsigned nstate = setMap.size();

  double binSize = (aup - alow) / double(nbin);

  vector<double > cluster;
  load_cluster (ifile, cluster);

  if (nstate != cluster.size()){
    cerr << "set map " << nstate << " is not consistent with the cluster file " << cluster.size() << endl;
    return 1;
  }

  FILE * fp = fopen (ofile.c_str(), "w");
  if (fp == NULL){
    cerr << "cannot open file " << ofile << endl;
    return 1;
  }
  for (unsigned ii = 0; ii < nbin; ++ii){
    double phi = (ii+0.5) * binSize + alow;
    for (unsigned jj = 0; jj < nbin; ++jj){
      double psi = (jj+0.5) * binSize + alow;
      unsigned dihIndex = jj + ii * nbin;
      map<unsigned, unsigned> :: const_iterator it = mymap.find(dihIndex);
      if (it != mymap.end()){
	unsigned fileIndex = it -> second;
	fprintf (fp, "%f %f %f\n", phi, psi, cluster[fileIndex]);
      }
      else {
	fprintf (fp, "%f %f %f\n", phi, psi, 0.);
      }      
    }
    fprintf (fp, "\n");
  }
  fclose (fp);
  
  return 0;
}
