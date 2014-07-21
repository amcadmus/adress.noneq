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
#include "Trajectory.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;
// using namespace GmxTop;

int main (int argc, char * argv[])
{
  string ifilegro, ifilextc, catname;
  double rc;
  po::options_description desc ("Allow options");
  
  desc.add_options()
      ("help,h", "count number of certain atom in a spherical region")
      ("rc,r", po::value<double > (&rc)->default_value (1.0), "radius of the spherical region")
      ("input-gro", po::value<std::string > (&ifilegro)->default_value (std::string("conf.gro")), "input gro file")
      ("input-xtc", po::value<std::string > (&ifilextc)->default_value (std::string("traj.xtc")), "input trajectory (xtc) file")
      ("atom-name,a", po::value<std::string > (&catname)->default_value ("CMW"), "the name of atom should be count");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);

  if (vm.count("help") || vm.count("h")){
    std::cout << desc<< "\n";
    return 0;
  }

  std::vector<int >  resdindex;
  std::vector<std::string >   resdname;
  std::vector<std::string >   atomname;
  std::vector<int >  atomindex;
  std::vector<std::vector<double > >  posi;
  std::vector<std::vector<double > >  velo;
  std::vector<double >  boxsize;
  
  GroFileManager::read (ifilegro, resdindex, resdname, atomname, atomindex,
  			posi, velo, boxsize);

  XtcLoader xtcl;
  xtcl.reinit (ifilextc.c_str());

  vector<bool > count (posi.size());
  for (unsigned ii = 0; ii < posi.size(); ++ii){
    if (atomname[ii] == catname){
      count[ii] = true;
    }
    else {
      count[ii] = false;
    }
  }

  vector<vector<double > > frame;
  vector<double > center;
  float time;
  double rc2 = rc * rc;

  while (xtcl.load ()){
    center = xtcl.getBox ();
    time = xtcl.getTime();
    xtcl.getFrame (frame);
    center[0] *= 0.5;
    center[1] *= 0.5;
    center[2] *= 0.5;
    int mycount = 0;
    for (unsigned ii = 0; ii < frame.size(); ++ii){
      if (count[ii]){
	double diff[3];
	diff[0] = center[0] - frame[ii][0];
	diff[1] = center[1] - frame[ii][1];
	diff[2] = center[2] - frame[ii][2];
	double dr2 = diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2];
	if (dr2 < rc2){
	  mycount ++;
	}
      }
    }
    printf ("%f %d\n", time, mycount);
  }
  
  // unsigned pt = 0;
  // string pname = resdname[0];  
  // for (unsigned pt1 = 1; pt1 < resdname.size(); ++pt1){
  //   if (pname != resdname[pt1]){
  //     printf ("%s %d\n", pname.c_str(), resdindex[pt1] - resdindex[pt]);
  //     pname = resdname[pt1];
  //     pt = pt1;
  //   }
  //   // cout << pt1 << endl;
  // }
  // printf ("%s %d\n", pname.c_str(), resdindex.back() - resdindex[pt] + 1);
  
  return 0;
}
