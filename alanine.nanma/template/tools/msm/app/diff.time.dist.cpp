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

double diff_dist (const vector<double > & dist0,
		  const vector<double > & dist1)
{
  double sum = 0;
  for (unsigned ii = 0; ii < dist0.size(); ++ii){
    sum += fabs(dist0[ii] - dist1[ii]);
  }
  // sum = sum / double(dist0.size());
  // printf ("%f\n", sum);
  return sum;
}	  


int main(int argc, char * argv[])
{
  std::string ofile, ifile0, ifile1, isfile;
  
  po::options_description desc ("Calculates the steady distribution. The user should be sure that the trajs are in steady state.\nAllow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input-dist-bf", po::value<string > (&ifile0)->default_value ("0.out"), "the traj name 0.")
    ("input-largest-set,s", po::value<std::string > (&isfile)->default_value ("largestSet"), "the input file of largest set.")
      ("input-dist-cg", po::value<string > (&ifile1)->default_value ("1.out"), "the traj name 1.")
      ("output", po::value<string > (&ofile)->default_value ("diff.dist.out"), "the output of forward commitor.");
  
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

  ifstream fpname0 (ifile0.c_str());
  if (!fpname0){
    std::cerr << "cannot open file " << ifile0 << std::endl;
    return 1;
  }
  ifstream fpname1 (ifile1.c_str());
  if (!fpname1){
    std::cerr << "cannot open file " << ifile1 << std::endl;
    return 1;
  }
  char nameline0 [MaxLineLength];
  char nameline1 [MaxLineLength];
  
  FILE * fp = fopen (ofile.c_str(), "w");
  if (fp == NULL){
    cerr << "cannot open file " << ofile << endl;
    return 1;
  }
  while (fpname0.getline(nameline0, MaxLineLength) &&
	 fpname1.getline(nameline1, MaxLineLength) ){
    double time0, time1;
    vector<double > dist0, dist1tmp, dist1;
    vector<string > words;
    StringOperation::split (nameline0, words);
    time0 = atof(words[0].c_str());
    for (unsigned ii = 1; ii < words.size(); ++ii){
      dist0.push_back (atof(words[ii].c_str()));
    }
    dist1.clear ();
    dist1.resize(dist0.size(), 0.0);
    StringOperation::split (nameline1, words);
    time1 = atof(words[0].c_str());
    for (unsigned ii = 1; ii < words.size(); ++ii){
      dist1tmp.push_back (atof(words[ii].c_str()));
    }
    if (dist1tmp.size() != setMap.size()){
      cerr << "unmatching setmap and cg distribution" << endl;
      return 1;
    }
    for (unsigned ii = 0; ii < dist1tmp.size(); ++ii){
      dist1[setMap[ii]] = dist1tmp[ii];
    }
    // double sum0 = 0;
    // double sum1 = 0;
    // for (unsigned ii = 0; ii < dist0.size(); ++ii){
    //   sum0 += dist0[ii];
    //   sum1 += dist1[ii];
    // }
    // printf ("%f %f %f\n", time0, sum0, sum1);
    if (time0 != time1){
      cerr << "times are not consistent" << endl;
      return 1;
    }
    double diff = diff_dist(dist0, dist1);
    fprintf (fp, "%f %e\n", time0, diff);
  }  
  fclose (fp);

  return 0;
}
