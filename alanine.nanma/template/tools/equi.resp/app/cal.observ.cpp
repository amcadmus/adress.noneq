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

void readCorrFile (const string & file,
		   vector<double > & times,
		   vector<vector<double > > & corrs,
		   vector<vector<double > > & corrErrors)
{
  times.clear();
  corrs.clear();
  corrErrors.clear();

  ifstream fp (file.c_str());
  if (!fp){
    std::cerr << "cannot open file " << file << std::endl;
    exit(1);
  }
  char line [MaxLineLength];
  int nset = 0;
  while (fp.getline(line, MaxLineLength)){
    if (line[0] == '#') continue;
    if (line[0] == '@') continue;    
    vector<std::string > words;
    StringOperation::split (string(line), words);
    if (times.size() == 0){
      nset = (words.size() - 1) / 2;
      corrs.resize (nset);
      corrErrors.resize (nset);
    }
    times.push_back (atof(words[0].c_str()));
    for (int ii = 0; ii < nset; ++ii){
      corrs[ii].push_back (atof(words[ii*2+1].c_str()));
      corrErrors[ii].push_back (atof(words[ii*2+1].c_str()));
    }
  }  
}

void readEquiFile (const string & file,
		   vector<double > & equis)
{
  equis.clear ();

  FILE * fp = fopen (file.c_str(), "r");
  double tmp;
  while (1 == fscanf(fp, "%lf", &tmp)){
    equis.push_back (tmp);
  }
  fclose (fp);
}

double fe (double time)
{
  if (time < 0.){
    return 0.;
  }
  else if (time < 10.) {
    return time / 10.;
  }
  else {
    return 1.;
  }  
}

int main(int argc, char * argv[])
{
  double begin, step, end;
  std::string cfile, efile, ofile;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("begin,b", po::value<double > (&begin)->default_value(0.f), "start time")
      ("step,s",  po::value<double > (&step )->default_value(1.f), "step time")
      ("end,e",   po::value<double > (&end  )->default_value(100.f), "end   time")
      ("corr-file,f",  po::value<std::string > (&cfile)->default_value ("corr.meta.out"), "the correlation file")
      ("equi-file,q",  po::value<std::string > (&efile)->default_value ("equi.meta.out"), "the equilibrium meta file")
      ("output,o",   po::value<std::string > (&ofile)->default_value ("resp.meta.out"), "the output file");
      
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }
  
  std::cout << "###################################################" << std::endl;
  std::cout << "# begin->end:  " << begin << " " << end << std::endl;
  std::cout << "# corr file:   " << cfile << std::endl;
  std::cout << "# out file:    " << ofile << std::endl;
  std::cout << "###################################################" << std::endl;

  vector<double > equis;
  readEquiFile (efile, equis);
  vector<double > times;
  vector<vector<double > > corrs;
  vector<vector<double > > corrErrors;
  readCorrFile (cfile, times, corrs, corrErrors);
  if (equis.size() != corrs.size()){
    cerr << "inconsistent nset in equi and in corr, exit" << endl;
    return 1;
  }

  double beta = 1./2.4923;
  FILE * fp = fopen (ofile.c_str(), "w");
  double dt = times[1] - times[0];
  for (double ctime = begin; ctime <= end; ctime+=step){
    fprintf (fp, "%f   ", ctime);
    for (int ii = 0; ii < int(corrs.size()); ++ii){ // nset
      double tmp = equis[ii];
      for (int jj = 0; jj < int(corrs[ii].size()) - 1; ++jj){
	tmp += beta * 0.5 * dt * (
	    fe(ctime - times[jj]) * corrs[ii][jj] + 
	    fe(ctime - times[jj+1]) * corrs[ii][jj+1]);
      }
      fprintf (fp, "%f   ", tmp);
    }
    fprintf (fp, "\n");
  }
  fclose (fp);
  
  return 0;
}

