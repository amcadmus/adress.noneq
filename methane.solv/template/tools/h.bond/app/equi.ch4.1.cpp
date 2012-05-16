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

#include "Tree.h"
#include "HbondMap.h"
#include "Analyzer.h"
#include <boost/program_options.hpp>

using namespace CircleOperations;
namespace po = boost::program_options;

int main(int argc, char * argv[])
{
  float begin, end, rcut;
  std::string ifile;
  float time_prec = .01;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("begin,b", po::value<float > (&begin)->default_value(0.f), "start time")
      ("end,e",   po::value<float > (&end  )->default_value(0.f), "end   time")
      ("rcut,r", po::value<float > (&rcut)->default_value(0.53f), "cut-off to cal h-bond")
      ("input,f",   po::value<std::string > (&ifile)->default_value ("traj.xtc"), "the input .xtc file");
      
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }
  
  std::cout << "###################################################" << std::endl;
  std::cout << "# begin->end: " << begin << " " << end << std::endl;
  std::cout << "# rcut: " << rcut << std::endl;
  std::cout << "###################################################" << std::endl;  

  HydrogenBond_Geo_1 hbond_id (.35, 30.);
  TrajLoader tjl (ifile.c_str());
  OneFrameHbonds ofh (tjl.getNumAtomCh4(), tjl.getNumAtomH2o(), tjl.getBox(), rcut, hbond_id);
  Analyzer ana;
  PolygonAverge pavg;
  pavg.reinit (13);

  std::vector<std::vector<ValueType > > ch4;
  std::vector<std::vector<ValueType > > h2o;
  std::vector<Hbond > bonds;

  int countread = 0;
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

    tjl.formCoords (ch4, h2o);
    bonds = ofh.calBonds (ch4, h2o);
    ana.clear();
    ana.readData_exhaustive(bonds);
    printf ("time: %.2f    No. 1st shell: %d    No. H-bond: %d    No. Circle: %d |",
	    tjl.getTime(), ofh.getNumFirstShell(), bonds.size(), ana.getCircles().circles.size());
    for (unsigned ii = 0; ii < ana.getCircles().circles.size(); ++ii){
      printf ("%d ", ana.getCircles().circles[ii].size());
    }
    pavg.deposit (ana.getCircles());
    printf ("\n");
  }

  pavg.average();
  FILE * fp = fopen ("poly.distrib.out", "w");
  pavg.print (fp);
  fclose (fp);
  
  return 0;
}
