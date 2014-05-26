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

int conv_multi_single (const vector<int > & indexes,
		       const int & nBin)
{
  int result = 0;
  for (unsigned ii = 0; ii < indexes.size(); ++ii){
    result = result * nBin + indexes[ii];
  }
  return result;
}


int main(int argc, char * argv[])
{
  std::string ofile, ifile;
  unsigned column, nBin;
  double aup, alow;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input-dih-list,f", po::value<std::string > (&ifile)->default_value ("angaver.xvg"), "the input files of dihedral angles.")
      ("column,c", po::value<unsigned > (&column)->default_value (2), "the column used.")
      ("num-bin,n", po::value<unsigned > (&nBin)->default_value (20), "number of blocks.")
      ("angle-up", po::value<double > (&aup)->default_value (180.), "upper bond of the angle.")
      ("angle-low", po::value<double > (&alow)->default_value (-180.), "lower bond of the angle.")
      ("output,o", po::value<std::string > (&ofile)->default_value ("dih.disctraj"), "the output of the discrete trajectory");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  vector<string > words;
  StringOperation::split (ifile, words);
  unsigned ndih = words.size();

  vector<ifstream * > ffiles;
  for (unsigned ii = 0; ii < ndih; ++ii){
    ifstream * tmp = new ifstream;
    tmp->open (words[ii].c_str());
    ffiles.push_back (tmp);
    if (!(*ffiles.back())){
      cerr << "cannot open angle file " << words[ii] << endl;
      return 1;
    }
    else {
      cout << "use file " << words[ii] << endl;
    }
  }

  double binSize = (aup - alow) / double(nBin);
  double binSizeI = 1./binSize;
  
  char valueline [MaxLineLength];
  FILE * fp = fopen (ofile.c_str(), "w");
  if (fp == NULL){
    cerr << "cannot open file " << ofile << endl;
    return 1;
  }
  
  while (1) {
    bool read = true;
    vector<int > indexes (ndih);
    for (unsigned ii = 0; ii < ndih; ++ii){
      if (! (ffiles[ii]->getline(valueline, MaxLineLength)) ){
	read = false;
      }
      else {
	StringOperation::split (string(valueline), words);
	if (words.size() < column) {
	  cerr << "wrong file format of " << ifile << endl;
	  exit (1);
	}
	double value = atof(words[column-1].c_str());
	indexes[ii] = (value - alow) * binSizeI;
	if (indexes[ii] < 0) indexes[ii] += int(nBin);
	else if (indexes[ii] >= int(nBin)) indexes[ii] -= int(nBin);
      }
    }
    if (!read){
      break;
    }
    else {
      fprintf (fp, "%d\n",  conv_multi_single (indexes, nBin));
    }
  }  
  
  return 0;
}
