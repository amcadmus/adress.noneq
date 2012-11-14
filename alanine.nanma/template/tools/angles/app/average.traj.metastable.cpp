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
#include "Distribution.h"
#include <boost/program_options.hpp>
namespace po = boost::program_options;
using namespace std;
#define MaxLineLength 2048

bool myread (FILE * fp,
	     float & time,
	     ValueType & phi,
	     ValueType & psi)
{
  size_t rv;
  rv = fread (&time, sizeof(float), 1, fp);
  if (rv != 1){
    // cerr << "error read time or reach EOF" << endl;
    return false;
  }
  rv = fread (&phi, sizeof(double), 1, fp);
  if (rv != 1){
    cerr << "error read phi " << endl;
    exit(1);
  }
  rv = fread (&psi, sizeof(double), 1, fp);
  if (rv != 1){
    cerr << "error read psi " << endl;
    exit(1);
  }
  return true;
}

void depositMetastable (const double & phi,
			const double & psi,
			const vector<MetastableSet> & sets,
			vector<double > & counts)
{
  int cc = 0;
  counts.resize (sets.size());
  for (unsigned ii = 0; ii < sets.size(); ++ii){
    counts[ii] = 0;
    if (sets[ii].inSet(phi, psi)){
      counts[ii] = 1;
      cc ++;
    }
  }
  
  if (cc != 1) {
    if (cc == 0){
      cerr << "not in any set!!!" << endl;
    }
    else {
      cerr << "in multi sets" << endl;
    }
  }
}


int main(int argc, char * argv[])
{
  std::string ifile, ofile;
  // unsigned numBlock = 20;
  // double refh;
  // float time_prec = .01;
  double setA_phi_b = 128, setA_phi_e = 13;
  double setA_psi_b =-125, setA_psi_e = 74;
  double setB_phi_b = 128, setB_phi_e = 13;
  double setB_psi_b = 74,  setB_psi_e =-125;
  double setC_phi_b = 13,  setC_phi_e = 128;
  double setC_psi_b = -180,setC_psi_e = 180;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("output,o", po::value<std::string > (&ofile)->default_value ("metastable.out"), "the output of metastable propulation")
      ("input,f",  po::value<std::string > (&ifile)->default_value ("angle.name"), "the file of file names");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  // MetastableSet setA 
  // MetastableSet setB (setB_phi_b, setB_phi_e, setB_psi_b, setB_psi_e);
  // MetastableSet setC (setC_phi_b, setC_phi_e, setC_psi_b, setC_psi_e);
  vector<MetastableSet > sets;
  sets.push_back (MetastableSet(setA_phi_b, setA_phi_e, setA_psi_b, setA_psi_e));
  sets.push_back (MetastableSet(setB_phi_b, setB_phi_e, setB_psi_b, setB_psi_e));
  sets.push_back (MetastableSet(setC_phi_b, setC_phi_e, setC_psi_b, setC_psi_e));

  ifstream fpname (ifile.c_str());
  if (!fpname){
    std::cerr << "cannot open file " << ifile << std::endl;
    return 1;
  }
  char nameline [MaxLineLength];
  float time;
  ValueType phi, psi;
  vector<float > times;
  vector<vector<double > > counts;
  unsigned countFile = 0;

  while (fpname.getline(nameline, MaxLineLength)){
    if (nameline[0] == '#') continue;
    FILE *fp = fopen (nameline, "r");
    cout << "reading file " << nameline << endl;
    if (fp == NULL){
      std::cerr << "cannot open file " << nameline << std::endl;
      return 1;
    }
    countFile ++;
    vector<double > tmpcount;
    if (countFile == 1){
      while (myread(fp, time, phi, psi)){
	times.push_back (time);
	depositMetastable (phi, psi, sets, tmpcount);
	counts.push_back (tmpcount);
      }
    }
    else {
      unsigned countFrame = 0;
      while (myread(fp, time, phi, psi)){
	if (countFrame >= times.size()){
	  cerr << "inconsistent frames" << endl;
	  return 1;
	}
	depositMetastable (phi, psi, sets, tmpcount);
	for (unsigned dd = 0; dd < tmpcount.size(); ++dd){
	  counts[countFrame][dd] += tmpcount[dd];
	}
	countFrame ++;
      }
    }
    fclose (fp);
  }

  FILE * fp = fopen (ofile.c_str(), "w");
  for (unsigned ii = 0; ii < times.size(); ++ii){
    fprintf (fp, "%f ", time);
    for (unsigned dd = 0; dd < sets.size(); ++dd){
      fprintf (fp, "%f ", counts[ii][dd]);
    }
    fprintf (fp, "\n");
  }
  fclose (fp);
  
  return 0;
}
