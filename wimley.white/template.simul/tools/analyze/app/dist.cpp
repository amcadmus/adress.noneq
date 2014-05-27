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

#define MaxLineLength 2048

namespace po = boost::program_options;
using namespace std;

int main(int argc, char * argv[])
{
  std::string ofile, ifile;
  unsigned column, nBin, nDataInBlock;
  double aup, alow;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input-list-files,f", po::value<std::string > (&ifile)->default_value ("angaver.xvg"), "the input files of dihedral angles.")
      ("column,c", po::value<unsigned > (&column)->default_value (2), "the column used.")
      ("num-data-block", po::value<unsigned > (&nDataInBlock)->default_value (320), "number data in blocks.")
      ("num-bins,n", po::value<unsigned > (&nBin)->default_value (72), "number bins for distribution.")
      ("angle-up", po::value<double > (&aup)->default_value (180.), "upper bond of the angle.")
      ("angle-low", po::value<double > (&alow)->default_value (-180.), "lower bond of the angle.")
      ("output,o", po::value<std::string > (&ofile)->default_value ("dist.out"), "the output of the discrete trajectory");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  double binSize = (aup - alow) / double(nBin);
  double binSizeI = 1./binSize;
  
  vector<BlockAverage_acc> avgs (nBin);
  for (unsigned ii = 0; ii < nBin; ++ii){
    avgs[ii].reinit (nDataInBlock);
  }
  
  vector<string > words;
  StringOperation::split (ifile, words);

  // int countTot = 0;

  for (unsigned ii = 0; ii < words.size(); ++ii){
    ifstream data (words[ii].c_str());
    if (!data){
      cerr << "cannot open angle file " << words[ii] << endl;
      return 1;
    }
    cout << "calculate with dih traj " << words[ii] << endl;
    char valueline [MaxLineLength];  
    while (data.getline(valueline, MaxLineLength)){
      if (valueline[0] == '#' || valueline[0] == '@'){
	continue;
      }
      vector<string > words;
      StringOperation::split (string(valueline), words);
      if (words.size() < column) {
	cerr << "wrong file format of " << ifile << endl;
	exit (1);
      }
      double value = atof(words[column-1].c_str());
      int indexes = (value - alow) * binSizeI;
      if (indexes < 0) indexes += int(nBin);
      else if (indexes >= int(nBin)) indexes -= int(nBin);
      for (unsigned jj = 0; jj < nBin; ++jj){
	if (int(jj) == indexes){
	  avgs[jj].deposite (1.);
	}
	else {
	  avgs[jj].deposite (0.);
	}
      }
      // countTot ++;
    }
  }

  for (unsigned ii = 0; ii < nBin; ++ii) {
    avgs[ii].calculate ();
  }

  FILE * fp = fopen (ofile.c_str(), "w");
  if (fp == NULL){
    cerr << "cannot open file " << ofile << endl;
    return 1;
  }

  for (unsigned ii = 0; ii < nBin; ++ii) {
    double xx = alow + (ii+0.5) * binSize;
    double vv = avgs[ii].getAvg() / double(1) * binSizeI;
    double ee = avgs[ii].getAvgError() / double(1) * binSizeI;
    fprintf (fp, "%f %e %e\n", xx, vv, ee);
  }

  return 0;
}


