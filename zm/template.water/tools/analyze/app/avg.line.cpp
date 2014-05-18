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
  std::string ifile, ofile, string_cols;
  unsigned nDataBlock;
  
  po::options_description desc ("Average the elements on a line.\nAllow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input,f", po::value<std::string > (&ifile)->default_value ("data.in"), "the input file.")
      ("columns,c", po::value<std::string > (&string_cols)->default_value (string("")), "the columns used.")
      ("num-block,n", po::value<unsigned > (&nDataBlock)->default_value (1), "number of data in each block.")
      ("output,o", po::value<std::string > (&ofile)->default_value ("avg.out"), "the output file.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  ifstream data (ifile.c_str());
  if (!data){
    cerr << "cannot open angle file " << ifile << endl;
    return 1;
  }

  char valueline [MaxLineLength];
  vector<int > cols;
  vector<string > words;
  StringOperation::split (string_cols, words);
  for (unsigned ii = 0; ii < words.size(); ++ii){
    cols.push_back (atoi(words[ii].c_str()));
  }
  if (cols.empty()) {
    cerr << "empty columns, return" << endl;
    return 1;
  }

  FILE * fp = fopen (ofile.c_str(), "w");
  
  while (data.getline(valueline, MaxLineLength)){
    if (valueline[0] == '#' || valueline[0] == '@'){
      continue;
    }
    BlockAverage_acc acc (nDataBlock);
    vector<string > words;
    StringOperation::split (string(valueline), words);
    for (unsigned ii = 0; ii < cols.size(); ++ii){
      if (words.size() < unsigned(cols[ii])){
	cerr << "wrong file format of " << ifile << endl;
	exit (1);
      }
      acc.deposite (atof(words[cols[ii]-1].c_str()));
    }
    acc.calculate();
    fprintf (fp, "%f %f\n", acc.getAvg(), acc.getAvgError());
  }
  
  return 0;
}
