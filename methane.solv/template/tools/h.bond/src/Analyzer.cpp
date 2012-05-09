#include "Analyzer.h"
#include <string.h>

using namespace CircleOperations;

void Analyzer::
readData (const std::vector<Hbond> & bonds)
{
  map.clear();
  tree.clear();
  for (unsigned ii = 0; ii < bonds.size(); ++ii){
    map.push_bond (bonds[ii]);
  }
  
  while (!map.empty()){
    tree.addRoot(map, 0);
    tree.addGenerations(map);
    // tree.print();
    Circles tmpc;
    tree.buildCircles (tmpc);
    tmpc.uniqueCircles();
    // std::cout << std::endl;
    // std::cout << "before simplified:" << std::endl;
    // tmpc.print();
    tmpc.simplifyCircles ();
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
  
  clist.rebuild (resd);
  std::vector<unsigned > cellsIdx;
  unsigned refIdx = clist.calCellIndex (ch4[0]);
  cellsIdx = clist.neighboringCellIndex (refIdx);
  const std::vector<std::vector<unsigned > > & list (clist.getList());
  std::vector<unsigned > firstShellIdx;
  const std::vector<ValueType > & ch4_coord (ch4[0]);
  
  for (unsigned ii = 0; ii < cellsIdx.size(); ++ii){
    for (unsigned jj = 0; jj < list[ii].size(); ++jj){
      unsigned tgtIdx = list[ii][jj];
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
OneFrameHbonds (const unsigned & numResd,
		const VectorType & box,
		const ValueType & cutoff,
		const HydrogenBond_Geo_1 hbond_)
{
  reinit (numResd, box, cutoff);
}

void OneFrameHbonds::
reinit (const unsigned & numResd,
	const VectorType & box_,
	const ValueType & cutoff,
	const HydrogenBond_Geo_1 hbond_)
{
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
TrajLoader (const char * filename)
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
  
  inited = true;
}

void TrajLoader::
load ()
{
  matrix tmpBox;
  read_xtc (xd, natoms, &step, &time, tmpBox, xx, &prec);
  box.x = tmpBox[0][0];
  box.y = tmpBox[1][1];
  box.z = tmpBox[2][2];
}

void TrajLoader::
formCoords (std::vector<std::vector<ValueType > > & ch4,
	    std::vector<std::vector<ValueType > > & h2o)
{
  unsigned numAtomCh4 = 1;
  unsigned numAtomH2o = (natoms - 1);
  
  ch4.resize(1);
  h2o.resize(numAtomH2o);

  for (unsigned ii = 0; ii < numAtomCh4; ++ii){
    for (unsigned dd = 0; dd < 3; ++dd){
      ch4[ii][dd] = xx[ii][dd];
    }
  }
  for (unsigned ii = 0; ii < numAtomH2o; ++ii){
    for (unsigned dd = 0; dd < 3; ++dd){
      h2o[ii][dd] = xx[ii+numAtomCh4][dd];
    }
  }
}



