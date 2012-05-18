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
#include "GroFileManager.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char * argv[])
{
  std::string ofile;
  std::string rname, cname, hname;
  float roh;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("roh,r", po::value<float > (&roh)->default_value(0.09572), "r_{OH}")
      ("rname",   po::value<std::string > (&rname)->default_value ("Meth"), "name of methane")
      ("hname",   po::value<std::string > (&hname)->default_value ("HC"), "name of H")
      ("cname",   po::value<std::string > (&cname)->default_value ("CT"), "name of C")
      ("output,f",   po::value<std::string > (&ofile)->default_value ("out.gro"), "the output .gro file");
      
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  
  std::vector<int > resdindex;
  std::vector<std::string >  resdname;
  std::vector<std::string >  atomname;
  std::vector<int > atomindex;
  std::vector<std::vector<double > > posi;
  std::vector<std::vector<double > > velo;
  std::vector<double > boxsize;
  
  double aa = roh * 2. / sqrt(3.);
  boxsize.resize(3, 2.1*aa);
  std::vector<double > shift(boxsize);
  for (unsigned dd = 0; dd < 3; ++dd){
    shift[dd] *= .5;
    shift[dd] = shift[dd] - .5 * aa;
  }
  std::vector<double > tmp (3,0.);
  velo.push_back (tmp);
  velo.push_back (tmp);
  velo.push_back (tmp);
  velo.push_back (tmp);
  velo.push_back (tmp);
  tmp[0] = tmp[1] = tmp[2] = 0.5 * aa;
  posi.push_back(tmp);
  tmp[0] = tmp[1] = tmp[2] = 0.;
  posi.push_back(tmp);
  tmp[0] = tmp[1] = aa;
  tmp[2] = 0;
  posi.push_back(tmp);
  tmp[0] = tmp[2] = aa;
  tmp[1] = 0;
  posi.push_back(tmp);
  tmp[1] = tmp[2] = aa;
  tmp[0] = 0;
  posi.push_back(tmp);
  for (unsigned ii = 0; ii < 5; ++ii){
    for (unsigned dd = 0; dd < 3; ++dd){
      posi[ii][dd] += shift[dd];
    }
  }

  resdindex.push_back (1);
  resdindex.push_back (1);
  resdindex.push_back (1);
  resdindex.push_back (1);
  resdindex.push_back (1);
  atomindex.push_back (1);
  atomindex.push_back (2);
  atomindex.push_back (3);
  atomindex.push_back (4);
  atomindex.push_back (5);
  resdname.push_back (rname);
  resdname.push_back (rname);
  resdname.push_back (rname);
  resdname.push_back (rname);
  resdname.push_back (rname);
  atomname.push_back (cname);
  atomname.push_back (hname+std::string("1"));
  atomname.push_back (hname+std::string("2"));
  atomname.push_back (hname+std::string("3"));
  atomname.push_back (hname+std::string("4"));
  
  GroFileManager::write (ofile, resdindex, resdname, atomname, atomindex,
			 posi, velo, boxsize);
  
  return 0;
}
