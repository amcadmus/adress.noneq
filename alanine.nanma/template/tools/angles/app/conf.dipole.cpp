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
#include "StringSplit.h"
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

bool myread_dipole (FILE * fp,
		    float & time,
		    vector<ValueType> & dipole)
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
  std::string iafile, idfile, ofile;
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
      ("output,o", po::value<std::string > (&ofile)->default_value ("metastable.out"), "the output of metastable propulation")
      ("input-angle",  po::value<std::string > (&iafile)->default_value ("angle.name"), "the file of file names")
      ("input-dipole",  po::value<std::string > (&idfile)->default_value ("dipole.name"), "the file of file names");

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

  ifstream fpnamea (iafile.c_str());
  if (!fpnamea){
    std::cerr << "cannot open file " << iafile << std::endl;
    return 1;
  }
  ifstream fpnamed (idfile.c_str());
  if (!fpnamed){
    std::cerr << "cannot open file " << idfile << std::endl;
    return 1;
  }
  char namelinea [MaxLineLength];
  char namelined [MaxLineLength];
  float timea, timed;
  ValueType phi, psi;
  vector<ValueType> dipole;
  vector<float > times;
  vector<vector<vector<double > > > values(sets.size());
  vector<vector<int > > counts (sets.size());
  unsigned countFile = 0;

  while (fpnamea.getline(namelinea, MaxLineLength) &&
	 fpnamed.getline(namelined, MaxLineLength)) {
    if (namelinea[0] == '#') continue;
    if (namelined[0] == '#') continue;
    FILE *fpa = fopen (namelinea, "r");
    FILE *fpd = fopen (namelined, "r");
    cout << "reading file " << namelinea << " and " << namelined << endl;
    if (fpa == NULL){
      std::cerr << "cannot open file " << namelinea << std::endl;
      return 1;
    }
    if (fpd == NULL){
      std::cerr << "cannot open file " << namelined << std::endl;
      return 1;
    }
    countFile ++;
    vector<double > tmpcount;
    if (countFile == 1){
      while (myread (fpa, timea, phi, psi) &&
	     myread_dipole (fpd, timed, dipole)){
	if (timea != timed){
	  cerr << "inconsistent time of angle and dipole!" << endl;
	  exit (1);
	}
	times.push_back (timea);
	depositMetastable (psi, phi, sets, tmpcount);
	for (unsigned dd = 0; dd < tmpcount.size(); ++dd){
	  if (tmpcount[dd] == 0){
	    values[dd].push_back (vector<ValueType>(3,0.0));
	    counts[dd].push_back (0);
	  }
	  else {
	    vector<double > tmpdipole(3);
	    values[dd].push_back (dipole);
	    counts[dd].push_back (1);
	  }
	}
      }
    }
    else {
      unsigned countFrame = 0;
      while (myread(fpa, timea, phi, psi) &&
	     myread_dipole (fpd, timed, dipole)){
	if (countFrame >= times.size()){
	  cerr << "inconsistent frames" << endl;
	  return 1;
	}
	depositMetastable (psi, phi, sets, tmpcount);
	for (unsigned dd = 0; dd < tmpcount.size(); ++dd){
	  if (tmpcount[dd] != 0){
	    values[dd][countFrame][0] += dipole[0];
	    values[dd][countFrame][1] += dipole[1];
	    values[dd][countFrame][2] += dipole[2];
	    counts[dd][countFrame] += 1;
	  }
	}
	countFrame ++;
      }
    }
    fclose (fpa);
    fclose (fpd);
  }
  
  FILE * fp = fopen (ofile.c_str(), "w");
  fprintf (fp, "# time   dipole A_1   dipole A_2   dipole B_1   dipole B_2   dipole C\n");
  for (unsigned ii = 0; ii < times.size(); ++ii){
    fprintf (fp, "%f ", times[ii]);
    for (unsigned dd = 0; dd < sets.size(); ++dd){
      fprintf (fp, "%f %f %f   ",
	       values[dd][ii][0] / double(counts[dd][ii]),
	       values[dd][ii][1] / double(counts[dd][ii]),
	       values[dd][ii][2] / double(counts[dd][ii])
	  );
    }
    fprintf (fp, "\n");
  }
  fclose (fp);
  
  return 0;
}

