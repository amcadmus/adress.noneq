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
  std::string ofile, ifile, isfile, idfile, opfile;
  double dt, period, begin, end, tau;
  unsigned ndataBlock;
  double maxRelError;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input,f",  po::value<std::string > (&ifile)->default_value ("traj.disc"), "the file of file names")
      ("input-dir,d", po::value<std::string > (&idfile)->default_value ("dir.name"), "the output of metastable propulation")
      ("input-largest-set,s", po::value<std::string > (&isfile)->default_value ("largestSet"), "the input file of largest set.")
      ("n-data-block,n", po::value<unsigned > (&ndataBlock)->default_value (1), "num data in each block.")
      ("max-rel-error,m", po::value<double > (&maxRelError)->default_value (2e-0), "maximum relative error.")
      ("dt,t", po::value<double > (&dt)->default_value (1.0), "time step of disc traj.")
      ("tau,a", po::value<double > (&tau)->default_value (1.0), "the lag time.")
      ("period,p", po::value<double > (&period)->default_value (40.0), "the period, in ps. should be multiples of dt")
      ("begin,b", po::value<double > (&begin)->default_value (0), "the begin of using data.")
      ("end,e", po::value<double > (&end)->default_value (0), "the end of using data.")
      ("output-init-prob", po::value<string > (&opfile)->default_value ("init.prob.out"), "the output of the initial probability.")
      ("output,o", po::value<string > (&ofile)->default_value ("tmatrix"), "the head of the output transition matrix.");
  

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  vector<int > setMap ;
  int tmpint;
  FILE * fp  = fopen (isfile.c_str(), "r");
  while (fscanf(fp, "%d", &tmpint) == 1){
    setMap.push_back (tmpint);
  }
  fclose (fp);
  map<unsigned,unsigned> mymap;
  for (unsigned ii = 0; ii < setMap.size(); ++ii){
    mymap[setMap[ii]] = ii;
  }
  // check a little bit
  for (unsigned ii = 0; ii < setMap.size(); ++ii){
    unsigned index = (mymap.find (setMap[ii])) -> second;
    if (setMap[index] != setMap[ii]){
      cerr << "error with map !" << endl;
      exit (1);
    }
  }  
  
  unsigned nstate = setMap.size();
  unsigned tauInt = unsigned( (tau + 0.5 * dt) / dt );
  unsigned beginInt = unsigned( (begin + 0.5 * dt) / dt );
  unsigned endInt = unsigned( (end + 0.5 * dt) / dt );
  unsigned periodInt = unsigned ( (period + 0.5 * dt) / dt );

  vector<vector<vector<BlockAverage_acc > > > tmatrix (periodInt); // periodInt x nstate x nstate
  for (unsigned ii = 0; ii < tmatrix.size(); ++ii){
    tmatrix[ii].resize(nstate);
    for (unsigned jj = 0; jj < nstate; ++jj){
      tmatrix[ii][jj].resize(nstate);
      for (unsigned kk = 0; kk < nstate; ++kk){
	tmatrix[ii][jj][kk].reinit (ndataBlock);
      }
    }
  }
  vector<BlockAverage_acc > initP (nstate);
  for (unsigned kk = 0; kk < nstate; ++kk){
    initP[kk].reinit (ndataBlock);
  }
  
  
  ifstream fpname (idfile.c_str());
  if (!fpname){
    std::cerr << "cannot open file " << idfile << std::endl;
    return 1;
  }
  char nameline [MaxLineLength];
  unsigned countFile = 0;
  
  while (fpname.getline(nameline, MaxLineLength)){
    if (nameline[0] == '#') continue;
    string filename = string(nameline) + string("/") + ifile;
    FILE *fp = fopen (filename.c_str(), "r");
    if (fp == NULL){
      std::cerr << "cannot open file " << filename << std::endl;
      return 1;
    }
    // cout << "reading file " << filename << endl;
    countFile ++;
    vector<unsigned > disc_traj;
    unsigned count_read = 0;
    unsigned tmpread;
    while (1 == fscanf(fp, "%d", &tmpread)){
      count_read ++;
      if (count_read <= beginInt) continue;
      if (endInt != 0 && count_read > endInt) break;
      // unsigned index = (mymap.find (tmpread)) -> second;
      // if (setMap[index] != tmpread){
      // 	cerr << "error with map !" << endl;
      // 	exit (1);
      // }
      disc_traj.push_back (tmpread);
    }
    // if (disc_traj.size() % periodInt != 0){
    //   cerr << "the size of disc traj is not a multiple of the period" << endl;
    //   exit (1);
    // }
    {
      unsigned myIdx = mymap.find(disc_traj[0]) -> second;
      for (unsigned kk = 0; kk < nstate; ++kk){
	if (myIdx == kk){
	  initP[kk].deposite (1.);
	}
	else {
	  initP[kk].deposite (0.);
	}
      }
    }
    for (unsigned ii = 0; ii < disc_traj.size() / periodInt; ++ii){
      for (unsigned jj = 0; jj < periodInt; ++jj){
	unsigned myPosi = ii * periodInt + jj;
	unsigned targetPosi = ii * periodInt + jj + tauInt;
	if (targetPosi < disc_traj.size()){
	  unsigned myIdx = mymap.find(disc_traj[myPosi]) -> second;
	  unsigned targetIdx = mymap.find(disc_traj[targetPosi]) -> second;
	  for (unsigned kk = 0; kk < nstate; ++kk){
	    if (kk == targetIdx){
	      tmatrix[jj][kk][myIdx].deposite(1.);
	    }
	    else {
	      tmatrix[jj][kk][myIdx].deposite(0.);
	    }
	  }
	}
      }
    }
    fclose (fp);
  }

  for (unsigned ii = 0; ii < tmatrix.size(); ++ii){
    tmatrix[ii].resize(nstate);
    for (unsigned jj = 0; jj < nstate; ++jj){
      tmatrix[ii][jj].resize(nstate);
      for (unsigned kk = 0; kk < nstate; ++kk){
	tmatrix[ii][jj][kk].calculate ();
      }
    }
  }
  for (unsigned kk = 0; kk < nstate; ++kk){
    initP[kk].calculate();
  }
  vector<vector<vector<double > > > cpTmatrix(tmatrix.size());
  for (unsigned ii = 0; ii < cpTmatrix.size(); ++ii){
    cpTmatrix[ii].resize(nstate);
    for (unsigned jj = 0; jj < nstate; ++jj){
      cpTmatrix[ii][jj].resize(nstate);
      for (unsigned kk = 0; kk < nstate; ++kk){
	cpTmatrix[ii][jj][kk] = tmatrix[ii][jj][kk].getAvg();
      }
    }
  }
  for (unsigned ii = 0; ii < cpTmatrix.size(); ++ii){
    for (unsigned kk = 0; kk < nstate; ++kk){
      double sum = 0;
      for (unsigned jj = 0; jj < nstate; ++jj){
	sum += cpTmatrix[ii][jj][kk];
      }
      if (sum != 1){
	if (sum == 0){
	  cpTmatrix[ii][kk][kk] = 1.;
	}
	else {
	  cerr << "sum is " << sum << " problematic" << endl;
	}
      }
    }
  }

  for (unsigned ii = 0; ii < tmatrix.size(); ++ii){
    char filename [MaxLineLength];
    sprintf (filename, "%s.%06d.out", ofile.c_str(), ii);
    FILE * fp = fopen (filename, "w");
    fprintf (fp, "DENSE	%d %d\n", nstate, nstate);
    double maxErr = 0.;
    double relErr = 0.;
    for (unsigned jj = 0; jj < nstate; ++jj){
      for (unsigned kk = 0; kk < nstate; ++kk){
	double value = cpTmatrix[ii][jj][kk];
	double error = tmatrix[ii][jj][kk].getAvgError();
	if (value != 0){
	  if (error / value >= maxRelError){
	    // cout << "set zero for count " << ii << " state "  << jj << " " << kk << " value " << value <<  " error " << error << " rel " << error / value  << endl;
	    value = 0.;
	    error = 0.;
	  }
	}
	fprintf (fp, "%.10e ", value);
	if (error > maxErr) {
	  maxErr = error ;
	  // cout << "find max: " << ii << " state "  << jj << " " << kk << " value " << value <<  " error " << error << " rel " << error / value  << endl;
	}
	if (error / value > relErr){
	  relErr = error / value;
	}
      }
      fprintf (fp, "\n");
    }
    fclose (fp);
    printf ("Time %f, max err %e, max rel err %e\n", dt * ii, maxErr, relErr);
  } 

  FILE * fpo = fopen (opfile.c_str(), "w");
  for (unsigned kk = 0; kk < nstate; ++kk){
    fprintf (fpo, "%e\n", initP[kk].getAvg());
  }
  fclose (fpo);
  
  return 0;
}
