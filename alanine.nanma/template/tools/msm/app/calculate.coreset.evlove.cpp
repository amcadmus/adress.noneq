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
#include "StringSplit.h"
#include "BlockAverage.h"
#include "MetaSet.h"

#define MaxLineLength 65536

namespace po = boost::program_options;
using namespace std;

unsigned load_cluster_map (const string & file,
			   const unsigned & nbin,
			   vector<unsigned > & clusterMap)
{
  clusterMap.resize(nbin*nbin);
  FILE * fp = fopen (file.c_str(), "r");
  if (fp == NULL){
    cerr << "cannot open file " << file <<  endl;
    exit(1);
  }
  unsigned max = 0;
  for (unsigned ii = 0; ii < nbin * nbin; ++ii){
    if (fscanf (fp, "%d", &clusterMap[ii]) != 1){
      cerr << "error reading " << file << endl;
      exit(1);
    }
    if (clusterMap[ii] > max) max = clusterMap[ii];
  }
  return max;
}

void load_steady_dist (const string & file,
		       vector<double > & dist)
{
  FILE * fp = fopen (file.c_str(), "r");
  if (fp == NULL){
    cerr << "cannot open file " << file <<  endl;
    exit(1);
  }
  double tmp;
  dist.clear ();
  while (1 == fscanf(fp, "%lf", &tmp)){
    dist.push_back (tmp);
  }
  fclose (fp);
}

void load_committor (const string & file,
		     const unsigned & ncluster,
		     vector<vector<double > > & fwq)
{
  FILE * fp = fopen (file.c_str(), "r");
  if (fp == NULL){
    cerr << "cannot open file " << file <<  endl;
    exit(1);
  }
  double tmp;
  unsigned countC = 0;
  fwq.clear();
  fwq.resize(ncluster);
  while (1 == fscanf(fp, "%lf", &tmp)){
    if (countC == ncluster){
      countC = 0;
    }
    fwq[countC].push_back (tmp);
    countC++;
  }
  fclose (fp);
}


void deposite_traj (const vector<unsigned > & traj,
		    const int & startPosi,
		    const int & stopPosi,
		    const vector<unsigned > & clusterMap,
		    vector<vector<BlockAverage_acc > > & fwCommitor,
		    vector<vector<BlockAverage_acc > > & bwCommitor)
{
  if (startPosi >= 0){
    for (unsigned mm = 0; mm < bwCommitor.size(); ++mm){
      if ((mm+1) == clusterMap[traj[startPosi]]){
	for (int ii = startPosi+1; ii < stopPosi; ++ii){
	  bwCommitor[mm][traj[ii]].deposite (1.);
	}
      }
      else {
	for (int ii = startPosi+1; ii < stopPosi; ++ii){
	  bwCommitor[mm][traj[ii]].deposite (0.);
	}
      }
    }
  }
  if (stopPosi < int(traj.size())){
    for (unsigned mm = 0; mm < fwCommitor.size(); ++mm){
      if ((mm+1) == clusterMap[traj[stopPosi]]){
	for (int ii = startPosi+1; ii < stopPosi; ++ii){
	  fwCommitor[mm][traj[ii]].deposite (1.);
	}
      }
      else {
	for (int ii = startPosi+1; ii < stopPosi; ++ii){
	  fwCommitor[mm][traj[ii]].deposite (0.);
	}
      }
    }
  }
}

void read_tmatrix (const string & filename,
		   vector<vector<double > > & tmatrix,
		   unsigned & nstate)
{
  ifstream fpname (filename.c_str());
  if (!fpname){
    std::cerr << "cannot open file " << filename << std::endl;
    exit(1);
  }
  char valueline [MaxLineLength];
  int countLine = 0;
  while (fpname.getline(valueline, MaxLineLength)){
    vector<string > words;
    StringOperation::split (string(valueline), words);
    if (countLine == 0){
      nstate = words.size();
      tmatrix.resize(nstate);
      for (unsigned ii = 0; ii < nstate; ++ii){
	tmatrix[ii].resize(nstate);
      }
    }
    if (words.size() != nstate){
      cerr << "format error of tmatrix reading " << endl;
      exit (1);
    }
    for (unsigned ii = 0; ii < nstate; ++ii){
      tmatrix[countLine][ii] = atof(words[ii].c_str());
    }
    countLine ++;
  }
}

void apply_trans (const vector<vector<double > > & tmatrix,
		  const vector<double > & pin,
		  vector<double > & pout)
{
  pout.resize(tmatrix.size());
  for (unsigned ii = 0; ii < tmatrix.size(); ++ii){
    pout[ii] = 0.;
    for (unsigned jj = 0; jj < tmatrix[ii].size(); ++jj){
      pout[ii] += tmatrix[jj][ii] * pin[jj];
    }
  }
}

int main(int argc, char * argv[])
{
  string iifile, itfile, ofile;
  double tend, dt;
  
  po::options_description desc ("Calculates the MSM based on coreset milestoning.\nAllow options");
  desc.add_options()
      ("help,h", "print this message")
      ("dt,d", po::value<double > (&dt)->default_value (10), "time interval.")
      ("end,e", po::value<double > (&tend)->default_value (4000), "end time.")
      ("input-init-prob", po::value<std::string > (&iifile)->default_value ("coreset.prob.out"), "the input file of coreset prob, take only the init.")
      ("input-tmatrix", po::value<string > (&itfile)->default_value ("coreset.tmatrix.out"), "the input transition matrix of MSM.")
      ("output", po::value<string > (&ofile)->default_value ("coreset.prob.msm.out"), "the output prob from MSM.");
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  vector<vector<double > > tmatrix;
  unsigned ncluster;
  read_tmatrix (itfile, tmatrix, ncluster);

  vector<double > initp (ncluster);
  FILE * fp = fopen (iifile.c_str(), "r");
  if (fp == NULL){
    cerr << "cannot open file " << iifile << endl;
    return 1;
  }
  double time;
  if (1 != fscanf (fp, "%lf", &time)){
    cerr << "format error of " << iifile << endl;
    return 1;
  }
  for (unsigned ii = 0; ii < ncluster; ++ii){
    double tmp;
    if (1 != fscanf (fp, "%lf", &tmp)){
      cerr << "format error of " << iifile << endl;
      return 1;
    }
    initp[ii] = tmp;
  }
  fclose (fp);

  unsigned nsteps = unsigned((tend + 0.5 * dt) / dt);
  vector<double > newp = initp;
  fp = fopen (ofile.c_str(), "w");
  if (fp == NULL){
    cerr << "cannot open file " << ofile << endl;
    return 1;
  }  
  for (unsigned tt = 0; tt <= nsteps; tt++){
    fprintf (fp, "%f ", dt * tt);
    for (unsigned ii = 0; ii < ncluster; ++ii){
      fprintf (fp, "%e ", initp[ii]);
    }
    fprintf (fp, "\n");
    apply_trans (tmatrix, initp, newp);
    initp = newp;
  }
  fclose (fp);
  
  return 0;
}
