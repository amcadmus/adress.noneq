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

#include "Trajectory.h"
#include "StringSplit.h"

#define MaxLineLength 65536

namespace po = boost::program_options;
using namespace std;



int main(int argc, char * argv[])
{
  std::string ofile, ifile;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input,f", po::value<std::string > (&ifile)->default_value ("traj.txt"), "the input xtc file.")
      ("output,o", po::value<std::string > (&ofile)->default_value ("traj.xtc"), "the output of the trajectory");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  XDRFILE * xd = xdrfile_open (ofile.c_str(), "w");
  if (xd == NULL){
    cerr << "cannot open file " << ofile << endl;
    return 1;
  }
  float dt = 1.;
  matrix box;
  box[0][0] = box[1][1] = box[2][2] = 10.;
  box[0][1] = box[0][2] = box[1][0] = box[1][2] = box[2][0] = box[2][1] = 0.;
  float prec = 1000.;
  
  ifstream fpname (ifile.c_str());
  if (!fpname){
    std::cerr << "cannot open file " << ifile << std::endl;
    exit(1);
  }
  char valueline [MaxLineLength];
  int countLine = 0;
  while (fpname.getline(valueline, MaxLineLength)){
    vector<string > words;
    StringOperation::split (string(valueline), words);
    unsigned natom = words.size() / 3;
    if (natom * 3 != words.size()){
      cerr << "format error of file " << ifile << endl;
      return 1;
    }
    rvec * xx = (rvec *) malloc (sizeof(rvec) * natom);
    for (unsigned ii = 0; ii < natom; ++ii){
      xx[ii][0] = atof(words[ii*3+0].c_str());
      xx[ii][1] = atof(words[ii*3+1].c_str());
      xx[ii][2] = atof(words[ii*3+2].c_str());      
    }
    write_xtc (xd, natom, countLine, countLine * dt, box, xx, prec);
    free (xx);
    countLine++;
  }
  
  return 0;
}
