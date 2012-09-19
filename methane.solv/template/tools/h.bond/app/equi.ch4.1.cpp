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
#include "StringSplit.h"

using namespace CircleOperations;
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
  float begin, end, rcut;
  std::string ifile, ofile, tfile;
  float time_prec = .01;
  TopInfo info;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("begin,b", po::value<float > (&begin)->default_value(0.f), "start time")
      ("end,e",   po::value<float > (&end  )->default_value(0.f), "end   time")
      ("rcut,r", po::value<float > (&rcut)->default_value(0.53f), "cut-off to cal h-bond")
      ("block,n", po::value<unsigned > (&numBlock)->default_value(20), "number of block for averaging")
      ("top-file,t",po::value<std::string > (&tfile), "topolgy of the system")
      ("output,o",  po::value<std::string > (&ofile)->default_value ("count.out"), "the output of count of h-bond")
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
  std::cout << "# rcut: " << rcut << std::endl;
  std::cout << "# block: " << numBlock << std::endl;
  std::cout << "# top file: " << tfile << std::endl;
  std::cout << "###################################################" << std::endl;  

  HydrogenBond_Geo_2 hbond_id;
  TrajLoader tjl (ifile.c_str(), info);
  OneFrameHbonds ofh (tjl.getBox(), rcut, dynamic_cast<const HydrogenBond *const>(&hbond_id));
  Analyzer ana;
  PolygonAverge pavg;
  pavg.reinit (13, numBlock);

  std::vector<std::vector<ValueType > > ch4;
  std::vector<std::vector<ValueType > > h2o;
  std::vector<Hbond > bonds;

  int countread = 0;
  FILE * fo = fopen(ofile.c_str(), "w");
  fprintf (fo, "# time  No.1st_shell  No.Hbond  No.3  No. 4  No.5  No.6  No.7\n");
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
    ana.getCircles().countPoly ();
    fprintf (fo, "%.3f  %d  %d  %d %d %d %d %d %d %d %d %d %d %d\n",
	     tjl.getTime(), ofh.getNumFirstShell(), bonds.size(),
	     ana.getCircles().count[3], 
	     ana.getCircles().count[4], 
	     ana.getCircles().count[5], 
	     ana.getCircles().count[6], 
	     ana.getCircles().count[7],
	     ana.getCircles().count[8],
	     ana.getCircles().count[9],
	     ana.getCircles().count[10],
	     ana.getCircles().count[11],
	     ana.getCircles().count[12],
	     ana.getCircles().count[13]
	     );
  }
  fclose(fo);

  pavg.average();
  FILE * fp = fopen ("poly.distrib.out", "w");
  pavg.print (fp);
  fclose (fp);
  
  return 0;
}
