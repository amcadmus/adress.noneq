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

namespace po = boost::program_options;

#define MaxLineLength 10240

int main(int argc, char * argv[])
{
  unsigned every, col, numBlocks;
  std::string ifile;
  double binsize, boxy, boxz;
  double temperature;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("every,e", po::value<unsigned > (&every)->default_value (1),  "cal average every * numbers")
      ("col,c", po::value<unsigned > (&col)->default_value(1), "colume to analyze")
      ("nblock,n", po::value<unsigned > (&numBlocks)->default_value(16), "number of blocks")
      ("bin-size", po::value<double > (&binsize)->default_value(1.), "size of the bin")
      ("boxy", po::value<double > (&boxy)->default_value(1.), "y of the box")
      ("boxz", po::value<double > (&boxz)->default_value(1.), "z of the box")
      ("temperature", po::value<double > (&temperature)->default_value(300.), "temperature")
      ("input,f",   po::value<std::string > (&ifile)->default_value ("number.out"), "input number file");
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }
  
  int countLine = 0;
  char line [MaxLineLength];
  FILE *fp = fopen (ifile.c_str(), "r");
  if (fp == NULL){
    std::cerr << "cannot open file " << ifile << std::endl;
    exit (1);
  }

  std::vector<double > values;
  
  while (fgets(line, MaxLineLength, fp) != NULL){
    if (line[0] == '#') continue;
    if ((countLine ++) % every == 0){
      std::vector<std::string > words;
      StringOperation::split (std::string(line), words);
      if (words.size() < col) {
	printf ("format error at line %d\n", countLine+1);
	exit(1);
      }
      values.push_back (atof(words[col-1].c_str()));
    }
  }
  
  printf ("#############################################################\n");
  printf ("# col:       %d\n", col);
  printf ("# every:     %d\n", every);
  printf ("# num line:  %d\n", countLine);
  printf ("# num date:  %d\n", int(values.size()));
  printf ("# nblock:    %d\n", numBlocks);
  printf ("#############################################################\n");
  
  BlockAverage ba;
  ba.processData (values, numBlocks);

  // printf ("# avg  avg_error  var  var_error\n");
  // printf ("%e  %e  %e  %e\n",
  // 	  ba.getAvg(), ba.getAvgError(),
  // 	  ba.getVar(), ba.getVarError());

  double n_avg = ba.getAvg();
  double n_avg_error = 2. * ba.getAvgError();
  double n_var = ba.getVar();
  double n_var_error = 2. * ba.getVarError();

  double volume = binsize * boxy * boxz;
  double rho = n_avg / volume;
  double rho_error = n_avg_error / volume;
  rho *= 18e-3 / (6.02e23 * 1e-27);
  rho_error *= 18e-3 / (6.02e23 * 1e-27);
  double comp = n_var / (n_avg * n_avg) * volume / (1.38 *temperature) * 1e-4;
  double comp_error =
      n_var_error / (n_avg * n_avg) +
      2 * n_var * n_avg_error / (n_avg * n_avg * n_avg);
  comp_error *= volume / (1.38 *temperature) * 1e-4;
  printf ("# rho  rho_error  comp  comp_error n_avg (e) n_var (e)\n");
  printf ("%e  %e  %e  %e   %e %e %e %e\n",
	  rho, rho_error,
	  comp, comp_error,
	  n_avg, n_avg_error,
	  n_var, n_var_error);
  
  fclose (fp);
  
  return 0;
}

