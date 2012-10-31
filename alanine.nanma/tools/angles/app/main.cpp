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
namespace po = boost::program_options;

#include "Analyzer.h"
#include "Angle.h"
#include "Distribution.h"

void readTop (const std::string & file,
              TopInfo & info)
{
  FILE * fp = fopen (file.c_str(), "r");
  if (fp == NULL){
    std::cout << "cannot open file " << file << std::endl;
    std::cout << "do not log top" << std::endl;
    return;
  }
  fscanf (fp, "%d %d", &(info.numAtomOnALA), &(info.comIndexALA));
  fscanf (fp, "%d", &(info.numAtomOnH2o));
  fscanf (fp, "%d %d %d", &(info.OIndexH2o), &(info.H1IndexH2o), &(info.H2IndexH2o));
  fclose (fp);
}


int main(int argc, char * argv[])
{
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
      ("refh,r",  po::value<double > (&refh)->default_value(10), "size of bin (deg.)")
      ("top-file,t",po::value<std::string > (&tfile)->default_value ("mytop"), "topolgy of the system")
      ("output,o",  po::value<std::string > (&ofile)->default_value ("angle.out"), "the output of angle distribution")
      ("input,f",   po::value<std::string > (&ifile)->default_value ("traj.xtc"), "the input .xtc file");
      
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
  std::cout << "# top file: " << tfile << std::endl;
  std::cout << "###################################################" << std::endl;  

  TrajLoader_xtc tjl (ifile.c_str(), info);
  std::vector<std::vector<ValueType > > ala;
  std::vector<std::vector<ValueType > > h2o;
  VectorType box = tjl.getBox();
 
  int countread = 0;
  AngleCalculator ac (box);
  ValueType phi, psi;
  Distribution_1d dist;

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
    if (countread++ % 100 == 0){
      printf ("# load frame at time: %.1f ps\r", time);
      fflush (stdout);
    }
    tjl.formCoords (ala, h2o);
    ac.calPhiPsi (ala, phi, psi);
    // cout << "time: " << time ;
    // cout << ";  angle psi is : " << psi << "; phi is: " << phi << endl;
    // cout << psi << "  " << phi << endl;

    dist.deposite (psi, phi);
  }
  cout << endl;
  
  dist.average();
  dist.print_xv (ofile);

  
  return 0;
}
