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

#include "GroFileManager.h"

namespace po = boost::program_options;

void applyPertMol (const double & rcut0,
		   const double & rcut1,
		   const double & strength,
		   const std::vector<double > & box,
		   const std::vector<std::vector<double > > & posi,
		   std::vector<std::vector<double > > & velo)
{
  std::vector<double > center (box);
  center[0] *= 0.5;
  center[1] *= 0.5;
  center[2] *= 0.5;
  std::vector<double > diff (3);
  for (unsigned ii = 1; ii < posi.size(); ++ii){
    for (unsigned dd = 0; dd < 3; ++dd){
      if      (posi[ii][dd] - posi[0][dd] > box[dd] * 0.5) {
	std::cerr << "wrong mole alignment" << std::endl;
      }
      else if (posi[ii][dd] - posi[0][dd] <-box[dd] * 0.5) {
	std::cerr << "wrong mole alignment" << std::endl;
      }
    }
  }
  for (unsigned ii = 0; ii < posi.size(); ++ii){
    for (unsigned dd = 0; dd < 3; ++dd){
      diff[dd] += posi[ii][dd] / double(posi.size());
    }
  }
  for (unsigned dd = 0; dd < 3; ++dd){
    diff[dd] -= center[dd];
  }
  double diff2 = 0;
  for (unsigned dd = 0; dd < 3; ++dd){
    diff2 += diff[dd] * diff[dd];
  }
  if (diff2 > rcut0 * rcut0 && diff2 < rcut1 * rcut1){
    double tmp = strength/sqrt(diff2);
    for (unsigned dd = 0; dd < 3; ++dd){
      diff[dd] *= tmp;
    }
    for (unsigned ii = 0; ii < velo.size(); ++ii){
      for (unsigned dd = 0; dd < 3; ++dd){
	velo[ii][dd] -= diff[dd];
      }
    }
  } 
}


int main(int argc, char * argv[])
{
  std::string ifile, ofile;
  double rcut0, rcut1, strength;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("rcut0", po::value<double > (&rcut0)->default_value(0.8), "outer shell size")
      ("rcut1", po::value<double > (&rcut1)->default_value(1.0), "inner shell size")
      ("strength,s", po::value<double > (&strength)->default_value(1.0), "strength of the perturbation")
      ("input,f",   po::value<std::string > (&ifile)->default_value ("conf.gro"), "the input .gro file")
      ("output,o",  po::value<std::string > (&ofile)->default_value ("out.gro"), "the output .gro file");
      
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }
  
  std::vector<int > resdindex;
  std::vector<std::string >  resdname;
  std::vector<std::string >  atomname;
  std::vector<int > atomindex;
  std::vector<std::vector<double > > posi;
  std::vector<std::vector<double > > velo, newVelo;
  std::vector<double > boxsize;
  
  GroFileManager::read (ifile, resdindex, resdname, atomname, atomindex, posi, velo, boxsize);
  
  int preResdIndex = 1;
  std::vector<std::vector<double > > mole_posi;
  std::vector<std::vector<double > > mole_velo;
  for (unsigned ii = 0; ii < resdindex.size(); ++ii){
    if (preResdIndex == resdindex[ii]){
      mole_posi.push_back (posi[ii]);
      mole_velo.push_back (velo[ii]);
    }
    else{
      applyPertMol (rcut0, rcut1, strength, boxsize, mole_posi, mole_velo);
      newVelo.insert (newVelo.end(), mole_velo.begin(), mole_velo.end());
      mole_posi.clear();
      mole_velo.clear();
      mole_posi.push_back (posi[ii]);
      mole_velo.push_back (velo[ii]);
    }
    preResdIndex = resdindex[ii];
  }
  if (mole_posi.size() != 0){
    applyPertMol (rcut0, rcut1, strength, boxsize, mole_posi, mole_velo);
    newVelo.insert (newVelo.end(), mole_velo.begin(), mole_velo.end());
  }
  if (newVelo.size() != velo.size()){
    std::cerr << "wrong program!" << std::endl;
    return 1;
  }
  
  GroFileManager::write (ofile, resdindex, resdname, atomname, atomindex, posi, newVelo, boxsize);

  return 0;
}
