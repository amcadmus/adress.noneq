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
  std::string ifile;
  unsigned columnt, columnd, columne;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input,f", po::value<std::string > (&ifile)->default_value ("data"), "the input file.")
      ("column-t,t", po::value<unsigned > (&columnt)->default_value (1), "the column of time used.")
      ("column-d,d", po::value<unsigned > (&columnd)->default_value (2), "the column of data used.")
      ("column-e,e", po::value<unsigned > (&columne)->default_value (0), "the column of errorused.");
  

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  ifstream dataf (ifile.c_str());
  if (!dataf){
    cerr << "cannot open angle file " << ifile << endl;
    return 1;
  }

  vector<double > times;
  vector<double > data;
  vector<double > errors;
  char valueline [MaxLineLength];
  
  while (dataf.getline(valueline, MaxLineLength)){
    if (valueline[0] == '#' || valueline[0] == '@'){
      continue;
    }
    vector<string > words;
    StringOperation::split (string(valueline), words);
    if (words.size() < columnt || words.size() < columnd) {
      cerr << "wrong file format of " << ifile << endl;
      exit (1);
    }
    if (columne != 0 && words.size() < columne){
      cerr << "wrong file format of " << ifile << endl;
      exit (1);
    }    
    times.push_back( atof(words[columnt-1].c_str()) );
    data.push_back( atof(words[columnd-1].c_str()) );
    if (columne != 0){
      errors.push_back( atof(words[columnd-1].c_str()) );
    }
  }

  double sumd = 0;
  double sume = 0;
  for (unsigned ii = 0; ii < times.size()-1; ++ii){
    double dt = times[ii+1] - times[ii];
    sumd += 0.5 * dt * (data[ii+1] + data[ii]);
    printf ("%f \t %e \t", times[ii+1], sumd);
    if (!errors.empty()){
      sume += 0.5 * dt * data[ii+1] * 0.5 * dt * data[ii+1];
      if (ii != 0){
	sume -= 0.5 * dt * data[ii] * 0.5 * dt * data[ii];
	sume += 1.0 * dt * data[ii] * 1.0 * dt * data[ii];
      }
      else {
        sume += 0.5 * dt * data[ii] * 0.5 * dt * data[ii];
      }      
      printf ("%e", sume);
    }
    printf ("\n");
  }

  return 0;
}
