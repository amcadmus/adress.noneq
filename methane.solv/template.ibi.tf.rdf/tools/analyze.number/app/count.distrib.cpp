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

int main(int argc, char * argv[])
{
  float begin, end, refh;
  std::string ifile, ofile, method;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("begin,b", po::value<float > (&begin)->default_value(0.f), "start time")
      ("end,e",   po::value<float > (&end  )->default_value(0.f), "end   time")
      ("refh", po::value<float > (&refh)->default_value(0.25f), "size of the bin")
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

  std::cout << "###################################################" << std::endl;
  std::cout << "# begin->end: " << begin << " " << end << std::endl;
  std::cout << "# refh: " << refh << std::endl;
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

  int nbin;
  float offset;
  float marginh;
  float ep = 1e-3;
  if (read_xtc (fp, natoms, &step, &time, box, xx, &prec) != 0){
    std::cerr << "wrong reading xtc" << std::endl;
    exit(1);
  }
  else {
    nbin = int(0.5 * box[0][0] / refh) * 2;
    if (fabs (box[0][0] - refh * nbin) <= ep){
      offset = 0.;
      marginh = 0.;
      fprintf (fout, "# box: %f %f %f, nbin: %d \n#",
	       box[0][0], box[1][1], box[2][2], nbin);
      for (int i = 0; i < nbin; ++i){
	fprintf (fout, " %f", refh);
      }
      fprintf (fout, "\n#");
      for (int i = 0; i < nbin; ++i){
	fprintf (fout, " %f", (0.5 + i) * refh);
      }
      fprintf (fout, "\n");
    }
    else {
      marginh = 0.5 * (box[0][0] - refh * nbin);
      offset = refh - marginh;
      nbin += 2;
      fprintf (fout, "# box: %f %f %f, nbin: %d \n#",
	       box[0][0], box[1][1], box[2][2], nbin);
      fprintf (fout, " %f", marginh);
      for (int i = 0; i < nbin-2; ++i){      
	fprintf (fout, " %f", refh);
      }
      fprintf (fout, " %f", marginh);
      fprintf (fout, "\n#");
      fprintf (fout, " %f", 0.5 * marginh);
      for (int i = 0; i < nbin-2; ++i){      
	fprintf (fout, " %f", marginh + (0.5 + i) * refh);
      }
      fprintf (fout, " %f", box[0][0] - 0.5 * marginh);
      fprintf (fout, "\n");
    }
  }
  
  int countread = 0;
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
    
    std::vector<int > count (nbin, 0);
    if (method == std::string ("adress")){
      int nmol = natoms / 4;
      for (int i = 0; i < nmol; ++i){
	if      (xx[i*4][0] <  0        ) xx[i*4][0] += box[0][0];
	else if (xx[i*4][0] >= box[0][0]) xx[i*4][0] -= box[0][0];
	int posi = (xx[i*4][0] + offset) / refh;
	if (posi >= nbin) {
	  std::cout << "out of range" << std::endl;
	  continue;
	}
	else {
	  count[posi] ++;
	}
      }
    }
    else if (method == std::string ("atom")){
      int nmol = natoms / 3;
      for (int i = 0; i < nmol; ++i){
	float comx;
	float dx1, dx2;
	dx1 = xx[i*3+1][0] - xx[i*3+0][0];
	dx2 = xx[i*3+2][0] - xx[i*3+0][0];
	if (dx1 > 0.5 * box[0][0]) {dx1 -= box[0][0]; printf ("hit\n");}
	if (dx1 <-0.5 * box[0][0]) {dx1 += box[0][0]; printf ("hit\n");}
	if (dx2 > 0.5 * box[0][0]) {dx2 -= box[0][0]; printf ("hit\n");}
	if (dx2 <-0.5 * box[0][0]) {dx2 += box[0][0]; printf ("hit\n");}
	comx = 16. * xx[i*3+0][0] +
	  1. * (xx[i*3+0][0] + dx1) +
	  1. * (xx[i*3+0][0] + dx2);
	comx /= 18.;
	if      (comx <  0        ) comx += box[0][0];
	else if (comx >= box[0][0]) comx -= box[0][0];
	int posi = (comx + offset) / refh;
	if (posi >= nbin) {
	  // std::cout << "out of range" << std::endl;
	  printf ("# out of range. comx: %f, nbin %d, refh %f, boxx %f\n",
		  comx, nbin, refh, box[0][0]);
	  continue;
	}
	else {
	  count[posi] ++;
	}
      }
    }
    else if (method == std::string ("cg")){
      int nmol = natoms;
      for (int i = 0; i < nmol; ++i){
	if      (xx[i][0] <  0        ) xx[i][0] += box[0][0];
	else if (xx[i][0] >= box[0][0]) xx[i][0] -= box[0][0];
	int posi = (xx[i][0] + offset) / refh;
	if (posi >= nbin) {
	  std::cout << "out of range" << std::endl;
	  continue;
	}
	else {
	  count[posi] ++;
	}
      }
    }

    fprintf (fout, "%f ", time);
    for (int i = 0; i < nbin; ++i){
      fprintf (fout, "%d ", count[i]);
    }
    fprintf (fout, "\n");
  }
  printf ("\n");
  
  xdrfile_close (fp);
  free (xx);
  fclose (fout);
  
  return 0;
}

