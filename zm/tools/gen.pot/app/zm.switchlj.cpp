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
#include "Functions.h"
#include "Interpolation.h"

#define MaxLineLength 2048

namespace po = boost::program_options;
using namespace std;
using namespace ZeroMultipole;

class SwitchFunction
{
  Poly myp;
  Poly mypp;
  double start, end, inter;
public:
  SwitchFunction (const double & start_,
		  const double & end_);
  double u  (const double & rr) const;
  double up (const double & rr) const;
}
    ;

class C6
{
public:
  double u  (const double & rr) const;
  double up (const double & rr) const;
}
    ;

class C6_Smooth
{
  C6 c6;
  SwitchFunction sw;
public:
  C6_Smooth (const double & smooth_start,
	     const double & smooth_end);
  double u  (const double & rr) const;
  double up (const double & rr) const;
}
    ;

class C12
{
public:
  double u  (const double & rr) const;
  double up (const double & rr) const;
}
    ;

class C12_Smooth
{
  C12 c12;
  SwitchFunction sw;
public:
  C12_Smooth (const double & smooth_start,
	      const double & smooth_end);
  double u  (const double & rr) const;
  double up (const double & rr) const;
}
    ;

SwitchFunction::
SwitchFunction (const double & start_,
		const double & end_)
    : start(start_), end(end_)
{
  inter = end - start;
  Interpolation::piece6OrderInterpol (start, end, 0, 1, 0, 0, 0, 0, myp);
  mypp = myp;
  mypp.derivative ();
}

double SwitchFunction::
u  (const double & rr) const 
{
  if (rr < start) {
    return 0.;
  }
  else if (rr > end){
    return 1.;
  }
  else {
    return myp.value (rr);
  }
}

double SwitchFunction::
up (const double & rr) const 
{
  if (rr < start) {
    return 0.;
  }
  else if (rr > end){
    return 0.;
  }
  else {
    return mypp.value (rr);
  }
}

double C6::
u  (const double & rr) const 
{
  return 1./(rr * rr * rr * rr * rr * rr);
}

double C6::
up (const double & rr) const 
{
  return -6./(rr * rr * rr * rr * rr * rr * rr);
}

double C12::
u  (const double & rr) const 
{
  double rr6 = rr * rr * rr * rr * rr * rr;
  return 1./(rr6 * rr6);
}

double C12::
up (const double & rr) const 
{
  double rr6 = rr * rr * rr * rr * rr * rr;
  return -12./(rr6 * rr6 * rr);
}

C6_Smooth::
C6_Smooth (const double & smooth_start,
	   const double & smooth_end)
    : sw(smooth_start, smooth_end)
{
}

double C6_Smooth::
u (const double & rr) const
{
  return (1. - sw.u(rr)) * c6.u(rr);
}

double C6_Smooth::
up (const double & rr) const
{
  return (1. - sw.u(rr)) * c6.up(rr) + ( - sw.up(rr)) * c6.u(rr);
}

C12_Smooth::
C12_Smooth (const double & smooth_start,
	   const double & smooth_end)
    : sw(smooth_start, smooth_end)
{
}

double C12_Smooth::
u (const double & rr) const
{
  return (1. - sw.u(rr)) * c12.u(rr);
}

double C12_Smooth::
up (const double & rr) const
{
  return (1. - sw.u(rr)) * c12.up(rr) + ( - sw.up(rr)) * c12.u(rr);
}


int main(int argc, char * argv[])
{
  std::string ofile;
  double hbin, xup, alpha, rc, rs;
  int ll;

  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("bin,b",  po::value<double > (&hbin)->default_value (0.002), "bin size")
      ("xup,x",  po::value<double > (&xup)->default_value (2.), "xrange")
      ("alpha,a",  po::value<double > (&alpha)->default_value (1.2), "alpha")
      ("rc,c",  po::value<double > (&rc)->default_value (1.5), "cut-off")
      ("rswitch",  po::value<double > (&rs)->default_value (0.05), "the switch region")
      ("order,l",  po::value<int > (&ll)->default_value (0), "order")
      ("output,o", po::value<std::string > (&ofile)->default_value ("table.xvg"), "the output of the table");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  Potential potzm (ll, alpha, rc);
  
  int nbin = (xup + 0.5 * hbin) / hbin;
  hbin = xup / double(nbin);
  vector<double > xx (nbin+1), vv(nbin+1), ff(nbin+1);

  for (int ii = 0; ii < nbin+1; ++ii){
    xx[ii] = hbin * ii;
    if (ii == 0){
      vv[ii] = ff[ii] = 0.;
    }
    else { 
      vv[ii] = potzm.pot (xx[ii]);
      ff[ii] = potzm.mpotp (xx[ii]);
    }
  }

  C6_Smooth pc6 (rc - rs, rc);
  C12_Smooth pc12 (rc - rs, rc);
  
  FILE * fp = fopen (ofile.c_str(), "w");
  if (fp == NULL){
    cerr << "cannot open output file " << ofile << endl;
    return 1;
  }
  fprintf (fp, "# generated by %s \n", argv[0]);
  fprintf (fp, "# l: %d alpha: %f rc: %f \n", ll, alpha, rc);
  double fmax = 1e30;
  for (int ii = 0; ii < nbin+1; ++ii){
    double vc6, fc6, vc12, fc12;
    if (ii == 0){
      vc6 = fc6 = vc12 = fc12 = 0.;
    }
    else {
      vc6 = -pc6.u(xx[ii]);
      fc6 = pc6.up(xx[ii]);
      vc12 = pc12.u(xx[ii]);
      fc12 = -pc12.up(xx[ii]);
    }
    if (fc6 > fmax) {
      fc6 = 0;
      vc6 = 0;
    }
    if (fc12 > fmax) {
      fc12 = 0;
      vc12 = 0;
    }
    fprintf (fp, "%f \t %.16e %.16e \t %.16e %.16e \t %.16e %.16e\n",
	     xx[ii],
	     vv[ii], ff[ii], vc6, fc6, vc12, fc12
	);
  }
  fclose (fp);

  for (int ii = 1; ii < nbin; ++ii){
    double tmp = -(vv[ii+1] - vv[ii-1]) / (2.*hbin);
    printf ("%f %e \t %e \t %e\n", xx[ii], vv[ii], ff[ii], fabs(tmp - ff[ii]));
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // // check of the implementation of the derivatives:
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // double alpha = 1.2;
  // double xx = 1.4;
  // double hh = 1e-6;
  // double tmpn;
  // tmpn = (funcV(alpha,xx+hh) - funcV(alpha,xx-hh)) / (2.*hh);
  // printf ("check D1:  \tprec %e  \tnumer %e  \tdiff %e\n", funcD1V(alpha,xx), tmpn, fabs(funcD1V(alpha,xx) - tmpn));
  // tmpn = (funcD1V(alpha,xx+hh) - funcD1V(alpha,xx-hh)) / (2.*hh);
  // printf ("check D2:  \tprec %e  \tnumer %e  \tdiff %e\n", funcD2V(alpha,xx), tmpn, fabs(funcD2V(alpha,xx) - tmpn));
  // tmpn = (funcD2V(alpha,xx+hh) - funcD2V(alpha,xx-hh)) / (2.*hh);
  // printf ("check D3:  \tprec %e  \tnumer %e  \tdiff %e\n", funcD3V(alpha,xx), tmpn, fabs(funcD3V(alpha,xx) - tmpn));
  // tmpn = (funcD3V(alpha,xx+hh) - funcD3V(alpha,xx-hh)) / (2.*hh);
  // printf ("check D4:  \tprec %e  \tnumer %e  \tdiff %e\n", funcD4V(alpha,xx), tmpn, fabs(funcD4V(alpha,xx) - tmpn));
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // // end check of the implementation of the derivatives:
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  

  return 0;
}
