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
		   vector<int > & cluster)
{
  cluster.clear ();
  FILE * fp = fopen (ifile.c_str(), "r");
  if (fp == NULL){
    cerr << "cannot open angle file " << ifile << endl;
    exit( 1);
  }
  int tmp;
  while (1 == fscanf(fp, "%d", &tmp) ){
    cluster.push_back (tmp);
  }
  fclose(fp);
}

void load_fz_cluster (const string &ifile,
		      vector<vector<double > > & cluster)
{
  char valueline [MaxLineLength];
  vector<string > words;
  ifstream ifp (ifile.c_str());
  if (! ifp){
    cerr << "cannot open angle file " << ifile << endl;
    exit( 1);
  }
  while (ifp.getline(valueline, MaxLineLength)){
    if (valueline[0] == '#' || valueline[0] == '@'){
      continue;
    }
    StringOperation::split (string(valueline), words);
    cluster.resize (words.size());
    for (unsigned ii = 0; ii < words.size(); ++ii){
      cluster[ii].push_back (atof(words[ii].c_str()));
    }
  }
}

int main(int argc, char * argv[])
{
  std::string ofile, ofzfile, ifile, ifzfile, isfile;
  double aup, alow;
  unsigned nbin;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "Convert set to dihedral")
      ("input-largest-set,s", po::value<std::string > (&isfile)->default_value ("largestSet"), "the input file of largest set.")
      ("num-bin,n", po::value<unsigned > (&nbin)->default_value (20), "number of blocks.")
      ("angle-up", po::value<double > (&aup)->default_value (180.), "upper bond of the angle.")
      ("angle-low", po::value<double > (&alow)->default_value (-180.), "lower bond of the angle.")      
      ("input,f", po::value<string > (&ifile)->default_value ("cluster.dat"), "the input of cluster.")
      ("input-fuzzy", po::value<string > (&ifzfile)->default_value ("cluster.fuzzy.dat"), "the input of fuzzy cluster.")
      ("output,o", po::value<string > (&ofile)->default_value ("cluster.out"), "the output of cluster.")
      ("output-fuzzy,o", po::value<string > (&ofzfile)->default_value ("cluster.fuzzy.out"), "the output of fuzzy cluster.");
  
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

  vector<int > cluster;
  vector<vector<double > > fzcluster;
  load_cluster (ifile, cluster);
  load_fz_cluster (ifzfile, fzcluster);

  if (nstate != cluster.size()){
    cerr << "set map is not consistent with the cluster file" << endl;
    return 1;
  }
  if (nstate != fzcluster.size()){
    cerr << "set map is not consistent with the fuzzy cluster file" << endl;
    return 1;
  }

  FILE * fp = fopen (ofile.c_str(), "w");
  FILE * fpfz = fopen (ofzfile.c_str(), "w");
  if (fp == NULL){
    cerr << "cannot open file " << ofile << endl;
    return 1;
  }
  if (fpfz == NULL){
    cerr << "cannot open file " << ofzfile << endl;
    return 1;
  }
  for (unsigned ii = 0; ii < nbin; ++ii){
    double phi = ii * binSize + alow;
    for (unsigned jj = 0; jj < nbin; ++jj){
      double psi = jj * binSize + alow;
      unsigned dihIndex = jj + ii * nbin;
      map<unsigned, unsigned> :: const_iterator it = mymap.find(dihIndex);
      if (it != mymap.end()){
	unsigned fileIndex = it -> second;
	unsigned mostlike = 0;
	double currentvalue = 0.;
	fprintf (fpfz, "%f %f ", phi, psi);
	for (unsigned kk = 0; kk < fzcluster.size(); ++kk){
	  fprintf(fpfz, "%f ", fzcluster[kk][fileIndex]);
	  if (fzcluster[kk][fileIndex] > currentvalue){
	    currentvalue = fzcluster[kk][fileIndex];
	    mostlike = kk;
	  }
	}
	fprintf (fpfz, "\n");
	fprintf (fp, "%f %f %d %d\n", phi, psi, cluster[fileIndex], mostlike);
      }
      else {
	fprintf (fpfz, "%f %f ", phi, psi);
	for (unsigned kk = 0; kk < fzcluster.size(); ++kk){
	  fprintf(fpfz, "%f ", 0.);
	}
	fprintf (fpfz, "\n");
	fprintf (fp, "%f %f %d %d\n", phi, psi, 0, 0);
      }      
    }
  }
  fclose (fp);
  fclose (fpfz);
  
  return 0;
}
