#include "Analyzer.h"
#include <string.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "BlockAverage.h"

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

void Analyzer::
readData_exhaustive (const std::vector<Hbond> & bonds)
{
  map.clear();
  tree.clear();
  for (unsigned ii = 0; ii < bonds.size(); ++ii){
    map.push_bond (bonds[ii]);
  }
  HbondMap map_bk (map);
  unsigned nNodes = map.nNodes();
  // map.print();

  for (unsigned ii = 0; ii < nNodes; ++ii){
    while (!map.empty()){
      // std::cout << ii << std::endl;
      tree.clear ();
      tree.addRoot(map, ii);
      tree.addGenerations(map);
      // tree.print();
      Circles tmpc;
      tree.buildCircles (tmpc);
      // tmpc.uniqueCircles();
      // // std::cout << std::endl;
      // // std::cout << "before simplified:" << std::endl;
      // // tmpc.print();
      // tmpc.simplifyCircles ();
      // // if (numFail != 0){
      // //   printf ("num of fail: %d   seed: %d    nNodes: %d\n",
      // // 	      numFail, seed, nNodes);
      // //   seed += 1;
      // //   if (seed < nNodes){
      // // 	goto restart;
      // //   }
      // // }
      // // std::cout << std::endl;
      // // std::cout << "simplified circles:" << std::endl;
      // // tmpc.print();    
      cs.add (tmpc);
      HbondMap newMap;
      tree.renewMap (map, newMap);
      map = newMap;
    }
    cs.uniqueCircles();
    map = map_bk;
  }

  cs.uniqueCircles();
  cs.simplifyCircles();
  cs.sortCircles();
  // cs.print();
  cs.combineSimilar();
  // cs.print();
}

PolygonAverge::
PolygonAverge ()
{
  reinit (0);
}

void PolygonAverge::
reinit (const unsigned & maxPolygon, const unsigned block)
{
  numBlock = block;
  nframe = 0;
  sizeValue = maxPolygon + 1;
  numPolygon = 0.;
  value.resize (sizeValue);
  record.resize (sizeValue);
  for (unsigned ii = 0; ii < sizeValue; ++ii){
    value[ii]  = 0.;
    record[ii].clear();
  }
}

void PolygonAverge::
deposit (const Circles & cirs)
{
  numPolygon += cirs.circles.size();
  for (unsigned ii = 0; ii < sizeValue; ++ii){
    record[ii].push_back(0.);
  }
  for (unsigned ii = 0; ii < cirs.circles.size(); ++ii){
    if (cirs.circles[ii].size() < sizeValue){
      value[cirs.circles[ii].size()] += 1.;
      record[cirs.circles[ii].size()].back() += 1.;
    }
  }
  nframe ++;
}

void PolygonAverge::
average ()
{
  numPolygon /= ValueType(nframe);
  for (unsigned ii = 0; ii < sizeValue; ++ii){
    value[ii] /= ValueType(nframe);
  }

  if (nframe != record[0].size()){
    std::cout << "in consistent number of frames" << std::endl;
    exit(1);
  }
  avg.resize (sizeValue);
  err.resize(sizeValue);
  BlockAverage ba;
  for (unsigned ii = 0; ii < sizeValue; ++ii){  
    ba.processData (record[ii], numBlock);
    avg[ii] = ba.getAvg();
    err[ii] = ba.getAvgError();
  }
}

void PolygonAverge::
print (FILE * fp) const
{
  for (unsigned ii = 3; ii < sizeValue; ++ii){
    fprintf (fp, "%d %f %f\n", ii, avg[ii], err[ii]);
  }
}


static void
alignWaters (const VectorType & box,
	     std::vector<std::vector<ValueType > > & w0,
	     std::vector<std::vector<ValueType > > & w1)
{
  std::vector<ValueType> vbox(3);
  vbox[0] = box.x;
  vbox[1] = box.y;
  vbox[2] = box.z;

  for (unsigned ii = 1; ii < 3; ++ii){
    for (unsigned dd = 0; dd < 3; ++dd) {
      ValueType diff = w0[ii][dd] - w0[0][dd];
      if      (diff >  0.5 * vbox[dd]) diff -= vbox[dd];
      else if (diff < -0.5 * vbox[dd]) diff += vbox[dd];
      w0[ii][dd] = w0[0][dd] + diff;
    }
  }
  
  for (unsigned ii = 0; ii < 3; ++ii){
    for (unsigned dd = 0; dd < 3; ++dd) {
      ValueType diff = w1[ii][dd] - w0[0][dd];
      if      (diff >  0.5 * vbox[dd]) diff -= vbox[dd];
      else if (diff < -0.5 * vbox[dd]) diff += vbox[dd];
      w1[ii][dd] = w0[0][dd] + diff;
    }
  }	
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
  const std::vector<ValueType > & ch4_coord (ch4[0]);
  firstShellIdx.clear ();
  
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
      alignWaters (box, water0, water1);
      if ((*hbond)(water0, water1) || (*hbond)(water1, water0)){
	// the identity of water is the index of its oxygen
	bonds.push_back(Hbond(firstShellIdx[ii], firstShellIdx[jj]));
      }
    }
  }
  
  return bonds;
}

OneFrameHbonds::
OneFrameHbonds (const VectorType & box,
		const ValueType & cutoff,
		const HydrogenBond * hbond_)
{
  reinit (box, cutoff, hbond_);
}

void OneFrameHbonds::
reinit (const VectorType & box_,
	const ValueType & cutoff,
	const HydrogenBond * hbond_)
{
  box = box_;
  clist.reinit (box_, cutoff);
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

TopInfo::
TopInfo ()
    : numAtomOnCh4 (1),
      comIndexCh4 (0),
      numAtomOnH2o (3),
      OIndexH2o (0),
      H1IndexH2o (1),
      H2IndexH2o (2)
{
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
TrajLoader (const char * filename,
	    const TopInfo info)
    : inited (false)
{
  reinit (filename, info);
}  

void TrajLoader::
reinit (const char * filename,
	const TopInfo info)
{
  char tmpname[2048];
  strncpy (tmpname, filename, 2047);
  
  xd = xdrfile_open (filename, "r");
  if (xd == NULL){
    std::cerr << "cannot open file " << filename << std::endl;
    return ;
  }
  read_xtc_natoms (tmpname, &natoms);
  step = 0;
  time = 0.;
  box = VectorType (0.,0.,0.);
  xx = (rvec *) malloc (sizeof(rvec) * natoms);
  prec = 1000.;

  tinfo = info;
  numMolCh4 = 1;
  numAtomCh4 = tinfo.numAtomOnCh4 * numMolCh4;
  numAtomH2o = natoms - numAtomCh4;
  if (numAtomH2o % tinfo.numAtomOnH2o != 0){
    std::cerr << "inconsistent num atom and mol size, exit" << std::endl;
    exit(1);
  }
  numMolH2o = numAtomH2o / tinfo.numAtomOnH2o;
  
  inited = true;

  load ();

  xdrfile_close (xd);
  xd = xdrfile_open (filename, "r");  
  if (xd == NULL){
    std::cerr << "cannot open file " << filename << std::endl;
    return ;
  }
}

bool TrajLoader::
load ()
{
  if (inited){
    matrix tmpBox;
    int st = read_xtc (xd, natoms, &step, &time, tmpBox, xx, &prec);
    box.x = tmpBox[0][0];
    box.y = tmpBox[1][1];
    box.z = tmpBox[2][2];
    if (st == exdrOK) return true;
    else return false;
  }
  else {
    return false;
  }
}

void TrajLoader::
formCoords (std::vector<std::vector<ValueType > > & ch4,
	    std::vector<std::vector<ValueType > > & h2o)
{
  
  ch4.resize(numMolCh4);
  h2o.resize(numMolH2o * 3);

  for (unsigned ii = 0; ii < numMolCh4; ++ii){
    ch4[ii].resize(3);
    for (unsigned dd = 0; dd < 3; ++dd){
      ch4[ii][dd] = xx[ii * tinfo.numAtomOnCh4 + tinfo.comIndexCh4][dd];
    }
  }
  for (unsigned ii = 0; ii < numMolH2o; ++ii){
    h2o[ii*3+0].resize(3);
    h2o[ii*3+1].resize(3);
    h2o[ii*3+2].resize(3);
    if (int(numAtomCh4 + ii * tinfo.numAtomOnH2o + tinfo.H2IndexH2o) > int(natoms)){
      std::cerr << "wrong index of water! exit" << std::endl;
      exit(1);
    }
    for (unsigned dd = 0; dd < 3; ++dd){
      h2o[ii*3+0][dd] = xx[numAtomCh4 + ii * tinfo.numAtomOnH2o + tinfo.OIndexH2o][dd];
      h2o[ii*3+1][dd] = xx[numAtomCh4 + ii * tinfo.numAtomOnH2o + tinfo.H1IndexH2o][dd];
      h2o[ii*3+2][dd] = xx[numAtomCh4 + ii * tinfo.numAtomOnH2o + tinfo.H2IndexH2o][dd];
    }
  }
}



