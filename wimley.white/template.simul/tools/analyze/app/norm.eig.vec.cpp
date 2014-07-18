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
  std::string rfile, ifile, ofile;
  unsigned col;
  
  po::options_description desc ("Normalize the eigen vector.\nAllow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input-ref,r", po::value<std::string > (&rfile)->default_value ("eig.vec.ref"), "the input reference eigen vector.")
      ("columns,c", po::value<unsigned > (&col)->default_value (2), "the column to used in the reference eigen vector file.")
      ("input,f", po::value<std::string > (&ifile)->default_value ("eig.vec.in"), "the input eigen vector.")
      ("output,o", po::value<std::string > (&ofile)->default_value ("eig.vec.out"), "the output file.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  ifstream rdata (rfile.c_str());
  ifstream idata (ifile.c_str());
  if (!rdata){
    cerr << "cannot open angle file " << rfile << endl;
    return 1;
  }
  if (!idata){
    cerr << "cannot open angle file " << ifile << endl;
    return 1;
  }

  char valueline [MaxLineLength];
  vector<string > words;

  vector<double > ref;
  vector<double > value;
  while (rdata.getline(valueline, MaxLineLength)){
    if (valueline[0] == '#' || valueline[0] == '@'){
      continue;
    }
    vector<string > words;
    StringOperation::split (string(valueline), words);
    if (words.size() < unsigned(col)){
      cerr << "wrong file format of " << ifile << endl;
      exit (1);
    }
    ref.push_back(atof(words[col-1].c_str()));
  }
  col=1;
  while (idata.getline(valueline, MaxLineLength)){
    if (valueline[0] == '#' || valueline[0] == '@'){
      continue;
    }
    vector<string > words;
    StringOperation::split (string(valueline), words);
    if (words.size() < unsigned(col)){
      cerr << "wrong file format of " << ifile << endl;
      exit (1);
    }
    value.push_back(atof(words[col-1].c_str()));
  }
  
    
  if (ref.size() != value.size()){
    cerr << "the eigen vector files should be consistent" << endl;
    return 1;
  }
  
  double sump = 0;
  double summ = 0;

  for (unsigned ii = 0; ii < ref.size(); ++ii){
    sump += (ref[ii] + value[ii]) * (ref[ii] + value[ii]);
    summ += (ref[ii] - value[ii]) * (ref[ii] - value[ii]);
  }
  // cout << "sump " << sump << endl;
  // cout << "summ " << summ << endl;

  if (sump < summ){
    for (unsigned ii = 0; ii < value.size(); ++ii){
      value[ii] = -value[ii];
    }
  }
  
  FILE * fp = fopen (ofile.c_str(), "w");
  for (unsigned ii = 0; ii < value.size(); ++ii){
    fprintf (fp, "%e\n", value[ii]);
  }
  fclose (fp);
  
  return 0;
}
