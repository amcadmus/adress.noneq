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
#include "Trajectory.h"
#include "BlockAverage.h"

#define MaxLineLength 2048

namespace po = boost::program_options;
using namespace std;

int main(int argc, char * argv[])
{
  std::string ifile;
  double qh, qo;
  float begin, end;
  float time_prec = .01;
  unsigned every, nDataBlock;
  double TT;
  double kB = 1.3806488e-23;
  double e0 = 8.854187817e-21;
  double ee = 1.602176565e-19;
  
  po::options_description desc ("Program of calculating the dielectric constant.\nAllow options");
  desc.add_options()
      ("help,h", "print this message")
      ("change-h,q",  po::value<double > (&qh)->default_value (0.417), "the charge of hydrogen atom")
      ("begin,b", po::value<float > (&begin)->default_value(0.f), "start time")
      ("end,e",   po::value<float > (&end  )->default_value(0.f), "end   time")
      ("every",   po::value<unsigned > (&every)->default_value(1), "every frame")
      ("temperature", po::value<double > (&TT)->default_value(300.), "Temperature")
      ("num-data-block",   po::value<unsigned > (&nDataBlock)->default_value(1), "number of data in each block")
      ("input,f",   po::value<std::string > (&ifile)->default_value ("traj.xtc"), "the input .xtc file");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  qo = - 2. * qh;
  
  int countread = 0;
  int realcountread = 0;

  XtcLoader tjl (ifile.c_str());
  BlockAverage_acc ba (nDataBlock);
  BlockAverage_acc bamx (nDataBlock);
  BlockAverage_acc bamy (nDataBlock);
  BlockAverage_acc bamz (nDataBlock);
  
  while (true == tjl.load()){
    countread ++;
    if ((countread-1) % every != 0) continue;
    float time = tjl.getTime();
    if (end != 0.f) {
      if (time < begin - time_prec){
        continue;
      }
      else if (time > end + time_prec) {
        break;
      } 
    }
    else {
      if (time < begin - time_prec) continue;
    }
    if ((realcountread++) % 100 == 0){
      printf ("# load frame at time: %.1f ps\r", time);
      fflush (stdout);
    }

    vector<vector<double > > frame;
    tjl.getFrame (frame);
    
    vector<double > moment (3, 0.);
    // we assume here the tip3p water 
    int nmol = int(frame.size()) / 3;
    for (int ii = 0; ii < nmol; ++ii){
      for (int dd = 0; dd < 3; ++dd){
	moment[dd] += frame[ii*3+0][dd] * qo;
	moment[dd] += frame[ii*3+1][dd] * qh;
	moment[dd] += frame[ii*3+2][dd] * qh;	
      }
    }

    bamx.deposite (moment[0]);
    bamy.deposite (moment[1]);
    bamz.deposite (moment[2]);
    double m2 = moment[0] * moment[0] + moment[1] * moment[1] + moment[2] * moment[2];
    ba.deposite (m2);
  }

  bamx.calculate ();
  bamy.calculate ();
  bamz.calculate ();
  ba.calculate ();
  double factor = 1.0 / (3. * tjl.getBox()[0] * tjl.getBox()[1] * tjl.getBox()[2] * kB * TT * e0) * ee * ee;

  printf ("# num data used: %d with %d blocks, %d data in each block. 60 percent confidence level\n",
	  ba.getNumDataUsed(), ba.getNumDataUsed() / nDataBlock, nDataBlock);
  printf ("# avg_M2 \t avg_M2_error\n");
  printf ("%e \t %e \n", ba.getAvg(), ba.getAvgError());
  printf ("# eps \t eps_error\n");
  printf ("%e \t %e \n",
	  1 + factor * ba.getAvg(), factor * ba.getAvgError());
  printf ("# avg_M\n");
  printf ("%e \t %e \t %e\n",
	  bamx.getAvg(),
	  bamy.getAvg(),
	  bamz.getAvg());
  printf ("# avg_M_error\n");
  printf ("%e \t %e \t %e\n",
	  bamx.getAvgError(),
	  bamy.getAvgError(),
	  bamz.getAvgError());

  return 0;
}
