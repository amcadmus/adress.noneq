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
  double binSize;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("begin,b", po::value<float > (&begin)->default_value(0.f), "start time")
      ("end,e",   po::value<float > (&end  )->default_value(0.f), "end   time")
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
  std::cout << "# block: " << numBlock << std::endl;
  std::cout << "# top file: " << tfile << std::endl;
  std::cout << "###################################################" << std::endl;  

  TrajLoader_xtc tjl (ifile.c_str(), info);

  std::vector<std::vector<ValueType > > ch4;
  std::vector<std::vector<ValueType > > h2o;

  int countread = 0;
  FILE * fo = fopen(ofile.c_str(), "w");

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

    // fprintf (fo, "%.3f  %d  %d  %d %d %d %d %d\n",
    // 	     tjl.getTime(), ofh.getNumFirstShell(), bonds.size(),
    // 	     ana.getCircles().count[3], 
    // 	     ana.getCircles().count[4], 
    // 	     ana.getCircles().count[5], 
    // 	     ana.getCircles().count[6], 
    // 	     ana.getCircles().count[7]);
  }
  fclose(fo);

  // FILE * fp = fopen ("poly.distrib.out", "w");
  // fclose (fp);
  
  return 0;
}
