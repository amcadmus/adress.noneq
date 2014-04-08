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
#include "StringSplit.h"
#include "BlockAverage.h"

#define MaxLineLength 2048

namespace po = boost::program_options;
using namespace std;

int main(int argc, char * argv[])
{
  std::string ofile, ifile;
  unsigned column, nBlock;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input,f", po::value<std::string > (&ifile)->default_value ("data"), "the input file.")
      ("column,c", po::value<unsigned > (&column)->default_value (1), "the column used.")
      ("num-block,n", po::value<unsigned > (&nBlock)->default_value (20), "number of blocks.")
      ("output,o", po::value<std::string > (&ofile)->default_value ("table.xvg"), "the output of the table");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  ifstream data (ifile.c_str());
  if (!data){
    cerr << "cannot open angle file " << data << endl;
    return 1;
  }

  vector<double > deposite;
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
    deposite.push_back (value);
  }

  BlockAverage ba;
  ba.processData (deposite, nBlock);
  
  printf ("# num data used: %d with %d blocks. 60 percent confidence level\n", ba.getNumDataUsed(), nBlock);
  printf ("# avg \t avg_err \t var \t var_err\n");
  printf ("%e \t %e \t %e \t %e\n",
	  ba.getAvg(), ba.getAvgError(),
	  ba.getVar(), ba.getVarError()
      );
  
  return 0;
}