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

#include "xdrfile/xdrfile.h"
#include "xdrfile/xdrfile_xtc.h"

#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include "BlockAverage.h"

int main(int argc, char * argv[])
{
  float begin, end, x0, x1, yz0, yz1;
  std::string ifile, ofile, method;
  
  po::options_description desc ("Allow options");
  desc.add_options()
    ("help,h", "print this message")
    ("begin,b", po::value<float > (&begin)->default_value(0.f), "start time")
    ("end,e",   po::value<float > (&end  )->default_value(0.f), "end   time")
    ("x0", po::value<float > (&x0)->default_value(0.f), "lower bound of the interval")
    ("x1", po::value<float > (&x1)->default_value(1.f), "upper bound of the interval")
    ("yz0", po::value<float > (&yz0)->default_value(0.f), "lower bound of the interval")
    ("yz1", po::value<float > (&yz1)->default_value(1.f), "upper bound of the interval")
    ("method,m",  po::value<std::string > (&method)->default_value ("adress"), "type of simulation to analyze")
    ("input,f",   po::value<std::string > (&ifile)->default_value ("traj.xtc"), "the input .xtc file")
    ("output,o",  po::value<std::string > (&ofile)->default_value ("number.out"), "the output file");
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  if (x0 > x1){
    float tmpx = x0;
    x0 = x1;
    x1 = tmpx;
  }
  if (yz0 > yz1){
    float tmpyz = yz0;
    yz0 = yz1;
    yz1 = tmpyz;
  }
  
  std::cout << "###################################################" << std::endl;
  std::cout << "# begin->end: " << begin << " " << end << std::endl;
  std::cout << "# [x0,  x1 ]: " << x0 << " " << x1 << std::endl;
  std::cout << "# [yz0, yz1]: " << yz0 << " " << yz1 << std::endl;
  std::cout << "# method: " << method << std::endl;
  std::cout << "# input: " << ifile << std::endl;
  std::cout << "###################################################" << std::endl;  
  
  XDRFILE *fp;
  int natoms, step;
  float time;
  matrix box;
  rvec * xx;
  float prec = 1000;
  float time_prec = .01;

  char tmpfname[1024];
  strncpy (tmpfname, ifile.c_str(), 1023);
  int c;
  if ((c = read_xtc_natoms (tmpfname, &natoms)) == 0) {
    // printf ("%d %d\n", c, natoms);
    xx = (rvec *) malloc (sizeof(rvec) * natoms);
  }
  else {
    // printf ("%d %d\n", c, natoms);
    fprintf (stderr, "error read_xtc_natoms");
    exit (1);
  }

  fp = xdrfile_open (ifile.c_str(), "r");
  if (fp == NULL){
    std::cerr << "cannot open file " << ifile << std::endl;
    exit (1);
  }
  FILE *fout = fopen (ofile.c_str(), "w");
  if (fout == NULL){
    std::cerr << "cannot open file " << ofile << std::endl;
    exit (1);
  }
  
  int countread = 0;
  std::vector<double > time_counts;
  while (read_xtc (fp, natoms, &step, &time, box, xx, &prec) == 0){
    if (end != 0.f) {
      if (time < begin - time_prec){
	continue;
      }
      else if (time > end + time_prec) {
	break;
      }	
    }
    else {
      if (time < begin - time_prec) continue;
    }
    if (countread++ % 100 == 0){
      printf ("# load frame at time: %.1f ps\r", time);
      fflush (stdout);
    }
    
    int count = 0;
    if (method == std::string ("adress")){
      int nmol = natoms / 4;
      for (int i = 0; i < nmol; ++i){
	if      (xx[i*4][0] <  0        ) xx[i*4][0] += box[0][0];
	else if (xx[i*4][0] >= box[0][0]) xx[i*4][0] -= box[0][0];
	if (xx[i*4][0] >= x0 && xx[i*4][0] < x1){
	  count ++;
	}
      }
    }
    else if (method == std::string ("atom")){
      int nmol = natoms / 3;
      for (int i = 0; i < nmol; ++i){
	std::vector<float > com(3, 0.);
	for (int dd = 0; dd < 3; ++dd){
	  float dx1, dx2;
	  dx1 = xx[i*3+1][dd] - xx[i*3+0][dd];
	  dx2 = xx[i*3+2][dd] - xx[i*3+0][dd];
	  if (dx1 > 0.5 * box[dd][dd]) {dx1 -= box[dd][dd]; printf ("hit\n");}
	  if (dx1 <-0.5 * box[dd][dd]) {dx1 += box[dd][dd]; printf ("hit\n");}
	  if (dx2 > 0.5 * box[dd][dd]) {dx2 -= box[dd][dd]; printf ("hit\n");}
	  if (dx2 <-0.5 * box[dd][dd]) {dx2 += box[dd][dd]; printf ("hit\n");}
	  com[dd] = 16. * xx[i*3+0][dd] +
	    1. * (xx[i*3+0][dd] + dx1) +
	    1. * (xx[i*3+0][dd] + dx2);
	  com[dd] /= 18.;
	}
	if (com[0] >= x0 && com[0] < x1 &&
	    com[1] >= yz0 && com[1] < yz1 &&
	    com[2] >= yz0 && com[2] < yz1
	    ){
	  count ++;
	}
      }
    }
    else if (method == std::string ("cg")){
      int nmol = natoms;
      for (int i = 0; i < nmol; ++i){
	if      (xx[i][0] <  0        ) xx[i][0] += box[0][0];
	else if (xx[i][0] >= box[0][0]) xx[i][0] -= box[0][0];
	if (xx[i][0] >= x0 && xx[i][0] < x1){
	  count ++;
	}
      }
    }

    fprintf (fout, "%f %d\n", time, count);

    time_counts.push_back (double(count));
  }
  printf ("\n");
  
  xdrfile_close (fp);
  free (xx);
  fclose (fout);

  int numBlocks (16);
  BlockAverage ba;
  ba.processData (time_counts, numBlocks);
  printf ("# avg  avg_error  var  var_error  var/avg\n");
  printf ("%e  %e  %e  %e   %e\n",
  	  ba.getAvg(), ba.getAvgError(),
  	  ba.getVar(), ba.getVarError(),
	  ba.getVar() / ba.getAvg());

  
  return 0;
}

