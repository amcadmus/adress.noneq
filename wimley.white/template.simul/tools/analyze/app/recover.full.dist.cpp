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

int conv_multi_single (const vector<int > & indexes,
		       const int & nBin)
{
  int result = 0;
  for (unsigned ii = 0; ii < indexes.size(); ++ii){
    result = result * nBin + indexes[ii];
  }
  return result;
}

vector<int> conv_single_multi (const int & idx,
			       const int & nfold,
			       const int & nBin)
{
  vector<int > result (nfold);
  int value = idx;
  int posi = nfold-1;
  for (int ii = 0; ii < nfold; ++ii){
    result[posi] = value % nBin;
    value = (value - result[posi]) / nBin;
    posi --;
  }
  return result;
}


int main(int argc, char * argv[])
{
  std::string ofile, ifile, isfile;
  unsigned column, nBin;
  double aup, alow;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input,f", po::value<std::string > (&ifile)->default_value ("input.out"), "the input file of data.")
      ("input-largest-set,s", po::value<std::string > (&isfile)->default_value ("largestSet"), "the input file of largest set.")
      ("column,c", po::value<unsigned > (&column)->default_value (2), "the column used.")
      ("num-bin,n", po::value<unsigned > (&nBin)->default_value (20), "number of blocks.")
      ("angle-up", po::value<double > (&aup)->default_value (180.), "upper bond of the angle.")
      ("angle-low", po::value<double > (&alow)->default_value (-180.), "lower bond of the angle.")
      ("output,o", po::value<std::string > (&ofile)->default_value ("output.out"), "the output of the dist");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  double binSize = (aup - alow) / double(nBin);

  vector<int > setMap ;
  int tmpint;
  FILE * fp  = fopen (isfile.c_str(), "r");
  while (fscanf(fp, "%d", &tmpint) == 1){
    setMap.push_back (tmpint);
  }
  fclose (fp);
  if (setMap.size() > unsigned (nBin * nBin)){
    cerr << "size of set map and the size of grid does not match, do nothing" << endl;
    return 1;
  }

  vector<double > values;
  ifstream data (ifile.c_str());
  if (!data){
    cerr << "cannot open angle file " << data << endl;
    return 1;
  }
  char valueline [MaxLineLength];
  int count = 0;
  while (data.getline(valueline, MaxLineLength)){
    if (valueline[0] == '#' || valueline[0] == '@'){
      continue;
    }
    if (++count == 1) continue;
    vector<string > words;
    StringOperation::split (string(valueline), words);
    if (words.size() < column) {
      cerr << "wrong file format of " << ifile << endl;
      exit (1);
    }
    values.push_back (atof(words[column-1].c_str()));
  }
  if (values.size() != setMap.size()){
    cerr << "size of values does not match the map, do nothing. " << endl;
    return 1;
  }

  vector<vector<double > > dist (nBin);
  for (unsigned ii = 0; ii < nBin; ++ii){
    dist[ii].resize (nBin, 0.);
  }
  for (int ii = 0; ii < int(setMap.size()); ++ii){
    vector<int > idx = conv_single_multi (setMap[ii], 2, nBin);
    dist[idx[0]][idx[1]] = values [ii];
  }

  for (unsigned ii = 0; ii < nBin; ++ii){
    for (unsigned jj = 0; jj < nBin; ++jj){
      vector<int> old_indexes(2);
      old_indexes[0] = ii;
      old_indexes[1] = jj;
      int index = conv_multi_single (old_indexes, nBin);
      vector<int > new_indexes = conv_single_multi (index, 2, nBin);
      if (new_indexes[0] != old_indexes[0] ||
	  new_indexes[1] != old_indexes[1]){
	cerr << "check not pass!" << endl;
	return 1;
      }
    }
  }
  
  
  fp = fopen (ofile.c_str(), "w");
  for (unsigned ii = 0; ii < nBin; ++ii){
    double xx = (ii + 0.5) * binSize + alow;
    for (unsigned jj = 0; jj < nBin; ++jj){
      double yy = (jj + 0.5) * binSize + alow;
      fprintf (fp, "%f %f %f\n", xx, yy, dist[ii][jj]);
    }
    fprintf (fp, "\n");
  }
  fclose (fp);
  
  return 0;
}
