#include "Floque.h"

Floque::
Floque (const int & nsize_,
	const double & period_)
    :period (period_), nsize(nsize_)
{
  nmode = nsize * 2 + 1;
  nshift = - nsize;
}

void Floque::
naiveFourier (const vector<double > & in,
	      const int & nn,
	      complex<double > & out)
{
  double omega = 2. * M_PI / period;
  double dt = period / in.size();
  out = complex<double>(0.0, 0.0);
  for (int jj = 0; jj < int(in.size())-1; ++jj){
    double tt = jj * dt;
    out.real() += 0.5 * dt * (in[jj] * cos(nn * omega * tt)  + in[jj+1] * cos(nn * omega * (tt+dt)));
    out.imag() += 0.5 * dt * (in[jj] * sin(-nn * omega * tt)  + in[jj+1] * sin(-nn * omega * (tt+dt)));      
  }
  out.real() += 0.5 * dt * (in.back() * cos(nn * omega * (period - dt))  + in.front() * cos(nn * omega * (period)));
  out.imag() += 0.5 * dt * (in.back() * sin(-nn * omega * (period - dt))  + in.front() * sin(-nn * omega * (period)));
  out = out / period;
}

void Floque::
fourierTransitionMatrix (const vector<vector<vector<double > > > & in)
{
  ffnsize = nsize * 2;
  ffnmode = ffnsize * 2 + 1;
  ffnshift = - ffnsize;
  nstate = in.front().size();
  cout << "# number DOF for Fourier transform is " << ffnmode << endl;
  // if (ffnmode > 1+int(in.size())){
  //   cerr << "Fatal: asked number of mode is larger than the input number of degrees of freedoms, exit" << endl;
  //   exit (1);
  // }

  // ffnsize * nstate * nstate
  vector<vector<vector<complex<double > > > > & ffin (ffTransitionMatrix);
  ffin.resize(ffnmode);
  for (int ii = 0; ii < ffnmode; ++ii){
    ffin[ii].resize(nstate);
    for (int jj = 0; jj < nstate; ++jj){
      ffin[ii][jj].resize (nstate, complex<double > (0., 0.));
    }
  }
  
  for (int ii = 0; ii < ffnmode; ++ii){
    int nn = ii + ffnshift;
    for (int jj = 0; jj < nstate; ++jj){
      for (int kk = 0; kk < nstate; ++kk){
	vector<double > inMatrixEle (in.size());
	for (int ll = 0; ll < int(inMatrixEle.size()); ++ll){
	  inMatrixEle[ll] = in[ll][jj][kk];
	}
	naiveFourier (inMatrixEle, nn, ffin[ii][jj][kk]);
      }
    }
  }
}

void Floque::
checkFourierTransitionMatrix (const vector<vector<vector<double > > > & in)
{
  // int ffnsize = nsize * 2;
  // int ffnmode = ffnsize * 2 + 1;
  // int ffnshift = - ffnsize;
  nstate = in.front().size();
  double dt = period / in.size();
  double omega = 2. * M_PI / period;

  // if (ffnmode > 1+int(in.size())){
  //   cerr << "Fatal: asked number of mode is larger than the input number of degrees of freedoms, exit" << endl;
  //   exit (1);
  // }
  if (ffTransitionMatrix.size() == 0) return;

  for (int jj = 0; jj < nstate; ++jj){
    for (int kk = 0; kk < nstate; ++kk){
      double maxAbsDiff = 0;
      double maxRelDiff = 0;
      for (int ll = 0; ll < int(in.size()); ++ll){
	double tt = ll * dt;
	complex <double > checked (0.0, 0.0);
	for (int ii = 0; ii < ffnmode; ++ii){
	  int nn = ii + ffnshift;
	  checked += ffTransitionMatrix[ii][jj][kk] *
	      complex<double > (cos(nn * omega * tt), sin(nn * omega * tt));
	}
	// cout << "( " << jj 
	//      << " " << kk
	//      << " )\t\t";
	// cout << "in: " << in[ll][jj][kk]
	//      << "\t check: " << checked
	//      << "\t diff: " << abs(checked - complex<double> (in[ll][jj][kk], 0.))
	//      << "\t rel_diff: " << abs(checked - complex<double> (in[ll][jj][kk], 0.)) / in[ll][jj][kk]
	//      << endl;
	double abs_diff = abs(checked - complex<double> (in[ll][jj][kk], 0.));
	double rel_diff = (in[ll][jj][kk] == 0) ? 0. : abs_diff / in[ll][jj][kk];
	// printf ("%d %d \t in: %.3e  check: %.3e %.3e \t  diff: %.3e  rel_diff %.3e\n",
	// 	jj, kk,
	// 	in[ll][jj][kk],
	// 	checked.real(), checked.imag(),
	// 	abs_diff, rel_diff);
	if (abs_diff > maxAbsDiff) maxAbsDiff = abs_diff;
	if (rel_diff > maxRelDiff) maxRelDiff = rel_diff;
      }
      // cout << endl;
      printf ("%d %d \t max_abs: %.3e  max_rel: %.3e\n", jj, kk, maxAbsDiff, maxRelDiff);
    }
    cout << endl;
  }
}


void Floque::
buildFloqueMatrix (const vector<vector<vector<double > > > & in)
{
  double omega = 2. * M_PI / period;
  nstate = in.front().size();
  floqueDOF = nmode * nstate;
  floqueMatrix.resize (floqueDOF);
  for (int ii = 0; ii < floqueDOF; ++ii){
    floqueMatrix[ii].resize (floqueDOF);
  }

  fourierTransitionMatrix (in);

  for (int ii = 0; ii < nmode; ++ii){
    int nn = ii + nshift;
    for (int jj = 0; jj < nmode; ++jj){
      int mm = jj + nshift;
      for (int alpha = 0; alpha < nstate; ++alpha){
	int firstIdx = ii * nstate + alpha;
	for (int beta = 0; beta < nstate; ++beta){
	  int secondIdx = jj * nstate + beta;
	  int ffinIndex = nn - mm - ffnshift;
	  floqueMatrix[firstIdx][secondIdx] = ffTransitionMatrix [ffinIndex][alpha][beta];
	  if (nn == mm && alpha ==  beta){
	    floqueMatrix[firstIdx][secondIdx].imag() -= (nn * omega);
	  }
	}
      }
    }
  }
}

void Floque::
printFloqueMatrix (const string & filename_)
{
  {
    string filename (filename_);  
    filename += ".r";
    FILE * fp = fopen (filename.c_str(), "w");
    if (fp == NULL){
      cerr << "cannot open file " << filename << endl;
      return ;
    }
    for (int ii = 0; ii < floqueDOF; ++ii){
      for (int jj = 0; jj < floqueDOF; ++jj){
	fprintf (fp, "%e  ", floqueMatrix[ii][jj].real());
      }
      fprintf (fp, "\n");
    }
    fclose (fp);
  }
  {
    string filename (filename_);  
    filename += ".i";
    FILE * fp = fopen (filename.c_str(), "w");
    if (fp == NULL){
      cerr << "cannot open file " << filename << endl;
      return ;
    }
    for (int ii = 0; ii < floqueDOF; ++ii){
      for (int jj = 0; jj < floqueDOF; ++jj){
	fprintf (fp, "%e  ", floqueMatrix[ii][jj].imag());
      }
      fprintf (fp, "\n");
    }
    fclose (fp);
  }
}

void Floque::
printFfTransitionMatrix (const string & filename_)
{
  for (unsigned kk = 0; kk < ffTransitionMatrix.size(); ++kk)
  {
    string filename (filename_);
    char number [1024];
    sprintf (number, ".%06d.r", kk);
    filename += number;
    FILE * fp = fopen (filename.c_str(), "w");
    if (fp == NULL){
      cerr << "cannot open file " << filename << endl;
      return ;
    }
    for (int ii = 0; ii < nstate; ++ii){
      for (int jj = 0; jj < nstate; ++jj){
	fprintf (fp, "%e  ", ffTransitionMatrix[kk][ii][jj].real());
      }
      fprintf (fp, "\n");
    }
    fclose (fp);
  }
  for (unsigned kk = 0; kk < ffTransitionMatrix.size(); ++kk)
  {
    string filename (filename_);
    char number [1024];
    sprintf (number, ".%06d.i", kk);
    filename += number;
    FILE * fp = fopen (filename.c_str(), "w");
    if (fp == NULL){
      cerr << "cannot open file " << filename << endl;
      return ;
    }
    for (int ii = 0; ii < nstate; ++ii){
      for (int jj = 0; jj < nstate; ++jj){
	fprintf (fp, "%e  ", ffTransitionMatrix[kk][ii][jj].imag());
      }
      fprintf (fp, "\n");
    }
    fclose (fp);
  }

  // for (unsigned kk = 0; kk < ffTransitionMatrix.size(); ++kk){
  //   double sum = 0.;
  //   for (int ii = 0; ii < nstate; ++ii){
  //     for (int jj = 0; jj < nstate; ++jj){
  // 	sum += abs(ffTransitionMatrix[kk][ii][jj]) * abs(ffTransitionMatrix[kk][ii][jj]);
  //     }
  //   }
  //   printf ("%d %e\n", kk + ffnshift, sqrt(sum));
  // }
}


void Floque::
convert2FloqueVector (const vector<double > & in,
		      vector<complex<double > > & out)
{
  if (floqueMatrix.size() == 0){
    cerr << "Fatal: Floque matrix should be initialized before converting vectors" <<endl;
    exit(1);
  }
  if (nstate != int(in.size())){
    cerr << "Fatal: inconsistent input vector with the previously recored Floque matrix " <<endl;
    exit(1) ;
  }
  out.clear ();
  out.resize (floqueDOF, complex<double > (0.,0.));

  int nn = 0;
  int ii = nn - nshift;
  for (int alpha = 0; alpha < nstate; ++alpha){
    int firstIdx = ii * nstate + alpha;
    out[firstIdx] = in[alpha];
  }
}

void Floque::
convert2NormalVector (const vector<complex<double > > & in,
		      const double & tt,
		      vector<complex<double > > & out)
{
  double omega = 2. * M_PI / period;
  out.resize(nstate);
  
  for (int alpha = 0; alpha < nstate; ++alpha){
    out[alpha] = complex<double > (0., 0.);
    for (int ii = 0; ii < nmode; ++ii){
      int nn = ii + nshift;
      int floqueIdx = ii * nstate + alpha;
      out[alpha] += in[floqueIdx] *
	  complex<double > (cos (nn * omega * tt), sin(nn * omega * tt));
    }
  }
  
  // for (int jj = 0; jj < nstate; ++jj){
  //   complex <double > checked (0.0, 0.0);
  //   for (int  = 0; ii < nmode; ++ii){
  //     int nn = ii + ffnshift;
  //     checked += ffTransitionMatrix[ii][jj][kk] *
  // 	  complex<double > (cos(nn * omega * tt), sin(nn * omega * tt));
  //   }
  // }
}


void Floque::
applyFloqueMatrix (const vector<complex<double > > & in,
		   vector<complex<double > > & out)
{
  out.resize(floqueMatrix.size());
  for (int ii = 0; ii < int(floqueMatrix.size()); ++ii){
    out[ii] = complex<double > (0., 0.);
    for (int jj = 0; jj < int(floqueMatrix.size()); ++jj){
      out[ii] += floqueMatrix[ii][jj] * in[jj];
    }
  }
}






// void Floque::naiveFourier
// naiveFourier (const vector<double > & in,	      
// 	      const vector<complex<double > > & out)
// {
//   double omega = 2. * M_PI / period;
//   out.resize (nmode);
//   for (int ii = 0; ii < nmode; ++ii){
//     int nn = ii + nshift;
//     double dt = period / in.size();    
//     out[ii] = complex <double > (0., 0.);
//     for (int jj = 0; jj < in.size()-1; ++jj){
//       double tt = jj * dt;
//       out[ii].real() += 0.5 * dt * (in[jj] * cos(nn * omega * tt)  + in[jj+1] * cos(nn * omega * (tt+dt)));
//       out[ii].imag() += 0.5 * dt * (in[jj] * sin(-nn * omega * tt)  + in[jj+1] * sin(-nn * omega * (tt+dt)));      
//     }
//     out[ii].real() += 0.5 * dt * (in.back() * cos(nn * omega * (period - dt))  + in.front() * cos(nn * omega * (period)));
//     out[ii].imag() += 0.5 * dt * (in.back() * sin(-nn * omega * (period - dt))  + in.front() * sin(-nn * omega * (period)));
//     out[ii] = out[ii] / period;
//   }
// }

