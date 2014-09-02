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
#include "MetaSet.h"

#define MaxLineLength 65536

namespace po = boost::program_options;
using namespace std;

void load_committor (const string & file,
		     vector<vector<double > > & fwq)
{
  ifstream fpname (file.c_str());
  if (!fpname){
    std::cerr << "cannot open file " << file << std::endl;
    exit( 1);
  }
  char nameline [MaxLineLength];
  unsigned countFile = 0;
  fwq.clear();
  while (fpname.getline(nameline, MaxLineLength)){
    if (nameline[0] == '#') continue;
    vector<string > words;
    StringOperation::split (nameline, words);
    if (countFile == 0){
      fwq.resize (words.size());
    }
    else {
      if (fwq.size() != words.size()){
	cerr << "format error of committor at line: " << nameline << endl;
	exit (1);
      }
    }
    for (unsigned ii = 0; ii < words.size(); ++ii){
      fwq[ii].push_back (atof(words[ii].c_str()));
    }
  }
}

void read_tmatrix (const string & filename,
		   vector<vector<double > > & tmatrix,
		   unsigned & nstate)
{
  ifstream fpname (filename.c_str());
  if (!fpname){
    std::cerr << "cannot open file " << filename << std::endl;
    exit(1);
  }
  char valueline [MaxLineLength];
  int countLine = 0;
  while (fpname.getline(valueline, MaxLineLength)){
    vector<string > words;
    StringOperation::split (string(valueline), words);
    if (countLine == 0){
      nstate = words.size();
      tmatrix.resize(nstate);
      for (unsigned ii = 0; ii < nstate; ++ii){
	tmatrix[ii].resize(nstate);
      }
    }
    if (words.size() != nstate){
      cerr << "format error of tmatrix reading " << endl;
      exit (1);
    }
    for (unsigned ii = 0; ii < nstate; ++ii){
      tmatrix[countLine][ii] = atof(words[ii].c_str());
    }
    countLine ++;
  }
}


int main(int argc, char * argv[])
{
  std::string isfile, ibwfile, ofile, ifile;
  
  po::options_description desc ("Calculates the coreset projection of time-dep prob..\nAllow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input", po::value<string > (&ifile)->default_value ("bf.prob.dih.out"), "the time-dependent prob..")
      ("input-largest-set,s", po::value<std::string > (&isfile)->default_value ("largestSet"), "the input file of largest set.")
      ("input-bw", po::value<string > (&ibwfile)->default_value ("commitor.bw.out.orig"), "the input of backward commitor.")
      ("output", po::value<string > (&ofile)->default_value ("coreset.prob.out"), "the output prob of coreset MSM.");
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }  

  vector<int > setMap ;
  {
    FILE * fp  = fopen (isfile.c_str(), "r");
    if (fp == NULL){
      cerr << "cannot open file " << isfile << endl;
      return 1;
    }
    int tmpint;
    while (fscanf(fp, "%d", &tmpint) == 1){
      setMap.push_back (tmpint);
    }
    fclose (fp);
  }
  unsigned nstate = setMap.size();

  vector<vector<double > > bwq;
  load_committor (ibwfile, bwq);
  unsigned ncluster = bwq.size();
  unsigned nstate1 = bwq[0].size();
  if (nstate1 != nstate) {
    cerr << "inconsistent between the setmap and the committor" << endl;
    return 1;
  }

  ifstream fpname (ifile.c_str());
  if (!fpname){
    std::cerr << "cannot open file " << ifile << std::endl;
    exit(1);
  }
  char valueline [MaxLineLength];

  FILE * fpo = fopen(ofile.c_str(), "w");
  if (fpo == NULL){
    cerr << "cannot open file " << ofile << endl;
    return 1;
  }
  while (fpname.getline(valueline, MaxLineLength)){
    vector<string > words;
    StringOperation::split (string(valueline), words);
    // if (words.size() != nstate+1){
    //   cerr << "format error at t-d prob." << endl;
    //   return 1;
    // }
    double time;
    vector<double > tmpDist;
    vector<double > projDist (ncluster, 0.);
    time = atof(words[0].c_str());
    for (unsigned ii = 0; ii < words.size()-1; ++ii){
      tmpDist.push_back(atof(words[ii+1].c_str()));
    }
    vector<double > nowDist (nstate);
    for (unsigned ii = 0; ii < nstate; ++ii){
      if (unsigned(setMap[ii]) >= tmpDist.size()) {
	cerr << "format error of the input t-d prob" << endl;
	return 1;
      }
      nowDist[ii] = tmpDist[setMap[ii]];
    }
    for (unsigned ii = 0; ii < ncluster; ++ii){
      for (unsigned jj = 0; jj < nstate; ++jj){
	projDist[ii] += bwq[ii][jj] * nowDist[jj];
      }
    }
    fprintf (fpo, "%f ", time);
    for (unsigned ii = 0; ii < ncluster; ++ii){
      fprintf (fpo, "%e ", projDist[ii]);
    }
    fprintf (fpo, "\n");
  }
  fclose (fpo);
  
  return 0;
}
