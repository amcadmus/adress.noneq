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
#include "StringSplit.h"
#include "BlockAverage.h"
#include "AutoCorrelCalculator.h"

#define MaxLineLength 2048

namespace po = boost::program_options;
using namespace std;

int main(int argc, char * argv[])
{
  std::string ofile, ifile;
  unsigned columnd, columnt, nDataBlock;
  double upTime;
  
  po::options_description desc ("Autocorrelation calculator.\nAllow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input,f", po::value<std::string > (&ifile)->default_value ("data"), "the input file.")
      ("column-time,t", po::value<unsigned > (&columnt)->default_value (1), "the column of time used.")
      ("column-data,d", po::value<unsigned > (&columnd)->default_value (2), "the column of data used.")
      ("up-time,u", po::value<double > (&upTime)->default_value (10), "the upper time limit.")
      ("num-data-block,n", po::value<unsigned > (&nDataBlock)->default_value (256), "number of blocks.")
      ("output,o", po::value<std::string > (&ofile)->default_value ("autoCorrelation.out"), "the output of the auto correlation");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  ifstream data0 (ifile.c_str());
  ifstream data  (ifile.c_str());
  if (!data0 || !data){
    cerr << "cannot open angle file " << ifile << endl;
    return 1;
  }
  char valueline [MaxLineLength];

  int countRead = 0;
  vector<double > times;
  while (data0.getline(valueline, MaxLineLength)){
    if (valueline[0] == '#' || valueline[0] == '@'){
      continue;
    }
    vector<string > words;
    StringOperation::split (string(valueline), words);
    if (words.size() < columnt) {
      cerr << "wrong file format of " << ifile << endl;
      exit (1);
    }
    times.push_back (atof(words[columnt - 1].c_str()));
    countRead++;
    if (countRead == 2){
      break;
    }
  }
  double dt = times[1] - times[0];
  int ndata = upTime / dt + 1;

  AutoCorrelCalculator acc (ndata, nDataBlock);
	
  countRead = 0;
  while (data.getline(valueline, MaxLineLength)){
    if (valueline[0] == '#' || valueline[0] == '@'){
      continue;
    }
    if (countRead % 10000 == 0){
      printf ("# read at time %f    \r", countRead * dt);
      fflush (stdout);
    }
    countRead ++;
    vector<string > words;
    StringOperation::split (string(valueline), words);
    if (words.size() < columnd) {
      cerr << "wrong file format of " << ifile << endl;
      exit (1);
    }
    double value = atof(words[columnd-1].c_str());
    acc.push_back (value);
  }
  printf ("\n");

  acc.calculate ();

  printf ("# detacted dt: %f\n", dt);
  printf ("# minimum num data used: %d with %d blocks. 60 percent confidence level\n", acc.getBlockAverage (ndata-1).getNumDataUsed(), acc.getBlockAverage (ndata-1).getNumDataUsed()/nDataBlock);

  FILE * fp = fopen (ofile.c_str(), "w");
  fprintf (fp, "# avg \t avg_err \t var \t var_err\n");
  for (int ii = 0; ii < ndata; ++ii){
    fprintf (fp, "%f \t %e \t %e\n",
	     ii * dt, acc.value(ii), acc.error(ii));	
  }
  fclose (fp);

  
  return 0;
}
