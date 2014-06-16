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

#include "GmxTop.h"
#include "GmxType.h"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

using namespace std;

bool
matchKey (const string & key,
	  const string & word)
{
  if (word.size() < key.size()){
    return false;
  }
  for (unsigned ii = 0; ii < key.size(); ++ii){
    if (word[ii] != key[ii]){
      return false;
    }
  }
  return true;
}

bool
findMatch (const vector<string > & array,
	   const string & key,
	   int & idx)
{
  bool find = false;
  for (unsigned ii = 0; ii < array.size(); ++ii){
    if (array[ii] == key){
      find = true;
      idx = int(ii);
      break;
    }
  }
  return find;
}


int main(int argc, char * argv[])
{
  std::string ifile, ofile, cgkey, exkey;

  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("cg-key,c", po::value<std::string > (&cgkey)->default_value ("CM"))
      ("ex-key,e", po::value<std::string > (&exkey)->default_value ("EXW"))
      ("output,o", po::value<std::string > (&ofile)->default_value ("index.ndx"), "the output index")
      ("input,f",  po::value<std::string > (&ifile)->default_value ("topol.top"), "the input top");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  GmxTop::gmx_sys_top systop;
  GmxTop::parseTop (ifile.c_str(), systop);
  // GmxTop::gmx_sys_types systype;
  // GmxTop::parseType (ifile.c_str(), systype);

  vector<string > cgname;
  vector<vector<int > > cgindex;
  vector<int > exindex;
  int countIdx = 1;

  // method is stupid...
  for (unsigned ii = 0; ii < systop.moles.size(); ++ii){
    for (int kk = 0; kk < systop.numMol[ii]; ++kk){
      for (unsigned jj = 0; jj < systop.moles[ii].atoms.size(); ++jj){
	if (matchKey (cgkey, systop.moles[ii].atoms[jj].at_name)){
	  int storeIdx = 0;
	  bool find = findMatch (cgname, systop.moles[ii].atoms[jj].at_name, storeIdx);
	  if (find){
	    cgindex[storeIdx].push_back (countIdx);
	  }
	  else {
	    cgname.push_back (systop.moles[ii].atoms[jj].at_name);
	    vector<int> tmp (1, countIdx);
	    cgindex.push_back (tmp);
	  }
	}
	else {
	  exindex.push_back (countIdx);
	}
	countIdx ++;
      }
    }
  }

  FILE * fp = fopen (ofile.c_str(), "w");
  if (fp == NULL){
    cerr << "cannot open file ofile " << endl;
    return 1;
  }

  fprintf (fp, "[ %s ]\n", exkey.c_str());
  int countLine = 0;
  for (unsigned ii = 0; ii < exindex.size(); ++ii){
    if (countLine == 15){
      countLine = 0;
      fprintf (fp, "\n");
    }
    fprintf (fp, "%d ", exindex[ii]);
    countLine ++;
  }
  fprintf (fp, "\n");

  for (unsigned kk = 0; kk < cgname.size(); ++kk){
    fprintf (fp, "[ %s ]\n", cgname[kk].c_str());
    int countLine = 0;
    for (unsigned ii = 0; ii < cgindex[kk].size(); ++ii){
      if (countLine == 15){
	countLine = 0;
	fprintf (fp, "\n");
      }
      fprintf (fp, "%d ", cgindex[kk][ii]);
      countLine ++;
    }
    fprintf (fp, "\n");
  }
  
  fclose (fp);
  
  return 0;
}
