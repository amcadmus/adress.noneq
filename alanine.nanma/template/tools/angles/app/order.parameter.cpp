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

#include "Defines.h"
#include "StringSplit.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;
using namespace std;

#define MaxLineLength 2048


bool myread_dipole (FILE * fp,
		    float & time,
		    vector<double> & dipole)
{
  char line[MaxLineLength];
  char * ret;

  while ((ret = fgets(line, MaxLineLength, fp)) &&
	 (line[0] == '#' || line[0] == '@')) {
  }
  if (ret == NULL){
    return false;
  }

  vector<string> out;
  StringOperation::split (string(line), out);

  if (out.size() < 4){
    return false;
  }
  else {
    time = atof(out[0].c_str());
    dipole.resize(3);
    dipole[0] = atof(out[1].c_str());
    dipole[1] = atof(out[2].c_str());
    dipole[2] = atof(out[3].c_str());
    return true;
  }
}

struct Pair 
{
  double x;
  int y;
  Pair (double x_, int y_)
      : x(x_), y(y_)
      {
      }
  bool operator < (const Pair & b) const
      {
	return x < b.x;
      }
}
    ;

void mysort (const vector<double > & eigen,
	     vector<int > & order)
{
  vector<Pair > tmp;
  for (unsigned ii = 0; ii < eigen.size(); ++ii){
    tmp.push_back (Pair(eigen[ii], ii));
  }
  sort (tmp.begin(), tmp.end());
  order.resize(eigen.size());
  for (unsigned ii = 0; ii < eigen.size(); ++ii){
    order[ii] = tmp[ii].y;
  }
}
	     

int main(int argc, char * argv[])
{
  std::string ifile_moi, ifile_a1, ifile_a2, ifile_a3, ofile;

  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("output,o", po::value<std::string > (&ofile)->default_value ("order.parameter.out"), "the order parameter of the molecule")
      ("input-moi",  po::value<std::string > (&ifile_moi)->default_value ("pincipal.name"), "the file of file names")
      ("input-axis1",  po::value<std::string > (&ifile_a1)->default_value ("pincipal.a1.name"), "the file of file names")
      ("input-axis2",  po::value<std::string > (&ifile_a2)->default_value ("pincipal.a2.name"), "the file of file names")
      ("input-axis3",  po::value<std::string > (&ifile_a3)->default_value ("pincipal.a3.name"), "the file of file names");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  ifstream fpname_moi (ifile_moi.c_str());
  if (!fpname_moi){
    std::cerr << "cannot open file " << ifile_moi << std::endl;
    return 1;
  }
  ifstream fpname_a1 (ifile_a1.c_str());
  if (!fpname_a1){
    std::cerr << "cannot open file " << ifile_a1 << std::endl;
    return 1;
  }
  ifstream fpname_a2 (ifile_a2.c_str());
  if (!fpname_a2){
    std::cerr << "cannot open file " << ifile_a2 << std::endl;
    return 1;
  }
  ifstream fpname_a3 (ifile_a3.c_str());
  if (!fpname_a3){
    std::cerr << "cannot open file " << ifile_a3 << std::endl;
    return 1;
  }
  
  char nameline_moi [MaxLineLength];
  char nameline_a1 [MaxLineLength];
  char nameline_a2 [MaxLineLength];
  char nameline_a3 [MaxLineLength];
  float time_moi;
  float time_a1;
  float time_a2;
  float time_a3;
  
  vector<double> moi;
  vector<vector<double > > aa(3);
  vector<float > times;
  vector<int > order;

  vector<vector<double > > values(3);
  unsigned countFile = 0;

  while (fpname_moi.getline(nameline_moi, MaxLineLength) &&
	 fpname_a1.getline(nameline_a1, MaxLineLength) &&
	 fpname_a2.getline(nameline_a2, MaxLineLength) &&
	 fpname_a3.getline(nameline_a3, MaxLineLength) ){
    FILE *fp_moi = fopen (nameline_moi, "r");
    FILE *fp_a1 = fopen (nameline_a1, "r");
    FILE *fp_a2 = fopen (nameline_a2, "r");
    FILE *fp_a3 = fopen (nameline_a3, "r");
    cout << "reading file "
	 << nameline_moi << " , "
	 << nameline_a1 << " , " 
	 << nameline_a2 << " , " 
	 << nameline_a3 << endl;
    if (fp_moi == NULL){
      std::cerr << "cannot open file " << nameline_moi << std::endl;
      return 1;
    }
    if (fp_a1 == NULL){
      std::cerr << "cannot open file " << nameline_a1 << std::endl;
      return 1;
    }
    if (fp_a2 == NULL){
      std::cerr << "cannot open file " << nameline_a2 << std::endl;
      return 1;
    }
    if (fp_a3 == NULL){
      std::cerr << "cannot open file " << nameline_a3 << std::endl;
      return 1;
    }

    countFile ++;
    vector<double > tmpcount;
    if (countFile == 1){
      while (myread_dipole (fp_moi, time_moi, moi) &&
	     myread_dipole (fp_a1, time_a1, aa[0]) &&
	     myread_dipole (fp_a2, time_a2, aa[1]) &&
	     myread_dipole (fp_a3, time_a3, aa[2]) ) {
	if (time_moi != time_a1 ||
	    time_moi != time_a2 ||
	    time_moi != time_a3 ){
	  cerr << "inconsistent time of axis and eigen value!" << endl;
	  exit (1);
	}
	times.push_back (time_moi);
	mysort (moi, order);
	for (unsigned dd = 0; dd < 3; ++dd){
	  double cosv = aa[order[dd]][0];
	  values[dd].push_back (0.5 * (3. * cosv * cosv - 1.));
	}
      }
    }
    else {
      unsigned countFrame = 0;
      while (myread_dipole (fp_moi, time_moi, moi) &&
	     myread_dipole (fp_a1, time_a1, aa[0]) &&
	     myread_dipole (fp_a2, time_a2, aa[1]) &&
	     myread_dipole (fp_a3, time_a3, aa[2]) ) {
	if (countFrame >= times.size()){
	  cerr << "inconsistent frames" << endl;
	  return 1;
	}
	if (time_moi != time_a1 ||
	    time_moi != time_a2 ||
	    time_moi != time_a3 ){
	  cerr << "inconsistent time of axis and eigen value!" << endl;
	  exit (1);
	}
	mysort (moi, order);
	for (unsigned dd = 0; dd < 3; ++dd){
	  double cosv = aa[order[dd]][0];
	  values[dd][countFrame] += 0.5 * (3. * cosv * cosv - 1.);
	}
	// depositMetastable (psi, phi, sets, tmpcount);
	// for (unsigned dd = 0; dd < tmpcount.size(); ++dd){
	//   if (tmpcount[dd] != 0){
	//     values[dd][countFrame][0] += dipole[0];
	//     values[dd][countFrame][1] += dipole[1];
	//     values[dd][countFrame][2] += dipole[2];
	//     counts[dd][countFrame] += 1;
	//   }
	// }
	countFrame ++;
      }
    }
    fclose (fp_moi);
    fclose (fp_a1);
    fclose (fp_a2);
    fclose (fp_a3);
  }
  
  FILE * fp = fopen (ofile.c_str(), "w");
  fprintf (fp, "# time   S_for_max_eigen   S_for_next_max_eigen   S_for_min_eigen)\n");
  for (unsigned ii = 0; ii < times.size(); ++ii){
    fprintf (fp, "%f ", times[ii]);
    for (unsigned dd = 0; dd < 3; ++dd){
      fprintf (fp, "%f   ", values[dd][ii] / double (countFile));
    }
    fprintf (fp, "\n");
  }
  fclose (fp);
  
  return 0;
}

