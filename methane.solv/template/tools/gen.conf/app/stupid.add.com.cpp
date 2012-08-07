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
#include <time.h>

#include "GroFileManager.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main (int argc, char * argv[])
{
  std::string ifilename;
  std::string ofilename;
  std::string tfile;
  po::options_description desc ("Allow options");  
  int nMeth;
  
  desc.add_options()
      ("help,h", "print this message")
      ("top-file,t",po::value<std::string > (&tfile)->default_value (std::string("mytop")), "topolgy of the system")
      ("output-file-name,o", po::value<std::string > (&ofilename)->default_value (std::string("out.gro")), "output conf file name")
      ("input-file-name,f",  po::value<std::string > (&ifilename)->default_value (std::string("conf.gro")), "input conf file name");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);

  if (vm.count("help") || vm.count("h")){
    std::cout << desc<< "\n";
    return 0;
  }

  FILE * fp = fopen (tfile.c_str(), "r");
  if (fp == NULL){
    std::cerr << "cannot open top file: " << tfile << std::endl;
    exit (1);
  }
  fscanf (fp, "%d", &nMeth);

  std::vector<int >  resdindex;
  std::vector<std::string >   resdname;
  std::vector<std::string >   atomname;
  std::vector<int >  atomindex;
  std::vector<std::vector<double > >  posi;
  std::vector<std::vector<double > >  velo;
  std::vector<double >  boxsize;
  
  GroFileManager::read (ifilename, resdindex, resdname, atomname, atomindex,
			posi, velo, boxsize);

  std::vector<int >  resdindex1;
  std::vector<std::string >   resdname1;
  std::vector<std::string >   atomname1;
  std::vector<int >  atomindex1;
  std::vector<std::vector<double > >  posi1;
  std::vector<std::vector<double > >  velo1;

  unsigned nmol = (posi.size() - nMeth) / 3;
  resdindex1.resize(nMeth+nmol*4);
  atomindex1.resize(nMeth+nmol*4);
  resdname1.resize(nMeth+nmol*4);
  atomname1.resize(nMeth+nmol*4);
  posi1.resize(nMeth+nmol*4);
  velo1.resize(nMeth+nmol*4);
  
  unsigned countAtom = nMeth;
  std::vector<double > mass (3, 1.00800);
  mass[0] = 15.99940;
  double totalmass = mass[0] + mass[1] + mass[2];

  std::cout << "nMeth is " << nMeth << std::endl;
  std::cout << "nmol is " << nmol << std::endl;
  std::cout << "size is " << atomname.size() << std::endl;
  
  for (unsigned i = 0; i < nmol; ++i){
    resdindex1[nMeth+4*i+0] = resdindex[nMeth+3*i+0];
    resdindex1[nMeth+4*i+1] = resdindex[nMeth+3*i+0];
    resdindex1[nMeth+4*i+2] = resdindex[nMeth+3*i+0];
    resdindex1[nMeth+4*i+3] = resdindex[nMeth+3*i+0];
    atomindex1[nMeth+4*i+0] = ++ countAtom;
    atomindex1[nMeth+4*i+1] = ++ countAtom;
    atomindex1[nMeth+4*i+2] = ++ countAtom;
    atomindex1[nMeth+4*i+3] = ++ countAtom;
    resdname1[nMeth+4*i+0] = resdname[nMeth+3*i+0];
    resdname1[nMeth+4*i+1] = resdname[nMeth+3*i+0];
    resdname1[nMeth+4*i+2] = resdname[nMeth+3*i+0];
    resdname1[nMeth+4*i+3] = resdname[nMeth+3*i+0];
    atomname1[nMeth+4*i+0] = atomname[nMeth+3*i+0];
    atomname1[nMeth+4*i+1] = atomname[nMeth+3*i+1];
    atomname1[nMeth+4*i+2] = atomname[nMeth+3*i+2];
    atomname1[nMeth+4*i+3] = "COM";

    std::vector<double > composi(3, 0.);
    std::vector<double > comvelo(3, 0.);
    
    for (unsigned jj = 0; jj < 3; ++jj){
      for (unsigned dd = 0; dd < 3; ++dd){
	composi[dd] += mass[jj] * posi[nMeth+3*i+jj][dd];
	comvelo[dd] += mass[jj] * velo[nMeth+3*i+jj][dd];
      }
    }
    for (unsigned dd = 0; dd < 3; ++dd){
      composi[dd] /= totalmass;
      comvelo[dd] /= totalmass;
    }
    posi1[nMeth+4*i+0] = posi[nMeth+3*i+0];
    posi1[nMeth+4*i+1] = posi[nMeth+3*i+1];
    posi1[nMeth+4*i+2] = posi[nMeth+3*i+2];
    posi1[nMeth+4*i+3] = composi;
    velo1[nMeth+4*i+0] = velo[nMeth+3*i+0];
    velo1[nMeth+4*i+1] = velo[nMeth+3*i+1];
    velo1[nMeth+4*i+2] = velo[nMeth+3*i+2];
    velo1[nMeth+4*i+3] = comvelo;
  }  

  for (int ii = 0; ii < nMeth; ++ii){
    resdindex1[ii] = resdindex[ii];
    atomindex1[ii] = atomindex[ii];
    resdname1[ii] = resdname[ii];
    atomname1[ii] = atomname[ii];
    posi1[ii] = posi[ii];
    velo1[ii] = velo[ii];
  }
  
  GroFileManager::write (ofilename,
			 resdindex1, resdname1,
			 atomname1, atomindex1,
			 posi1, velo1, boxsize);
  // FILE * fp = fopen ("topol.top", "w");
  // fprintf (fp, "#include \"system.itp\"\n[ molecules ]\nSOL %d\nTWALLA %d\nTWALLB %d\n",
  // 	   natom/3, naddHalf, naddHalf);
  // fclose (fp);
  
  return 0;
}

  
