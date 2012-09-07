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

#include "Analyzer.h"
#include "WaveCalculator.h"
#include <boost/program_options.hpp>
// #include "StringSplit.h"

namespace po = boost::program_options;

void readTop (const std::string & file,
	      TopInfo & info)
{
  FILE * fp = fopen (file.c_str(), "r");
  if (fp == NULL){
    std::cout << "cannot open file " << file << std::endl;
    return;
  }
  fscanf (fp, "%d %d", &(info.numAtomOnCh4), &(info.comIndexCh4));
  fscanf (fp, "%d", &(info.numAtomOnH2o));
  fscanf (fp, "%d %d %d", &(info.OIndexH2o), &(info.H1IndexH2o), &(info.H2IndexH2o));
  fclose (fp);
}


int main(int argc, char * argv[])
{
  unsigned numBlock;
  float begin, end;
  std::string ifile, ofile, tfile;
  float time_prec = .01;
  TopInfo info;
  double refh;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("begin,b", po::value<float > (&begin)->default_value(0.f), "start time")
      ("end,e",   po::value<float > (&end  )->default_value(0.f), "end   time")
      ("block,n", po::value<unsigned > (&numBlock)->default_value(20), "number of block for averaging")
      ("refh,r", po::value<double > (&refh)->default_value(.1), "size of bin")
      ("top-file,t",po::value<std::string > (&tfile)->default_value ("mytop"), "topolgy of the system")
      ("output,o",  po::value<std::string > (&ofile)->default_value ("density.wave.dat"), "the output of count of h-bond")
      ("input,f",   po::value<std::string > (&ifile)->default_value ("traj.trr"), "the input .xtc file");
      
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }
  if (vm.count("top-file")){
    readTop (tfile, info);
  }
  
  std::cout << "###################################################" << std::endl;
  std::cout << "# begin->end: " << begin << " " << end << std::endl;
  std::cout << "# block: " << numBlock << std::endl;
  std::cout << "# top file: " << tfile << std::endl;
  std::cout << "###################################################" << std::endl;  

  TrajLoader_trr tjl (ifile.c_str(), info);

  std::vector<std::vector<ValueType > > ch4xx;
  std::vector<std::vector<ValueType > > ch4vv;
  std::vector<std::vector<ValueType > > ch4ff;
  std::vector<std::vector<ValueType > > h2o;
  std::vector<std::vector<ValueType > > h2ovv;
  std::vector<std::vector<ValueType > > h2off;

  std::vector<std::vector<double > > com;
  std::vector<double > mass (3);
  std::vector<double > tmass;
  mass[info.OIndexH2o] = 16;
  mass[info.H1IndexH2o] = 1;
  mass[info.H2IndexH2o] = 1;
  double totalmass = mass[info.OIndexH2o] + mass[info.H1IndexH2o] + mass[info.H2IndexH2o];
  double boxSize[3];
  boxSize[0] = tjl.getBox().x;
  boxSize[1] = tjl.getBox().y;
  boxSize[2] = tjl.getBox().z;
  

  int countread = 0;

  DensityWave dw;
  dw.reinit (refh, tjl.getBox());
  dw.initFile (ofile.c_str());

  while (true == tjl.load()){
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
    if (countread++ % 1 == 0){
      // printf ("# load frame at time: %.1f ps\r", time);
      fflush (stdout);
    }

    tjl.formCoords (ch4xx, ch4vv, ch4ff, h2o, h2ovv, h2off);

    // cal coms
    {
      com.resize (h2o.size() / info.numAtomOnH2o);
      tmass.resize (h2o.size() / info.numAtomOnH2o, totalmass);
      for (unsigned ii = 0; ii < h2o.size() / info.numAtomOnH2o; ++ii){
	std::vector<double > composi(3, 0.);
	// std::vector<double > comvelo(3, 0.);
	for (unsigned jj = 1; jj < 3; ++jj){
	  for (unsigned dd = 0; dd < 3; ++dd){
	    double diff = h2o[info.numAtomOnH2o*ii+jj][dd] - h2o[info.numAtomOnH2o*ii+0][dd];
	    if (diff > boxSize[dd] * 0.5){
	      h2o[info.numAtomOnH2o*ii+jj][dd] -= boxSize[dd];
	    }
	    else if (diff < -boxSize[dd] * 0.5) {
	      h2o[info.numAtomOnH2o*ii+jj][dd] += boxSize[dd];
	    }
	  }
	}
	for (unsigned jj = 0; jj < 3; ++jj){
	  for (unsigned dd = 0; dd < 3; ++dd){
	    composi[dd] += mass[jj] * h2o[info.numAtomOnH2o*ii+jj][dd];
	    // comvelo[dd] += mass[jj] * velo[3*i+jj][dd];
	  }
	}
	for (unsigned dd = 0; dd < 3; ++dd){
	  composi[dd] /= totalmass;
	}
	com[ii] = composi;
      }
    }
    
    dw.calculate (time, com, tmass);
    dw.write();
  }
  
  return 0;
}
