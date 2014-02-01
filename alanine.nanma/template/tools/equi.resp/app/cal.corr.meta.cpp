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
namespace po = boost::program_options;
#define MaxLineLength 10240

#include "StringSplit.h"
#include "BlockAverage.h"
#include "Distribution.h"

using namespace std;

bool myread (FILE * fp,
	     float & time,
	     double & phi,
	     double & psi)
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
  float begin, end;
  std::string afile, ffile, ofile;
  float time_prec = .01;
  unsigned nblock;
  double corrLength;
  unsigned every;

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
      ("begin,b", po::value<float > (&begin)->default_value(0.f), "start time")
      ("end,e",   po::value<float > (&end  )->default_value(0.f), "end   time")
      ("every",   po::value<unsigned > (&every  )->default_value(1), "skip every steps")
      ("corr-length,l",   po::value<double > (&corrLength)->default_value(1.), "correlation length")
      ("nblock",   po::value<unsigned > (&nblock)->default_value(30), "number of blocks")
      ("angle-file,a",po::value<std::string > (&afile)->default_value ("angle.dat"), "angle file")
      ("flux-file,f",  po::value<std::string > (&ffile)->default_value ("gxs.out"), "flux file")
      ("output,o",   po::value<std::string > (&ofile)->default_value ("corr.meta.out"), "out put file of corr for metastable sets");
      
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }
  
  std::cout << "###################################################" << std::endl;
  std::cout << "# begin->end:  " << begin << " " << end << std::endl;
  std::cout << "# angle file:  " << afile << std::endl;
  std::cout << "# flux file:   " << ffile << std::endl;
  std::cout << "# corr length: " << corrLength << std::endl;
  std::cout << "# out file:    " << ofile << std::endl;
  std::cout << "###################################################" << std::endl;  

  vector<MetastableSet > sets;
  sets.push_back (MetastableSet(setA1_psi_b, setA1_psi_e, setA1_phi_b, setA1_phi_e));
  sets.push_back (MetastableSet(setA2_psi_b, setA2_psi_e, setA2_phi_b, setA2_phi_e));
  sets.push_back (MetastableSet(setB1_psi_b, setB1_psi_e, setB1_phi_b, setB1_phi_e));
  sets.push_back (MetastableSet(setB2_psi_b, setB2_psi_e, setB2_phi_b, setB2_phi_e));
  sets.push_back (MetastableSet(setC_psi_b, setC_psi_e, setC_phi_b, setC_phi_e));

  char line [MaxLineLength];
  vector<double > times;
  float time;
  vector<vector<double > > counts;
  vector<double > fluxes;

  ifstream fpf (ffile.c_str());
  if (!fpf){
    std::cerr << "cannot open file " << ffile << std::endl;
    return 1;
  }
  int tmpCountRead = 0;
  while (fpf.getline(line, MaxLineLength)){
    if (line[0] == '#') continue;
    if (line[0] == '@') continue;
    if ((tmpCountRead ++) % every != 0){
      continue;
    }
    vector<std::string > words;
    StringOperation::split (string(line), words);
    if (end != 0 && atof(words[0].c_str()) > end + time_prec) {
      break;
    } 
    times.push_back (atof(words[0].c_str()));
    fluxes.push_back (atof(words[1].c_str()));
  }
  FILE *fp = fopen (afile.c_str(), "r");
  if (fp == NULL){
    std::cerr << "cannot open file " << afile << std::endl;
    return 1;
  }
  double phi, psi;
  tmpCountRead = 0;
  while (myread(fp, time, phi, psi)){
    if (end != 0 && time > end + time_prec) {
      break;
    } 
    if ((tmpCountRead ++) % every != 0){
      continue;
    }
    vector<double > tmpcount;
    depositMetastable (psi, phi, sets, tmpcount);
    counts.push_back (tmpcount);
  }
  fclose (fp);


  double dt = times[1] - times[0];
  int ncorr = int((corrLength + 0.5 * dt) / dt + 1);
  vector<vector<BlockAverage_acc > > corrs (sets.size());
  for (int ii = 0; ii < int(corrs.size()); ++ii){
    corrs[ii].resize (ncorr);
    for (int jj = 0; jj < ncorr; ++jj){
      corrs[ii][jj].reinit (nblock);
    }
  }

  for (int ii = ncorr; ii < int(times.size()); ++ii){
    if (ii % 1000 == 0){
      printf ("# compute at time %f    \r", times[ii]);
      fflush (stdout);
    }
    if (end != 0.f) {
      if (times[ii] < begin - time_prec){
        continue;
      }
      else if (times[ii] > end + time_prec) {
        break;
      } 
    }
    else {
      if (times[ii] < begin - time_prec) continue;
    }
    for (int jj = 0; jj < int(sets.size()); ++jj){
      for (int kk = 0; kk < ncorr; ++kk){
	corrs[jj][kk].deposite (counts[ii][jj] * fluxes[ii-kk]);
      }
    }
  }
  printf ("\n");

  for (int ii = 0; ii < int(corrs.size()); ++ii){
    for (int jj = 0; jj < ncorr; ++jj){
      corrs[ii][jj].calculate();
    }
  }

  fp = fopen (ofile.c_str(), "w");
  for (int jj = 0; jj < ncorr; ++jj){
    fprintf (fp, "%f   ", dt * jj);
    for (int ii = 0; ii < int(corrs.size()); ++ii){
      fprintf (fp, "%e %e   ", corrs[ii][jj].getAvg(), corrs[ii][jj].getAvgError());
    }
    fprintf (fp, "\n");
  }
  fclose (fp);
  
  return 0;
}

