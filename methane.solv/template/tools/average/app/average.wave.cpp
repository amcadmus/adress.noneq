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
#define MaxLineLength 10240

#include "StringSplit.h"
namespace po = boost::program_options;
using namespace std;

void myread (void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  size_t nread = fread (ptr, size, nmemb, stream);
  if (nread != nmemb){
    std::cerr << "cannot open file " << std::endl;
    exit (1);
  }
}


int main(int argc, char * argv[])
{
  std::string ifile, ofile;

  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("output,o", po::value<std::string > (&ofile)->default_value ("avg.wave.out"), "the output of wave")
      ("input,f",  po::value<std::string > (&ifile)->default_value ("waves.name"), "the file of file names");
      
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  ifstream fpname (ifile.c_str());
  if (!fpname){
    std::cerr << "cannot open file " << ifile << std::endl;
    return 1;
  }
  char nameline [MaxLineLength];
  vector<double > grid;
  vector<double > time;
  vector<vector<double> > value;
  unsigned gridSize = 0;
  size_t tmpGridSize;
  float * rr;
  float * vv;
  unsigned countFile = 0;
  
  while (fpname.getline(nameline, MaxLineLength)){
    if (nameline[0] == '#') continue;
    FILE * fptraj = fopen (nameline, "r");
    if (fptraj == NULL){
      cout << "cannot open file " << nameline << endl;
      return 1;
    }
    countFile ++;
    if (countFile == 1){
      myread (&tmpGridSize, sizeof(size_t), 1, fptraj);
      gridSize = tmpGridSize;
      rr = (float *) malloc (sizeof(float) * gridSize);
      vv = (float *) malloc (sizeof(float) * gridSize);      
      myread (rr, sizeof(float), gridSize, fptraj);
      grid.resize (gridSize);
      for (unsigned ii = 0; ii < gridSize; ++ii){
	grid[ii] = rr[ii];
      }
      float mytime;
      while (fread (&mytime, sizeof(float), 1, fptraj) == 1){
	time.push_back (mytime);
	myread (vv, sizeof(float), gridSize, fptraj);
	vector<double > tmpvalue (gridSize);
	for (unsigned ii = 0; ii < gridSize; ++ii){
	  tmpvalue[ii] = vv[ii];
	}
	value.push_back (tmpvalue);
      }      
    }
    else {
      myread (&tmpGridSize, sizeof(size_t), 1, fptraj);
      myread (rr, sizeof(float), gridSize, fptraj);
      if (tmpGridSize != gridSize){
	cerr << "in consistent grid size of file " << nameline << endl;
	return 1;
      }
      float mytime;
      unsigned countTimeSlice = 0;
      while (fread (&mytime, sizeof(float), 1, fptraj) == 1){
	time.push_back (mytime);
	myread (vv, sizeof(float), gridSize, fptraj);
	if (countTimeSlice >= time.size()){
	  cerr << "wrong time frame of file " << nameline << endl;
	  return 1;
	}
	for (unsigned ii = 0; ii < gridSize; ++ii){
	  value[countTimeSlice][ii] += vv[ii];
	}
	countTimeSlice ++;
      }
    }
    fclose (fptraj);
  }


  for (unsigned ii = 0; ii < time.size(); ++ii){
    double tmp = time[ii];
    int tmpint = int (tmp * 100 + 0.5) / 100;
    int tmpint2 = int (tmp * 100 + 0.5) - tmpint * 100;
    sprintf (nameline, "%s.%05d.%02d", ofile.c_str(), tmpint, tmpint2);
    FILE * fp = fopen (nameline, "w");
    if (fp == NULL){
      cerr << "cannot open file " << nameline << endl;
      return 1;
    }
    for (unsigned jj = 0; jj < gridSize; ++jj){
      fprintf (fp, "%e %e\n", time[ii], value[ii][jj] / double(countFile));
    }
    fclose (fp);
  }
  
  return 0;
}
