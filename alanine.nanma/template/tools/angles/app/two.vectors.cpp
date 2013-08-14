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
      ("input-name",  po::value<std::string > (&ifile)->default_value ("xtc.name"), "the file of file names");

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
  vector<vector<double > > vector2;
  vector<double > length0;
  vector<double > length1;
  vector<double > length2;
  vector<double > order0;
  vector<double > order1;
  vector<double > order2;
  vector<double > avg2;
  vector<double > var2;
  unsigned countFile = 0;
  

  while (fpname.getline(nameline, MaxLineLength)){
    XtcLoader xtcl ;
    vector<vector<double > > frame;
    string myfilename (nameline);
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
	for (unsigned dd = 0; dd < 3; ++dd){
	  c0[dd] /= dd0;
	  c1[dd] /= dd1;
	}
	vector<double > c2(c0);
	double dd2 = 0.;
	for (unsigned dd = 0; dd < 3; ++dd){
	  c2[dd] += c1[dd];
	  dd2 += c2[dd] * c2[dd];
	}
	dd2 = sqrt(dd2);
	for (unsigned dd = 0; dd < 3; ++dd){
	  c2[dd] /= dd2;
	}
	vector0.push_back (c0);
	vector1.push_back (c1);
	vector2.push_back (c2);
	length0.push_back (dd0);
	length1.push_back (dd1);
	length2.push_back (dd2);
	// order0.push_back (0.5 * (3. * c0[0] * c0[0] - 1.));
	// order1.push_back (0.5 * (3. * c1[0] * c1[0] - 1.));
	// order2.push_back (0.5 * (3. * c2[0] * c2[0] - 1.));
	double tmps2;
	tmps2 = 1. - c0[0] * c0[0];
	order0.push_back (3. * tmps2 - 2.);
	tmps2 = 1. - c1[0] * c1[0];
	order1.push_back (3. * tmps2 - 2.);
	tmps2 = 1. - c2[0] * c2[0];
	order2.push_back (3. * tmps2 - 2.);
	avg2.push_back (c2[0]);
	var2.push_back (c2[0] * c2[0]);
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
	  c0[dd] /= dd0;
	  c1[dd] /= dd1;
	}
	vector<double > c2(c0);
	double dd2 = 0.;
	for (unsigned dd = 0; dd < 3; ++dd){
	  c2[dd] += c1[dd];
	  dd2 += c2[dd] * c2[dd];
	}
	dd2 = sqrt(dd2);
	for (unsigned dd = 0; dd < 3; ++dd){
	  c2[dd] /= dd2;
	}
	for (unsigned dd = 0; dd < 3; ++dd){
	  vector0[countFrame][dd] += c0[dd];
	  vector1[countFrame][dd] += c1[dd];
	  vector2[countFrame][dd] += c2[dd];
	}
	length0[countFrame] += dd0;
	length1[countFrame] += dd1;
	length2[countFrame] += dd2;
	// order0[countFrame] += (0.5 * (3. * c0[0] * c0[0] - 1.));
	// order1[countFrame] += (0.5 * (3. * c1[0] * c1[0] - 1.));
	// order2[countFrame] += (0.5 * (3. * c2[0] * c2[0] - 1.));
	double tmps2;
	tmps2 = 1. - c0[0] * c0[0];
	order0[countFrame] += 3. * tmps2 - 2.;
	tmps2 = 1. - c1[0] * c1[0];
	order1[countFrame] += 3. * tmps2 - 2.;
	tmps2 = 1. - c2[0] * c2[0];
	order2[countFrame] += 3. * tmps2 - 2.;
	avg2[countFrame] += c2[0];
	var2[countFrame] += c2[0] * c2[0];
	
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
  fprintf (fp, "# time   length0   length1   order0   order1   order1+2   vector0 (3 components)   vector1 (3 components))   vector1+vector2 (3 components))\n");
  for (unsigned ii = 0; ii < times.size(); ++ii){
    fprintf (fp, "%f ", times[ii]);
    fprintf (fp, "%f   %f   %f   %f    %f    %f   %f   %f    ",
	     length0[ii] / double (countFile),
	     length1[ii] / double (countFile),
	     length2[ii] / double (countFile),
	     order0[ii] / double (countFile),
	     order1[ii] / double (countFile),
	     order2[ii] / double (countFile),
	     avg2[ii] / double (countFile),
	     sqrt(var2[ii] / double (countFile) - avg2[ii] / double (countFile) * avg2[ii] / double (countFile))
	);
    for (unsigned dd = 0; dd < 3; ++dd){
      fprintf (fp, "%f ", vector0[ii][dd] / double (countFile));
    }
    fprintf (fp, "  ");
    for (unsigned dd = 0; dd < 3; ++dd){
      fprintf (fp, "%f ", vector1[ii][dd] / double (countFile));
    }
    fprintf (fp, "  ");
    for (unsigned dd = 0; dd < 3; ++dd){
      fprintf (fp, "%f ", vector2[ii][dd] / double (countFile));
    }
    fprintf (fp, "\n");
  }
  fclose (fp);
  
  return 0;
}

