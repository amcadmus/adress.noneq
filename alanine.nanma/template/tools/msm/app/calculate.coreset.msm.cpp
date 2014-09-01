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


int main(int argc, char * argv[])
{
  std::string ifwfile, ibwfile, idfile, isfile, ismfile, ifloquetfile, isdfile, otmfile;
  unsigned nbin;
  
  po::options_description desc ("Calculates the forward and backward commitor.\nAllow options");
  desc.add_options()
      ("help,h", "print this message")
      ("num-bin,n", po::value<unsigned > (&nbin)->default_value (20), "number of blocks.")
      ("input-largest-set,s", po::value<std::string > (&isfile)->default_value ("largestSet"), "the input file of largest set.")
      ("input-floquet,f", po::value<string > (&ifloquetfile)->default_value ("floque.B.out"), "the input transition matrix.")
      ("input-cluster-map", po::value<string > (&ismfile)->default_value ("cluster.map.out"), "the input of cluster map.")
      ("input-steady", po::value<string > (&isdfile)->default_value ("steady.dist.out.orig"), "the input steady state.")
      ("input-fw", po::value<string > (&ifwfile)->default_value ("commitor.fw.out.orig"), "the input of forward commitor.")
      ("input-bw", po::value<string > (&ibwfile)->default_value ("commitor.bw.out.orig"), "the input of backward commitor.")
      ("output-tmatrix", po::value<string > (&otmfile)->default_value ("coreset.tmatrix.out"), "the output transition matrix of coreset MSM.");
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  vector<int > setMap ;
  {
    FILE * fp  = fopen (isfile.c_str(), "r");
    if (fp == NULL){
      cerr << "cannot open file " << isfile << endl;
      return 1;
    }
    int tmpint;
    while (fscanf(fp, "%d", &tmpint) == 1){
      setMap.push_back (tmpint);
    }
    fclose (fp);
  }
  unsigned nstate = setMap.size();
  map<unsigned,unsigned> mymap;
  for (unsigned ii = 0; ii < setMap.size(); ++ii){
    mymap[setMap[ii]] = ii;
  }

  vector<unsigned > clusterMap;
  unsigned numCluster = load_cluster_map (ismfile, nbin, clusterMap);
  vector<unsigned > clusterMapOrig (nstate, 0);
  for (unsigned ii = 0; ii < nbin; ++ii){
    for (unsigned jj = 0; jj < nbin; ++jj){
      unsigned dihIndex = ii * nbin + jj;
      map<unsigned, unsigned> :: const_iterator it = mymap.find(dihIndex);
      if (it != mymap.end()){
	unsigned fileIndex = it -> second;
	clusterMapOrig[fileIndex] = clusterMap[dihIndex];
      }
    }
  }

  vector<vector<double > > tmatrix;
  unsigned nstate1;
  read_tmatrix (ifloquetfile, tmatrix, nstate1);
  if (nstate1 != nstate){
    cerr << "the largest set file is not consistent with the transition matrix" << endl;
    return 1;
  }
  vector<vector<double > > fwq;
  vector<vector<double > > bwq;
  load_committor (ifwfile, numCluster, fwq);
  load_committor (ibwfile, numCluster, bwq);
  vector<double > dist;
  load_steady_dist (isdfile, dist);

  vector<vector<double > > omatrix;
  reverse_tmatrix (tmatrix, dist, omatrix);
  vector<vector<double > > pmatrix(omatrix);
  for (unsigned ii = 0; ii < nstate; ++ii){
    pmatrix[ii][ii] -= 1.;
  }
  vector<vector<double > > Pbwq(numCluster);
  for (unsigned ii = 0; ii < numCluster; ++ii){
    apply_trans (pmatrix, bwq[ii], Pbwq[ii]);
  }

  // for (unsigned ii = 0; ii < nstate; ++ii){
  //   printf ("%d\n", clusterMapOrig[ii]);
  // }
  

  // for (unsigned ii = 0; ii < numCluster; ++ii){
  //   double sum = 0.;
  //   for (unsigned jj = 0; jj < nstate; ++jj){
  //     sum += bwq[ii][jj];
  //     // if (dist[jj] != 0) bwq[ii][jj] /= dist[jj];
  //     // if (dist[jj] != 0) fwq[ii][jj] /= dist[jj];
  //   }
  //   cout <<  (sum) << endl;
  //   sum = 0.;
  //   for (unsigned jj = 0; jj < nstate; ++jj){
  //     sum += fwq[ii][jj];
  //   }
  //   cout <<  (sum) << endl;
  // }
  // for (unsigned jj = 0; jj < nstate; ++jj){
  //   double sum = 0;
  //   for (unsigned ii = 0; ii < numCluster; ++ii){
  //     sum +=  bwq[ii][jj];
  //   }
  //   cout << sum << endl;
  // }
  // for (unsigned jj = 0; jj < nstate; ++jj){
  //   double sum = 0;
  //   for (unsigned ii = 0; ii < numCluster; ++ii){
  //     sum +=  fwq[ii][jj];
  //   }
  //   cout << sum << endl;
  // }
  
  
  // for (unsigned jj = 0; jj < nstate; ++jj){
  //   double sum = 0;
  //   for (unsigned ii = 0; ii < numCluster; ++ii){
  //     sum +=  Pbwq[ii][jj];
  //   }
  //   cout << sum << endl;
  // }

  vector<double > coresetDist (numCluster, 0.);
  for (unsigned ii = 0; ii < numCluster; ++ii){
    for (unsigned jj = 0; jj < nstate; ++jj){
    // printf ("%e\n", coresetDist[ii]);
      coresetDist[ii] += bwq[ii][jj] * dist[jj];
    }
  }
  double sum = 0.;
  for (unsigned ii = 0; ii < numCluster; ++ii){
    printf ("%e\n", coresetDist[ii]);
    sum += coresetDist[ii];
  }
  if (fabs(sum - 1) > 1e-6){
    cerr << "sum of prob is not 1, " << fabs(sum - 1) <<  " return..." << endl;
    return 1;
  }

  vector<vector<double > > coresetTmatrix (numCluster);
  for (unsigned ii = 0; ii < numCluster; ++ii){
    coresetTmatrix[ii].resize (numCluster, 0.);
  }

  for (unsigned ii = 0; ii < numCluster; ++ii){
    for (unsigned jj = 0; jj < numCluster; ++jj){
      if (ii == jj) continue;
      for (unsigned kk = 0; kk < nstate; ++kk){
	coresetTmatrix[ii][jj] += Pbwq[ii][kk] * fwq[jj][kk] * dist[kk];
      }
      coresetTmatrix[ii][jj] /= coresetDist[ii];
    }
  }
  
  for (unsigned ii = 0; ii < numCluster; ++ii){
    double sum = 0;
    for (unsigned jj = 0; jj < numCluster; ++jj){
      if (ii == jj) continue;
      sum += coresetTmatrix[ii][jj];
    }
    coresetTmatrix[ii][ii] = 1 - sum;
  }

  FILE * fp = fopen (otmfile.c_str(), "w");
  if (fp == NULL){
    cerr << "cannot open file " << otmfile <<  endl;
    exit(1);
  }  
  for (unsigned ii = 0; ii < numCluster; ++ii){
    for (unsigned jj = 0; jj < numCluster; ++jj){
      fprintf (fp, "%e \t", coresetTmatrix[ii][jj]);
    }
    fprintf (fp, "\n");
  }
  fclose (fp);
  
  return 0;
}
