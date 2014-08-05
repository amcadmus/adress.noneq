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
#include <ctime>
#include "StringSplit.h"
#include "BlockAverage.h"
#include "RandomGenerator.h"

#define MaxLineLength 65536

namespace po = boost::program_options;
using namespace std;

void read_tmatrix (const string & filename,
		   vector<vector<double > > & tmatrix,
		   unsigned & nstate)
{
  ifstream fpname (filename.c_str());
  if (!fpname){
    std::cerr << "cannot open file " << filename << std::endl;
    exit(1);
  }
  char valueline [MaxLineLength];
  int countLine = 0;
  while (fpname.getline(valueline, MaxLineLength)){
    vector<string > words;
    StringOperation::split (string(valueline), words);
    if (countLine == 0){
      nstate = atoi (words[2].c_str());
      tmatrix.resize(nstate);
      for (unsigned ii = 0; ii < nstate; ++ii){
	tmatrix[ii].resize(nstate);
      }
    }
    else {
      if (words.size() != nstate){
	cerr << "format error of tmatrix reading " << endl;
	exit (1);
      }
      for (unsigned ii = 0; ii < nstate; ++ii){
	tmatrix[countLine-1][ii] = atof(words[ii].c_str());
      }
    }
    countLine ++;
  }
}

void find_interval (const vector<double > & values,
		    const double & value,
		    int & start,
		    int & end)
{
  start = 0;
  end = values.size() - 1;
  while (end - start > 1){
    int mid = (start + end)/2;
    if (value > values[mid]){
      start = mid;
    }
    else {
      end = mid;
    }
  }
}

int main(int argc, char * argv[])
{
  std::string ofile, ifile, isfile;
  double nsteps;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input-largest-set,s", po::value<std::string > (&isfile)->default_value ("largestSet"), "the input file of largest set.")
      ("input-m,f", po::value<string > (&ifile)->default_value ("floque.B.out"), "the input transition matrix.")
      ("num-step,n", po::value<double > (&nsteps)->default_value (100), "the number of steps of the traj.")
      ("output,o", po::value<string > (&ofile)->default_value ("traj.out"), "the output of traj.");
  
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

  vector<vector<double > > tmatrix;
  unsigned nstate1;
  read_tmatrix (ifile, tmatrix, nstate1);

  if (nstate1 != nstate){
    cerr << "the largest set file is not consistent with the transition matrix" << endl;
    return 1;
  }

  vector<vector<double > > sumMatrix (tmatrix.size());
  for (unsigned ii = 0; ii < sumMatrix.size(); ++ii){
    sumMatrix[ii].resize (tmatrix[ii].size());
    double sum = 0.;
    for (unsigned jj = 0; jj < sumMatrix[ii].size(); ++jj){
      sumMatrix[ii][jj] = sum;
      sum += tmatrix[ii][jj];
    }
    if (fabs(sum - 1.) > 1e-10){
      cerr << "transition matrix should be row sum 1" << endl;
      return 1;
    }
    sumMatrix[ii].push_back(1.);
  }

  RandomGenerator_MT19937::init_genrand ((time(0)));
  unsigned state = unsigned(RandomGenerator_MT19937::genrand_real2() * nstate);
  if (state >= nstate){
    cerr << "wrong initial state!" << endl;
    return 1;
  }

  FILE * fp = fopen (ofile.c_str(), "w");
  if (fp == NULL){
    cerr << "cannot open file " << ofile << endl;
    return 1;
  }
  for (double tt = 0.; tt < nsteps; tt+=1.){
    int begin, end;
    double myrand = RandomGenerator_MT19937::genrand_real2();
    find_interval (sumMatrix[state], myrand, begin, end);
    if (myrand == sumMatrix[state][begin] || myrand == sumMatrix[state][end]){
      tt -= 1.;
      continue;
    }
    else {
      state = begin;
    }
    fprintf (fp, "%d\n", setMap[state]);
  }
  fclose (fp);
  
  return 0;
}
