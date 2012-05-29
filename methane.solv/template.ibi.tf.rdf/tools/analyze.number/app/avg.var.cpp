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
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("every,e", po::value<unsigned > (&every)->default_value (1),  "cal average every * numbers")
      ("col,c", po::value<unsigned > (&col)->default_value(1), "colume to analyze")
      ("nblock,n", po::value<unsigned > (&numBlocks)->default_value(16), "number of blocks")
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

  printf ("# avg  avg_error  var  var_error\n");
  printf ("%e  %e  %e  %e\n",
	  ba.getAvg(), ba.getAvgError(),
	  ba.getVar(), ba.getVarError());

  fclose (fp);
  
  return 0;
}

