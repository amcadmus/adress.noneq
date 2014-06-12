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
#include "BlockAverage.h"
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
  std::string ifile, idfile, ofile;
  // double refh;
  // float time_prec = .01;
  // double setA_psi_b = 128, setA_psi_e = 13;
  // double setA_phi_b =-125, setA_phi_e = 74;
  // double setB_psi_b = 128, setB_psi_e = 13;
  // double setB_phi_b = 74,  setB_phi_e =-125;
  double setC_psi_b = 13,  setC_psi_e = 128;
  double setC_phi_b = -180,setC_phi_e = 180;
  
  double setA1_psi_b =-134, setA1_psi_e = 13;
  double setA1_phi_b =-125, setA1_phi_e = 74;
  double setA2_psi_b = 128, setA2_psi_e =-134;
  double setA2_phi_b =-125, setA2_phi_e = 74;

  double setB1_psi_b =-110, setB1_psi_e = 13;
  double setB1_phi_b = 74,  setB1_phi_e =-125;
  double setB2_psi_b = 128, setB2_psi_e =-110;
  double setB2_phi_b = 74,  setB2_phi_e =-125;

  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input-dir,d",  po::value<std::string > (&idfile)->default_value ("dir.name"), "the file of dir names")
      ("input,f",  po::value<std::string > (&ifile)->default_value ("angle.dat"), "the file name to find in the dir")
      ("output,o", po::value<std::string > (&ofile)->default_value ("traj.disc"), "the output file name in the dir");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  // MetastableSet setA 
  // MetastableSet setB (setB_psi_b, setB_psi_e, setB_phi_b, setB_phi_e);
  // MetastableSet setC (setC_psi_b, setC_psi_e, setC_phi_b, setC_phi_e);
  vector<MetastableSet > sets;
  // sets.push_back (MetastableSet(setA_psi_b, setA_psi_e, setA_phi_b, setA_phi_e));
  // sets.push_back (MetastableSet(setB_psi_b, setB_psi_e, setB_phi_b, setB_phi_e));
  sets.push_back (MetastableSet(setA1_psi_b, setA1_psi_e, setA1_phi_b, setA1_phi_e));
  sets.push_back (MetastableSet(setA2_psi_b, setA2_psi_e, setA2_phi_b, setA2_phi_e));
  sets.push_back (MetastableSet(setB1_psi_b, setB1_psi_e, setB1_phi_b, setB1_phi_e));
  sets.push_back (MetastableSet(setB2_psi_b, setB2_psi_e, setB2_phi_b, setB2_phi_e));
  sets.push_back (MetastableSet(setC_psi_b, setC_psi_e, setC_phi_b, setC_phi_e));

  ifstream fpname (idfile.c_str());
  if (!fpname){
    std::cerr << "cannot open file " << idfile << std::endl;
    return 1;
  }
  char nameline [MaxLineLength];
  float time;
  ValueType phi, psi;
  vector<vector<BlockAverage_acc > > avgs;
  unsigned countFile = 0;

  while (fpname.getline(nameline, MaxLineLength)){
    if (nameline[0] == '#') continue;
    string filename = string(nameline) + string("/") + ifile;
    string filenameo = string(nameline) + string("/") + ofile;
    FILE *fp = fopen (filename.c_str(), "r");
    FILE *fpo = fopen (filenameo.c_str(), "w");
    // cout << "reading file " << filename << endl;
    if (fp == NULL){
      std::cerr << "cannot open file " << filename << std::endl;
      return 1;
    }
    if (fpo == NULL){
      std::cerr << "cannot open file " << filenameo << std::endl;
      return 1;
    }
    countFile ++;
    vector<double > tmpcount;
    while (myread(fp, time, phi, psi)){
      depositMetastable (psi, phi, sets, tmpcount);
      for (unsigned ii = 0; ii < tmpcount.size(); ++ii){
	if (tmpcount[ii] != 0){
	  fprintf (fpo, "%d\n", ii);
	}
      }
    }
    fclose (fp);
    fclose (fpo);
  }

  return 0;
}

