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
  std::string itfile, ofile;
  unsigned every;
  // unsigned numBlock = 20;
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
      ("every,e", po::value<unsigned > (&every)->default_value (1), "split every frame")
      ("output,o", po::value<std::string > (&ofile)->default_value ("equi.frame"), "the output of frames")
      ("input,f",  po::value<std::string > (&itfile)->default_value ("angle.dat"), "the file of trajectory");

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

  ifstream fpname (itfile.c_str());
  if (!fpname){
    std::cerr << "cannot open file " << itfile << std::endl;
    return 1;
  }
  float time;
  ValueType phi, psi;
  vector<double > counts;

  FILE *fp = fopen (itfile.c_str(), "r");
  cout << "reading file " << itfile << endl;
  if (fp == NULL){
    std::cerr << "cannot open file " << itfile << std::endl;
    return 1;
  }
  
  unsigned countFrame = 0;
  unsigned countCal = 0;
  string tmpname;
  FILE **fps;
  fps = (FILE **) malloc (sizeof(FILE *) * 5);
  tmpname = ofile + ".a1";
  fps[0] = fopen (tmpname.c_str(), "w");
  tmpname = ofile + ".a2";
  fps[1] = fopen (tmpname.c_str(), "w");
  tmpname = ofile + ".b1";
  fps[2] = fopen (tmpname.c_str(), "w");
  tmpname = ofile + ".b2";
  fps[3] = fopen (tmpname.c_str(), "w");
  tmpname = ofile + ".c";
  fps[4] = fopen (tmpname.c_str(), "w");
  vector <unsigned > countConf (5, 0);
  FILE * fpo = fopen (ofile.c_str(), "w");
  
  while (myread(fp, time, phi, psi)){
    // cout << "time is " << time << endl;
    if ((countFrame++) % every != 0) continue;
    countCal ++;
    vector<double > tmpcount;
    depositMetastable (psi, phi, sets, tmpcount);
    for (unsigned ii = 0; ii < 5; ++ii){
      if (tmpcount[ii] == 1) {
	countConf[ii] ++;
	if (countCal != 1){
	  fprintf (fps[ii], "%06d %.10f\n", countCal-1, time - 2.5);
	  fprintf (fpo, "%06d %.10f\n", countCal-1, time - 2.5);
	}
	break;
      }
    }
  }
  fclose (fp);
  fclose (fpo);
  fclose (fps[0]);
  fclose (fps[1]);
  fclose (fps[2]);
  fclose (fps[3]);
  fclose (fps[4]);
  free (fps);
  
  return 0;
}