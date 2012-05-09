#include "Analyzer.h"
#include <string.h>
#include <fstream>

using namespace CircleOperations;

void Analyzer::
readData (const std::vector<Hbond> & bonds)
{
  map.clear();
  tree.clear();
  for (unsigned ii = 0; ii < bonds.size(); ++ii){
    map.push_bond (bonds[ii]);
  }
  unsigned nNodes = map.nNodes();
  unsigned seed = 0;
  
  while (!map.empty()){
    restart:
    tree.clear ();
    tree.addRoot(map, seed);
    tree.addGenerations(map);
    // tree.print();
    Circles tmpc;
    tree.buildCircles (tmpc);
    tmpc.uniqueCircles();
    // std::cout << std::endl;
    // std::cout << "before simplified:" << std::endl;
    // tmpc.print();
    int numFail = tmpc.simplifyCircles ();
    if (numFail != 0){
      printf ("num of fail: %d   seed: %d    nNodes: %d\n",
	      numFail, seed, nNodes);
      seed += 1;
      if (seed < nNodes){
	goto restart;
      }
    }
    // std::cout << std::endl;
    // std::cout << "simplified circles:" << std::endl;
    // tmpc.print();    
    cs.add (tmpc);
    HbondMap newMap;
    tree.renewMap (map, newMap);
    map = newMap;
  }

  cs.uniqueCircles();
  cs.sortCircles();
}

std::vector<Hbond> OneFrameHbonds::
calBonds (const std::vector<std::vector<ValueType > > & ch4,
	  const std::vector<std::vector<ValueType > > & h2o)
{
  std::vector<std::vector<ValueType> > resd;
  resd.push_back (ch4[0]);
  unsigned nmol = h2o.size() / 3;
  for (unsigned ii = 0; ii < nmol; ++ii) {
    resd.push_back (h2o[ii*3]);
  }
  for (unsigned ii = 0; ii < resd.size(); ++ii){
    if      (resd[ii][0] >= box.x) resd[ii][0] -= box.x;
    else if (resd[ii][0] <  0    ) resd[ii][0] += box.x;
    if      (resd[ii][1] >= box.y) resd[ii][1] -= box.y;
    else if (resd[ii][1] <  0    ) resd[ii][1] += box.y;
    if      (resd[ii][2] >= box.z) resd[ii][2] -= box.z;
    else if (resd[ii][2] <  0    ) resd[ii][2] += box.z;
  }
  
  clist.rebuild (resd);
  std::vector<unsigned > cellsIdx;
  unsigned refIdx = clist.calCellIndex (ch4[0]);
  cellsIdx = clist.neighboringCellIndex (refIdx);
  const std::vector<std::vector<unsigned > > & list (clist.getList());
  std::vector<unsigned > firstShellIdx;
  const std::vector<ValueType > & ch4_coord (ch4[0]);
  
  for (unsigned ii = 0; ii < cellsIdx.size(); ++ii){
    unsigned targetCellIdx = cellsIdx[ii];
    for (unsigned jj = 0; jj < list[targetCellIdx].size(); ++jj){
      unsigned tgtIdx = list[targetCellIdx][jj];
      if (tgtIdx == 0) continue;
      else tgtIdx --;
      // tgtIdx is the mol index in h2o now
      std::vector<ValueType > o_coord (h2o[tgtIdx*3]);
      o_coord[0] -= ch4_coord[0];
      o_coord[1] -= ch4_coord[1];
      o_coord[2] -= ch4_coord[2];
      if      (o_coord[0] >  box.x * 0.5) o_coord[0] -= box.x;
      else if (o_coord[0] < -box.x * 0.5) o_coord[0] += box.x;
      if      (o_coord[1] >  box.y * 0.5) o_coord[1] -= box.y;
      else if (o_coord[1] < -box.y * 0.5) o_coord[1] += box.y;
      if      (o_coord[2] >  box.z * 0.5) o_coord[2] -= box.z;
      else if (o_coord[2] < -box.z * 0.5) o_coord[2] += box.z;
      ValueType diff = o_coord[0] * o_coord[0] + o_coord[1] * o_coord[1] + o_coord[2] * o_coord[2];
      // printf ("diff is %f\n", diff);
      if (diff < range2){
	firstShellIdx.push_back(tgtIdx*3);
      }
    }
  }
  numFistShell = firstShellIdx.size();

  std::vector<Hbond> bonds;
  
  for (unsigned ii = 0; ii < numFistShell; ++ii){
    std::vector<std::vector<ValueType> > water0;
    water0.push_back(h2o[firstShellIdx[ii]+0]);
    water0.push_back(h2o[firstShellIdx[ii]+1]);
    water0.push_back(h2o[firstShellIdx[ii]+2]);    
    for (unsigned jj = ii+1; jj < numFistShell; ++jj){
      std::vector<std::vector<ValueType> > water1;
      water1.push_back(h2o[firstShellIdx[jj]+0]);
      water1.push_back(h2o[firstShellIdx[jj]+1]);
      water1.push_back(h2o[firstShellIdx[jj]+2]);    
      if (hbond(water0, water1) || hbond(water1, water0)){
	// the identity of water is the index of its oxygen
	bonds.push_back(Hbond(firstShellIdx[ii], firstShellIdx[jj]));
      }
    }
  }
  
  return bonds;
}

OneFrameHbonds::
OneFrameHbonds (const unsigned & numAtomCh4,
		const unsigned & numAtomH2o,
		const VectorType & box,
		const ValueType & cutoff,
		const HydrogenBond_Geo_1 hbond_)
{
  reinit (numAtomCh4, numAtomH2o, box, cutoff);
}

void OneFrameHbonds::
reinit (const unsigned & numAtomCh4,
	const unsigned & numAtomH2o,
	const VectorType & box_,
	const ValueType & cutoff,
	const HydrogenBond_Geo_1 hbond_)
{
  unsigned numResd = numAtomCh4 + numAtomH2o / 3;
  box = box_;
  clist.reinit (numResd, box_, cutoff);
  hbond = hbond_;
  range2 = cutoff * cutoff;
  numFistShell = 0;
}

void TrajLoader::
clear ()
{
  if (inited){
    free (xx);
    xdrfile_close (xd);
    inited = false;
  }
}

TrajLoader::
TrajLoader ()
    : inited (false)
{
}

TrajLoader::
~TrajLoader ()
{
  clear ();
}

TrajLoader::
TrajLoader (const char * filename)
    : inited (false)
{
  reinit (filename);
}  

void TrajLoader::
reinit (const char * filename)
{
  char tmpname[2048];
  strncpy (tmpname, filename, 2047);
  
  xd = xdrfile_open (filename, "r");
  read_xtc_natoms (tmpname, &natoms);
  step = 0;
  time = 0.;
  box = VectorType (0.,0.,0.);
  xx = (rvec *) malloc (sizeof(rvec) * natoms);
  prec = 1000.;

  numAtomCh4 = 1;
  numAtomH2o = (natoms - 1);  

  load ();
  xdrfile_close (xd);
  xd = xdrfile_open (filename, "r");
  
  inited = true;
}

bool TrajLoader::
load ()
{
  matrix tmpBox;
  int st = read_xtc (xd, natoms, &step, &time, tmpBox, xx, &prec);
  box.x = tmpBox[0][0];
  box.y = tmpBox[1][1];
  box.z = tmpBox[2][2];
  if (st == exdrOK) return true;
  else return false;
}

void TrajLoader::
formCoords (std::vector<std::vector<ValueType > > & ch4,
	    std::vector<std::vector<ValueType > > & h2o)
{
  ch4.resize(numAtomCh4);
  h2o.resize(numAtomH2o);

  for (unsigned ii = 0; ii < numAtomCh4; ++ii){
    ch4[ii].resize(3);
    for (unsigned dd = 0; dd < 3; ++dd){
      ch4[ii][dd] = xx[ii][dd];
    }
  }
  for (unsigned ii = 0; ii < numAtomH2o; ++ii){
    h2o[ii].resize(3);
    for (unsigned dd = 0; dd < 3; ++dd){
      h2o[ii][dd] = xx[ii+numAtomCh4][dd];
    }
  }
}



