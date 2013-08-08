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

#include "Trajectory.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;
using namespace std;

#define MaxLineLength 2048


int main(int argc, char * argv[])
{
  std::string ifile, ofile;

  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("output,o", po::value<std::string > (&ofile)->default_value ("two.vectors.out"), "the order parameter of the molecule")
      ("input-dir",  po::value<std::string > (&ifile)->default_value ("dir.name"), "the file of file names");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  ifstream fpname (ifile.c_str());
  if (!fpname){
    std::cerr << "cannot open file " << ifile << std::endl;
    return 1;
  }
  
  char nameline [MaxLineLength];
  vector<float > times;
  vector<vector<double > > vector0;
  vector<vector<double > > vector1;
  vector<double > length0;
  vector<double > length1;
  unsigned countFile = 0;
  

  while (fpname.getline(nameline, MaxLineLength)){
    XtcLoader xtcl ;
    vector<vector<double > > frame;
    string myfilename (nameline);
    myfilename += string("/traj.xtc");
    cout << "reading file " << myfilename << endl;    
    if (! xtcl.reinit(myfilename.c_str())){
      return 1;
    }
    countFile ++;
    if (countFile == 1){
      while ( xtcl.load() ) {
	times.push_back (xtcl.getTime());
	xtcl.getFrame (frame);
	vector<double > c0 (frame[0]);
	vector<double > ca (frame[8]);
	vector<double > c1 (frame[18]);
	double dd0 = 0.;
	double dd1 = 0.;
	for (unsigned dd = 0; dd < 3; ++dd){
	  c0[dd] -= ca[dd];
	  c1[dd] -= ca[dd];
	  dd0 += c0[dd] * c0[dd];
	  dd1 += c1[dd] * c1[dd];
	}
	dd0 = sqrt(dd0);
	dd1 = sqrt(dd1);
	vector0.push_back (c0);
	vector1.push_back (c1);
	length0.push_back (dd0);
	length1.push_back (dd1);
      }
    }
    else {
      unsigned countFrame = 0;
      while ( xtcl.load() ) {
	if (countFrame >= times.size()){
	  cerr << "inconsistent frames" << endl;
	  return 1;
	}
	xtcl.getFrame (frame);
	vector<double > c0 (frame[0]);
	vector<double > ca (frame[8]);
	vector<double > c1 (frame[18]);
	double dd0 = 0.;
	double dd1 = 0.;
	for (unsigned dd = 0; dd < 3; ++dd){
	  c0[dd] -= ca[dd];
	  c1[dd] -= ca[dd];
	  dd0 += c0[dd] * c0[dd];
	  dd1 += c1[dd] * c1[dd];
	}
	dd0 = sqrt(dd0);
	dd1 = sqrt(dd1);
	for (unsigned dd = 0; dd < 3; ++dd){
	  vector0[countFrame][dd] += c0[dd];
	  vector1[countFrame][dd] += c1[dd];
	}
	length0[countFrame] += dd0;
	length1[countFrame] += dd1;
	// if (time_moi != time_a1 ||
	//     time_moi != time_a2 ||
	//     time_moi != time_a3 ){
	//   cerr << "inconsistent time of axis and eigen value!" << endl;
	//   exit (1);
	// }
	// mysort (moi, order);
	// for (unsigned dd = 0; dd < 3; ++dd){
	//   double cosv = aa[order[dd]][0];
	//   values[dd][countFrame] += 0.5 * (3. * cosv * cosv - 1.);
	// }
	// // depositMetastable (psi, phi, sets, tmpcount);
	// // for (unsigned dd = 0; dd < tmpcount.size(); ++dd){
	// //   if (tmpcount[dd] != 0){
	// //     values[dd][countFrame][0] += dipole[0];
	// //     values[dd][countFrame][1] += dipole[1];
	// //     values[dd][countFrame][2] += dipole[2];
	// //     counts[dd][countFrame] += 1;
	// //   }
	// // }
	countFrame ++;
      }
    }
  }
  
  FILE * fp = fopen (ofile.c_str(), "w");
  fprintf (fp, "# time   length0   length1   vector0 (3 components)   vector1 (3 components))\n");
  for (unsigned ii = 0; ii < times.size(); ++ii){
    fprintf (fp, "%f ", times[ii]);
    fprintf (fp, "%f   %f   ",
	     length0[ii] / double (countFile),
	     length1[ii] / double (countFile)
	);
    for (unsigned dd = 0; dd < 3; ++dd){
      fprintf (fp, "%f ", vector0[ii][dd] / double (countFile));
    }
    fprintf (fp, "  ");
    for (unsigned dd = 0; dd < 3; ++dd){
      fprintf (fp, "%f ", vector1[ii][dd] / double (countFile));
    }
    fprintf (fp, "\n");
  }
  fclose (fp);
  
  return 0;
}

