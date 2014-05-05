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
#include "CellList.h"
#include "Dipole.h"

#define MaxLineLength 2048

namespace po = boost::program_options;
using namespace std;

int main(int argc, char * argv[])
{
  std::string ifile;
  double qh, qo;
  double mh, mo;
  float begin, end;
  float time_prec = .01;
  unsigned every, nDataBlock;
  double rup, refh, cellSize;
  
  po::options_description desc ("Program of calculating the dielectric constant.\nAllow options");
  desc.add_options()
      ("help,h", "print this message")
      ("change-h,q",  po::value<double > (&qh)->default_value (0.417), "the charge of hydrogen atom")
      ("mass-h",  po::value<double > (&mh)->default_value (1.008000), "the mass of hydrogen atom")
      ("mass-o",  po::value<double > (&mo)->default_value (15.999400), "the mass of hydrogen atom")
      ("begin,b", po::value<float > (&begin)->default_value(0.f), "start time")
      ("end,e",   po::value<float > (&end  )->default_value(0.f), "end   time")
      ("every",   po::value<unsigned > (&every)->default_value(1), "every frame")
      ("num-data-block",   po::value<unsigned > (&nDataBlock)->default_value(1), "number of data in each block")
      ("rup,u",   po::value<double > (&rup)->default_value(3.f), "max r to make rdf")
      ("refh",  po::value<double > (&refh)->default_value(0.01f), "bin size")
      ("cell-size,c", po::value<double > (&cellSize)->default_value(1.f), "cell list radius")
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
  if (rup * 2. > tjl.getBox()[0]) {
    cerr << "to large rup, set to half box size" << endl;
    rup = 0.5 * tjl.getBox()[0];
  }
  int nmols = tjl.getNAtom()/3;

  VectorType vbox;
  vbox.x = tjl.getBox()[0];
  vbox.y = tjl.getBox()[1];
  vbox.z = tjl.getBox()[2];

  CellList clist (nmols, vbox, cellSize);

  KirkwoodFactor gkr;
  gkr.reinit (rup, refh, nDataBlock);
  
  std::vector<std::vector<ValueType > > coms;
  coms.resize (nmols);

  std::vector<std::vector<ValueType > > dipoles;
  dipoles.resize (nmols);

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
    
    // we assume here the tip3p water 
    for (int ii = 0; ii < nmols; ++ii){
      vector<double > moment (3, 0.);
      vector<double > com (3, 0.);
      double totmi = 1./(mh * 2. + mo);
      for (int dd = 0; dd < 3; ++dd){
	moment[dd] += frame[ii*3+0][dd] * qo;
	moment[dd] += frame[ii*3+1][dd] * qh;
	moment[dd] += frame[ii*3+2][dd] * qh;	
	// com[dd] += frame[ii*3+0][dd] * mo * totmi;
	// com[dd] += frame[ii*3+1][dd] * mh * totmi;
	// com[dd] += frame[ii*3+2][dd] * mh * totmi;
      }
      for (int dd = 0; dd < 3; ++dd){
	ValueType dx1, dx2;
	dx1 = frame[ii*3+1][dd] - frame[ii*3+0][dd];
	dx2 = frame[ii*3+2][dd] - frame[ii*3+0][dd];
	if (dx1 > 0.5 * tjl.getBox()[dd]) {dx1 -= tjl.getBox()[dd]; printf ("hit\n");}
	if (dx1 <-0.5 * tjl.getBox()[dd]) {dx1 += tjl.getBox()[dd]; printf ("hit\n");}
	if (dx2 > 0.5 * tjl.getBox()[dd]) {dx2 -= tjl.getBox()[dd]; printf ("hit\n");}
	if (dx2 <-0.5 * tjl.getBox()[dd]) {dx2 += tjl.getBox()[dd]; printf ("hit\n");}
	com[dd] = mo * totmi * frame[ii*3+0][dd] + mh * totmi * (frame[ii*3+0][dd] + dx1) + mh * totmi * (frame[ii*3+0][dd] + dx2);
	if      (com[dd] <  0               ) com[dd] += tjl.getBox()[dd];
	else if (com[dd] >= tjl.getBox()[dd]) com[dd] -= tjl.getBox()[dd];
      }
      coms[ii] = com;
      dipoles[ii] = moment;
    }

    clist.rebuild (coms);
    gkr.deposit (coms, clist, dipoles, vbox);
  }

  gkr.calculate ();


  return 0;
}
