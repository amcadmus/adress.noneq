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
      pout[ii] += tmatrix[ii][jj] * pin[jj];
    }
  }
}

void reverse_tmatrix (const vector<vector<double > > & tmatrix,
		      const vector<double > & dist,
		      vector<vector<double > > & omatrix)
{
  omatrix = tmatrix;
  for (unsigned ii = 0; ii < tmatrix.size(); ++ii){
    for (unsigned jj = 0; jj < tmatrix[ii].size(); ++jj){
      if (dist[jj] != 0){
	omatrix[jj][ii] = dist[ii] * tmatrix[ii][jj] / dist[jj];
      }
      else {
	if (dist[ii] * tmatrix[ii][jj] != 0){
	  cerr << "problem of reverse" <<endl;
	}
	else{
	  omatrix[jj][ii] =0.;
	}
      }
    }
  }  
}

void convert_milestone (const vector<unsigned > & input,
			const vector<unsigned > & clusterMap,
			vector<unsigned > & output)
{
  output.resize(input.size());
  if (output.size() == 0) return;
  output[0] = clusterMap[input[0]];
  for (unsigned ii = 1; ii < input.size(); ++ii){
    unsigned current = clusterMap[input[ii]];
    if (current == output[ii-1] || current == 0) {
      output[ii] = output[ii-1];
    }
    else{
      output[ii] = current;
    }
  }
}

int compute_next_index (const vector<unsigned > & disc_traj,
			const vector<unsigned > & clusterMap,
			const unsigned & curtposi_)
{
  unsigned curtposi (curtposi_);
  if (curtposi >= disc_traj.size()) return -1;
  if (clusterMap[disc_traj[curtposi]] != 0){
    return clusterMap[disc_traj[curtposi]] - 1;
  }
  else {
    while (curtposi < disc_traj.size() && clusterMap[disc_traj[curtposi]] == 0){
      curtposi ++;
    }
    if (curtposi == disc_traj.size()){
      return -1;
    }
    else {
      return clusterMap[disc_traj[curtposi]] - 1;
    }
  }
}


int main(int argc, char * argv[])
{
  std::string ifile, idfile, ismfile, otmfile, ommfile;
  unsigned nbin, ndataBlock;
  
  po::options_description desc ("Calculates the MSM based on coreset milestoning.\nAllow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input,f",  po::value<std::string > (&ifile)->default_value ("traj.dih.disc.periodT"), "the file of file names")
      ("input-dir,d", po::value<std::string > (&idfile)->default_value ("dir.name"), "the output of metastable propulation")
      ("n-data-block,n", po::value<unsigned > (&ndataBlock)->default_value (1), "num data in each block.")
      ("num-bin,n", po::value<unsigned > (&nbin)->default_value (20), "number of blocks.")
      ("input-cluster-map", po::value<string > (&ismfile)->default_value ("cluster.map.out"), "the input of cluster map.")
      ("output-matrix-t", po::value<string > (&otmfile)->default_value ("coreset.t.out"), "the output matrix T of coreset MSM.")
      ("output-matrix-m", po::value<string > (&ommfile)->default_value ("coreset.m.out"), "the output matrix M of coreset MSM.");
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }
  
  vector<unsigned > clusterMap;
  unsigned numCluster = load_cluster_map (ismfile, nbin, clusterMap);

  vector<vector<BlockAverage_acc > > tmatrix (numCluster);
  vector<vector<BlockAverage_acc > > mmatrix (numCluster);
  for (unsigned ii = 0; ii < numCluster; ++ii){
    tmatrix[ii].resize(numCluster);
    mmatrix[ii].resize(numCluster);
    for (unsigned jj = 0; jj < numCluster; ++jj){
      tmatrix[ii][jj].reinit(ndataBlock);
      mmatrix[ii][jj].reinit(ndataBlock);
    }
  }  
  
  ifstream fpname (idfile.c_str());
  if (!fpname){
    std::cerr << "cannot open file " << idfile << std::endl;
    return 1;
  }
  char nameline [MaxLineLength];
  unsigned countFile = 0;

  while (fpname.getline(nameline, MaxLineLength)){
    if (nameline[0] == '#') continue;
    string filename = string(nameline) + string("/") + ifile;
    FILE *fp = fopen (filename.c_str(), "r");
    if (fp == NULL){
      std::cerr << "cannot open file " << filename << std::endl;
      return 1;
    }
    if (countFile % 100 == 0){
      cout << "# processing file " << filename << endl;
    }
    countFile ++;
    vector<unsigned > disc_traj;
    unsigned tmpread;
    while (1 == fscanf(fp, "%d", &tmpread)){
      disc_traj.push_back (tmpread);
    }
    fclose (fp);
    vector<unsigned > milestone_traj;
    // for (unsigned ii = 0; ii < disc_traj.size(); ++ii){
    //   cout << disc_traj[ii] << " ";     
    // }
    // cout << endl;    
    convert_milestone (disc_traj, clusterMap, milestone_traj);
    // for (unsigned ii = 0; ii < milestone_traj.size(); ++ii){
    //   cout << milestone_traj[ii] << " ";      
    // }
    // cout << endl;
    // cout << endl;
    unsigned curtposi = 0;
    while (curtposi < disc_traj.size()){
      unsigned curtindex = milestone_traj[curtposi] - 1;
      int nextindex = compute_next_index (disc_traj, clusterMap, curtposi);
      int next2index = compute_next_index (disc_traj, clusterMap, curtposi + 1);
      if (nextindex < 0 ) break;
      for (int kk = 0; kk < numCluster; ++kk){
	if ((kk) == nextindex){
	  mmatrix[curtindex][kk].deposite (1.);
	}
	else{
	  mmatrix[curtindex][kk].deposite (0.);
	}
      }
      if (next2index >= 0){
	for (int kk = 0; kk < numCluster; ++kk){
	  if (kk == next2index){
	    mmatrix[curtindex][kk].deposite (1.);
	  }
	  else{
	    mmatrix[curtindex][kk].deposite (0.);
	  }
	}	
      }
      curtposi ++;
    }
  }
  
  for (unsigned ii = 0; ii < numCluster; ++ii){
    for (unsigned jj = 0; jj < numCluster; ++jj){
      mmatrix[ii][jj].calculate();
      tmatrix[ii][jj].calculate();
      // cout << tmatrix[ii][jj].getAvg() << endl;
      // cout << mmatrix[ii][jj].getAvg() << endl;     
    }
  }

  FILE * fp = fopen (otmfile.c_str(), "w");
  if (fp == NULL){
    std::cerr << "cannot open file " << otmfile << std::endl;
    return 1;
  }
  for (unsigned ii = 0; ii < numCluster; ++ii){
    for (unsigned jj = 0; jj < numCluster; ++jj){
      fprintf (fp, "%f ", tmatrix[ii][jj].getAvg());
      printf ("%f ", tmatrix[ii][jj].getAvgError());
    }
    fprintf (fp, "\n");
    printf("\n");
  }
  printf("\n");
  fclose (fp);
  FILE * fp1 = fopen (ommfile.c_str(), "w");
  if (fp1 == NULL){
    std::cerr << "cannot open file " << ommfile << std::endl;
    return 1;
  }
  for (unsigned ii = 0; ii < numCluster; ++ii){
    for (unsigned jj = 0; jj < numCluster; ++jj){
      fprintf (fp1, "%f ", mmatrix[ii][jj].getAvg());
      printf ("%f ", mmatrix[ii][jj].getAvgError());
    }
    fprintf (fp1, "\n");
    printf("\n");
  }
  fclose (fp1);
  
  
  return 0;
}
