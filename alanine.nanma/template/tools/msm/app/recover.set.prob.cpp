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
  std::string ofile, ifile, ipfile, isfile;
  double aup, alow;
  unsigned nbin, nsample;

  double setC_psi_b = 13,  setC_psi_e = 128;
  double setC_phi_b = -180,setC_phi_e = 180;
  
  double setA1_psi_b =-134, setA1_psi_e = 13;
  double setA1_phi_b =-125, setA1_phi_e = 74;
  double setA2_psi_b = 128, setA2_psi_e =-134;
  double setA2_phi_b =-125, setA2_phi_e = 74;

  double setB1_psi_b =-110, setB1_psi_e = 13;
  double setB1_phi_b = 74,  setB1_phi_e =-125;
  double setB2_psi_b = 128, setB2_psi_e =-110;
  double setB2_phi_b = 74,  setB2_phi_e =-125;
  
  vector<MetastableSet > sets;
  // sets.push_back (MetastableSet(setA_psi_b, setA_psi_e, setA_phi_b, setA_phi_e));
  // sets.push_back (MetastableSet(setB_psi_b, setB_psi_e, setB_phi_b, setB_phi_e));
  sets.push_back (MetastableSet(setA1_psi_b, setA1_psi_e, setA1_phi_b, setA1_phi_e));
  sets.push_back (MetastableSet(setA2_psi_b, setA2_psi_e, setA2_phi_b, setA2_phi_e));
  sets.push_back (MetastableSet(setB1_psi_b, setB1_psi_e, setB1_phi_b, setB1_phi_e));
  sets.push_back (MetastableSet(setB2_psi_b, setB2_psi_e, setB2_phi_b, setB2_phi_e));
  sets.push_back (MetastableSet(setC_psi_b, setC_psi_e, setC_phi_b, setC_phi_e));
  unsigned nsets = sets.size();

  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input-prob", po::value<string > (&ipfile)->default_value ("cg.prob.out"), "the input probabilities.")
      ("input-largest-set,s", po::value<std::string > (&isfile)->default_value ("largestSet"), "the input file of largest set.")
      ("num-bin,n", po::value<unsigned > (&nbin)->default_value (20), "number of blocks.")
      ("num-sample", po::value<unsigned > (&nsample)->default_value (100), "number of sampling on each direction.")
      ("angle-up", po::value<double > (&aup)->default_value (180.), "upper bond of the angle.")
      ("angle-low", po::value<double > (&alow)->default_value (-180.), "lower bond of the angle.")
      ("output,o", po::value<string > (&ofile)->default_value ("prob.out"), "the output of prob.");
  
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

  double binSize = (aup - alow) / double(nbin);
  vector<vector<double > > weights (nbin * nbin);
  double sampleBinSize = binSize / double (nsample);
  
  for (unsigned ii = 0; ii < nbin; ++ii){
    double valuexx = ii * binSize + alow;
    for (unsigned jj = 0; jj < nbin; ++jj){
      double valueyy = jj * binSize + alow;
      unsigned index = ii * nbin + jj;
      weights[index].clear();
      weights[index].resize(nsets, 0.);
      vector<double > counts (nsets);
      for (unsigned mm = 0; mm < nsample; ++mm){
	double posixx = valuexx + mm * sampleBinSize;
	for (unsigned nn = 0; nn < nsample; ++nn){
	  double posiyy = valueyy + nn * sampleBinSize;
	  for (unsigned kk = 0; kk < nsets; ++kk){
	    if (sets[kk].inSet(posixx, posiyy)){
	      counts[kk] += 1.;
	      break;
	    }
	  }
	}
      }
      double sum = 0.;
      for (unsigned kk = 0; kk < nsets; ++kk){
	weights[index][kk] = counts[kk] / double(nsample * nsample);
	sum += weights[index][kk];
      }
      if (sum != 1){
	cerr << "wrong sum " << endl;
	return 1;
      }
    }
  }

  ifstream traj (ipfile.c_str());
  if (! traj){
    cerr << "cannot open angle file " << ipfile << endl;
    return 1;
  }
  FILE * fpo ;
  if ((fpo = fopen(ofile.c_str(), "w")) == NULL) {
    std::cerr << "ERROR: errno=" << 1 << " opening file"
    << " at " << __FILE__ << ":" << __LINE__
    << std::endl << std::flush;
    exit(1);
  }
  
  char valueline [MaxLineLength];
  while (traj.getline(valueline, MaxLineLength)){
    if (valueline[0] == '#' || valueline[0] == '@'){
      continue;
    }
    vector<string > words;
    StringOperation::split (string(valueline), words);
    if (words.size() != nstate+1){
      cerr << "wrong format of cg traj " << endl;
      return 1;
    }
    double time = atof(words[0].c_str());
    fprintf (fpo, "%f\t", time);
    vector<double > probs (nsets, 0.);
    for (unsigned ii = 0; ii < nstate; ++ii){
      double value = atof(words[ii+1].c_str());
      unsigned index = setMap[ii];
      for (unsigned jj = 0; jj < nsets; ++jj){
	probs[jj] += weights[index][jj] * value;
      }
    }
    for (unsigned jj = 0; jj < nsets; ++jj){
      fprintf (fpo, "%e ", probs[jj]);
    }
    fprintf (fpo, "\n");
  }

  fclose (fpo);
  
  return 0;
}
