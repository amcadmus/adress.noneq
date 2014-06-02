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
  std::string ifile1, ifile2;
  unsigned column1,column2, nBlock, every;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input-1", po::value<std::string > (&ifile1)->default_value ("data1"), "the input file.")
      ("column-1", po::value<unsigned > (&column1)->default_value (1), "the column used.")
      ("input-2", po::value<std::string > (&ifile2)->default_value ("data2"), "the input file.")
      ("column-2", po::value<unsigned > (&column2)->default_value (1), "the column used.")
      ("num-block,n", po::value<unsigned > (&nBlock)->default_value (20), "number of blocks.")
      ("every", po::value<unsigned > (&every)->default_value (1), "avery number of data.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  vector<double > deposite1;
  vector<double > deposite2;
  vector<double > values;
  char valueline [MaxLineLength];
  int count = 0;
  
  ifstream data1 (ifile1.c_str());
  if (!data1){
    cerr << "cannot open angle file " << ifile1 << endl;
    return 1;
  }
  while (data1.getline(valueline, MaxLineLength)){
    if (valueline[0] == '#' || valueline[0] == '@'){
      continue;
    }
    vector<string > words;
    StringOperation::split (string(valueline), words);
    if (words.size() < column1) {
      cerr << "wrong file format of " << ifile1 << endl;
      exit (1);
    }
    if (count++ % every == 0){
      double value = atof(words[column1-1].c_str());
      deposite1.push_back (value);
    }
  }
  ifstream data2 (ifile2.c_str());
  if (!data2){
    cerr << "cannot open angle file " << ifile2 << endl;
    return 1;
  }
  while (data2.getline(valueline, MaxLineLength)){
    if (valueline[0] == '#' || valueline[0] == '@'){
      continue;
    }
    vector<string > words;
    StringOperation::split (string(valueline), words);
    if (words.size() < column2) {
      cerr << "wrong file format of " << ifile2 << endl;
      exit (1);
    }
    if (count++ % every == 0){
      double value = atof(words[column2-1].c_str());
      deposite2.push_back (value);
    }
  }

  if (deposite1.size() != deposite2.size()){
    cerr << "the number of data in files does not match, do nothing" << endl;
    return 1;
  }

  BlockAverage ba1, ba2;
  ba1.processData (deposite1, nBlock);
  ba2.processData (deposite2, nBlock);
  for (unsigned ii = 0; ii < deposite1.size(); ++ii){
    values.push_back ((deposite1[ii] - ba1.getAvg()) * (deposite2[ii] - ba2.getAvg()));
  }
  BlockAverage ba;
  ba.processData (values, nBlock);
  
  printf ("# num data used: %d with %d blocks. 60 percent confidence level\n", ba.getNumDataUsed(), nBlock);
  printf ("# avg \t avg_err \t var \t var_err\n");
  printf ("%e \t %e\n",
	  ba.getAvg(), ba.getAvgError()
      );
  
  return 0;
}
