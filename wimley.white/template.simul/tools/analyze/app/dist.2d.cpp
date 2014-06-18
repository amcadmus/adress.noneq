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
#include <cmath>
#include <boost/program_options.hpp>
#include <vector>
#include "StringSplit.h"
#include "BlockAverage.h"

#define MaxLineLength 2048

namespace po = boost::program_options;
using namespace std;

int main(int argc, char * argv[])
{
  std::string ofile, phifile, psifile;
  unsigned column, nBin, nDataInBlock;
  double aup, alow;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input-list-phi-files,f", po::value<std::string > (&phifile)->default_value ("angaver.phi.xvg"), "the input files of dihedral angles phi.")
      ("input-list-psi-files,f", po::value<std::string > (&psifile)->default_value ("angaver.psi.xvg"), "the input files of dihedral angles psi.")
      ("column,c", po::value<unsigned > (&column)->default_value (2), "the column used.")
      ("num-data-block", po::value<unsigned > (&nDataInBlock)->default_value (320), "number data in blocks.")
      ("num-bins,n", po::value<unsigned > (&nBin)->default_value (72), "number bins for distribution.")
      ("angle-up", po::value<double > (&aup)->default_value (180.), "upper bond of the angle.")
      ("angle-low", po::value<double > (&alow)->default_value (-180.), "lower bond of the angle.")
      ("output,o", po::value<std::string > (&ofile)->default_value ("dist.out"), "the output of the discrete trajectory");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  double binSize = (aup - alow) / double(nBin);
  double binSizeI = 1./binSize;
  
  vector<vector<BlockAverage_acc > > avgs (nBin);
  for (unsigned ii = 0; ii < nBin; ++ii){
    avgs[ii].resize (nBin);
    for (unsigned jj = 0; jj < nBin; ++jj){
      avgs[ii][jj].reinit (nDataInBlock);
    }
  }
  
  vector<string > phiwords;
  StringOperation::split (phifile, phiwords);
  vector<string > psiwords;
  StringOperation::split (psifile, psiwords);
  if (phiwords.size() != psiwords.size()){
    cerr << "should provide the same number of phi and psi files with the same format" << endl;
    return 1;
  }
  
  for (unsigned ii = 0; ii < phiwords.size(); ++ii){
    ifstream phidata (phiwords[ii].c_str());
    ifstream psidata (psiwords[ii].c_str());
    if (!phidata){
      cerr << "cannot open angle file " << phiwords[ii] << endl;
      return 1;
    }
    if (!psidata){
      cerr << "cannot open angle file " << psiwords[ii] << endl;
      return 1;
    }
    cout << "calculate with dih traj " << phiwords[ii] << " and " << psiwords[ii] << endl;
    char phivalueline [MaxLineLength];  
    char psivalueline [MaxLineLength];  
    while (phidata.getline(phivalueline, MaxLineLength) &&
	   psidata.getline(psivalueline, MaxLineLength) ){
      if (phivalueline[0] == '#' || phivalueline[0] == '@' ||
	  psivalueline[0] == '#' || psivalueline[0] == '@'){
	cerr << "the angle file should not start with comments" << endl;
	return 1;
      }
      vector<string > words;
      StringOperation::split (string(phivalueline), words);
      if (words.size() < column) {
	cerr << "wrong file format of " << phifile << endl;
	exit (1);
      }
      double phivalue = atof(words[column-1].c_str());
      int phiindexes = (phivalue - alow) * binSizeI;
      if (phiindexes < 0) phiindexes += int(nBin);
      else if (phiindexes >= int(nBin)) phiindexes -= int(nBin);
      StringOperation::split (string(psivalueline), words);
      if (words.size() < column) {
	cerr << "wrong file format of " << psifile << endl;
	exit (1);
      }
      double psivalue = atof(words[column-1].c_str());
      int psiindexes = (psivalue - alow) * binSizeI;
      if (psiindexes < 0) psiindexes += int(nBin);
      else if (psiindexes >= int(nBin)) psiindexes -= int(nBin);

      for (unsigned jj = 0; jj < nBin; ++jj){
	for (unsigned kk = 0; kk < nBin; ++kk){
	  if (int(jj) == phiindexes && int(kk) == psiindexes){
	    avgs[jj][kk].deposite (1.);
	  }
	  else {
	    avgs[jj][kk].deposite (0.);
	  }
	}
      }
      // countTot ++;
    }
  }

  for (unsigned ii = 0; ii < nBin; ++ii) {
    for (unsigned jj = 0; jj < nBin; ++jj) {
      avgs[ii][jj].calculate ();
    }
  }

  FILE * fp = fopen (ofile.c_str(), "w");
  if (fp == NULL){
    cerr << "cannot open file " << ofile << endl;
    return 1;
  }

  for (unsigned ii = 0; ii < nBin; ++ii) {
    double xx = alow + (ii+0.5) * binSize;
    for (unsigned jj = 0; jj < nBin; ++jj) {
      double yy = alow + (jj+0.5) * binSize;
      double vv = avgs[ii][jj].getAvg() / double(1) * binSizeI * binSizeI;
      double ee = avgs[ii][jj].getAvgError() / double(1) * binSizeI * binSizeI;
      fprintf (fp, "%f %f %e %e\n", xx, yy, vv, ee);
    }
    fprintf (fp, "\n");
  }

  return 0;
}


