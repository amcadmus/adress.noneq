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

void apply_trans (const vector<vector<double > > & tmatrix,
		  const vector<double > & pin,
		  vector<double > & pout)
{
  pout.resize(tmatrix.size());
  for (unsigned ii = 0; ii < tmatrix.size(); ++ii){
    pout[ii] = 0.;
    for (unsigned jj = 0; jj < tmatrix[ii].size(); ++jj){
      pout[ii] += tmatrix[ii][jj] * pin[jj];
    }
  }
}


int main(int argc, char * argv[])
{
  std::string ofile, ifile, ipfile, isfile;
  double dt, period, end;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input,f", po::value<string > (&ifile)->default_value ("tmatrix"), "the head of the input transition matrix.")
      ("input-prob", po::value<string > (&ipfile)->default_value ("init.prob.out"), "the input probability.")
      ("input-largest-set,s", po::value<std::string > (&isfile)->default_value ("largestSet"), "the input file of largest set.")
      ("dt,t", po::value<double > (&dt)->default_value (1.0), "time step of disc traj.")
      ("period,p", po::value<double > (&period)->default_value (40.0), "the period, in ps. should be multiples of dt")
      ("end,e", po::value<double > (&end)->default_value (400.), "the end of using data.")
      ("output,o", po::value<string > (&ofile)->default_value ("prob.out"), "the output of prob.");
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }
  
  unsigned nstate;
  unsigned endInt = unsigned( (end + 0.5 * dt) / dt );
  unsigned periodInt = unsigned ( (period + 0.5 * dt) / dt );

  vector<vector<vector<double > > > tmatrix (periodInt); // periodInt x nstate x nstate

  for (unsigned ii = 0; ii < periodInt; ++ii){
    char filename [MaxLineLength];
    sprintf (filename, "%s.%06d.out", ifile.c_str(), ii);
    read_tmatrix (filename, tmatrix[ii], nstate);
  }

  vector<double > init_p ;
  vector<int > setMap ;
  map<unsigned,unsigned> mymap;
  {
    vector<double > init_p_tmp ;
    FILE * fp = fopen (ipfile.c_str(), "r");
    if (fp == NULL){
      cerr << "cannot open file " << ipfile << endl;
      return 1;
    }
    double tmpp;
    while (1 == fscanf (fp, "%lf", &tmpp)){
      init_p_tmp.push_back (tmpp);
    }
    fclose (fp);
    int tmpint;
    fp  = fopen (isfile.c_str(), "r");
    if (fp == NULL){
      cerr << "cannot open file " << isfile << endl;
      return 1;
    }
    while (fscanf(fp, "%d", &tmpint) == 1){
      setMap.push_back (tmpint);
    }
    fclose (fp);
    if (setMap.size() != nstate){
      cerr << "largest set should be the same as the dof of tmatrix" << endl;
      return 1;
    }
    init_p.resize (nstate);
    for (unsigned ii = 0; ii < nstate; ++ii){
      init_p[ii] = init_p_tmp[setMap[ii]];
    }
    for (unsigned ii = 0; ii < setMap.size(); ++ii){
      mymap[setMap[ii]] = ii;
    }
  }  


  vector<double > pcur = init_p;
  for (unsigned ii = 0; ii < endInt+1; ++ii){
    printf ("%f ", ii * dt);
    for (unsigned jj = 0; jj < nstate; ++jj){
      printf ("%e ", pcur[jj]);
    }
    printf ("\n");
    unsigned posi = ii % periodInt;
    vector <double > pnew;
    apply_trans (tmatrix[posi], pcur, pnew);
    pcur = pnew;
  }
  
  return 0;
}
