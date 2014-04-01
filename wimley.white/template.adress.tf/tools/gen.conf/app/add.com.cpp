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
#include <time.h>

#include "GroFileManager.h"
#include <boost/program_options.hpp>
#include "StringSplit.h"
#include "GmxTop.h"

namespace po = boost::program_options;
using namespace GmxTop;

int main (int argc, char * argv[])
{
  std::string ifilename;
  std::string ofilename;
  string tpfilename;
  string resdNameString;
  string cgNameString;
  po::options_description desc ("Allow options");
  
  desc.add_options()
    ("help,h", "print this message")
    ("output,o", po::value<std::string > (&ofilename)->default_value (std::string("out.gro")), "output conf file name")
    ("topol,p",  po::value<std::string > (&tpfilename)->default_value (std::string("topol.top")), "input topol name")
    ("resd-name", po::value<std::string > (&resdNameString)->default_value (std::string("RESD")), "name of explicit group to be added")
    ("cg-name", po::value<std::string > (&cgNameString)->default_value (std::string("CG")), "name of cg")
    ("input,f",  po::value<std::string > (&ifilename)->default_value (std::string("conf.gro")), "input conf file name");
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);

  if (vm.count("help") || vm.count("h")){
    std::cout << desc<< "\n";
    return 0;
  }

  gmx_sys_top top;
  gmx_sys_types type;
  parseTop (tpfilename, top);
  parseType (tpfilename, type);

  std::vector<int >  resdindex;
  std::vector<std::string >   resdname;
  std::vector<std::string >   atomname;
  std::vector<int >  atomindex;
  std::vector<std::vector<double > >  posi;
  std::vector<std::vector<double > >  velo;
  std::vector<double >  boxsize;
  
  GroFileManager::read (ifilename, resdindex, resdname, atomname, atomindex,
  			posi, velo, boxsize);

  std::vector<int >  resdindex1;
  std::vector<std::string >   resdname1;
  std::vector<std::string >   atomname1;
  std::vector<int >  atomindex1;
  std::vector<std::vector<double > >  posi1;
  std::vector<std::vector<double > >  velo1;

  // loop over moles
  int savedResdIdx = -1;
  int atomIdx = 1;
  
  for (int ii = 0; ii < int(posi.size()); ++ii){
    // find new correct mol
    cout << ii << endl;
    if (savedResdIdx != resdindex[ii] && resdNameString == resdname[ii]){
      savedResdIdx = resdindex[ii];
      vector<double > sum_p (3, 0.);
      vector<double > sum_v (3, 0.);
      double sum_m = 0.;      
      for (int atomii = ii; (atomii < int(posi.size()) && resdindex[atomii] == savedResdIdx); ++atomii){
	gmx_atomtypes_item attype;
	matchAtomName (atomname[atomii], top, type, attype);
	sum_m += attype.mass;
	sum_p[0] += posi[atomii][0] * attype.mass;
	sum_p[1] += posi[atomii][1] * attype.mass;
	sum_p[2] += posi[atomii][2] * attype.mass;
	sum_v[0] += velo[atomii][0] * attype.mass;
	sum_v[1] += velo[atomii][1] * attype.mass;
	sum_v[2] += velo[atomii][2] * attype.mass;
	resdname1.push_back (resdname[atomii]);
	resdindex1.push_back (resdindex[atomii]);
	atomindex1.push_back (atomIdx ++);
	atomname1.push_back (atomname[atomii]);
	posi1.push_back (posi[atomii]);
	velo1.push_back (velo[atomii]);
      }
      sum_p[0] /= sum_m;
      sum_p[1] /= sum_m;
      sum_p[2] /= sum_m;
      sum_v[0] /= sum_m;
      sum_v[1] /= sum_m;
      sum_v[2] /= sum_m;
      resdindex1.push_back (savedResdIdx);
      resdname1.push_back (resdname[ii]);
      atomindex1.push_back (atomIdx ++);
      atomname1.push_back (cgNameString);
      posi1.push_back (sum_p);
      velo1.push_back (sum_v);
    }
    else if (resdNameString != resdname[ii]) {
      resdname1.push_back (resdname[ii]);
      resdindex1.push_back (resdindex[ii]);
      atomindex1.push_back (atomIdx ++);
      atomname1.push_back (atomname[ii]);
      posi1.push_back (posi[ii]);
      velo1.push_back (velo[ii]);      
    }
  }

  cout << "finished" << endl;
  
  GroFileManager::write (ofilename,
  			 resdindex1, resdname1,
  			 atomname1, atomindex1,
  			 posi1, velo1, boxsize);
  
  // for (unsigned ii = 0; ii < top.moles.size(); ++ii){
  //   cout << "molname: " << top.moles[ii].name
  // 	 << " num: " << top.numMol[ii]
  // 	 << endl;
  //   for (unsigned jj = 0; jj < top.moles[ii].atoms.size(); ++jj){
  //     cout << "atom: " << top.moles[ii].atoms[jj].name << "   "
  // 	   << "mass: " << top.moles[ii].atoms[jj].mass
  // 	   << endl;
  //   }
  //   cout << endl;
  // }
  
  return 0;
}
