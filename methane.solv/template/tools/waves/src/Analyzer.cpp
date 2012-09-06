#include "Analyzer.h"
#include <string.h>
#include <fstream>
#include <iostream>
#include <algorithm>

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

void TrajLoader_xtc::
clear ()
{
  if (inited){
    free (xx);
    xdrfile_close (xd);
    inited = false;
  }
}


TrajLoader_xtc::
TrajLoader_xtc ()
    : inited (false)
{
}

TrajLoader_xtc::
~TrajLoader_xtc ()
{
  clear ();
}

TrajLoader_xtc::
TrajLoader_xtc (const char * filename,
		const TopInfo info)
    : inited (false)
{
  reinit (filename, info);
}  

void TrajLoader_xtc::
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

bool TrajLoader_xtc::
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

void TrajLoader_xtc::
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


void TrajLoader_trr::
clear ()
{
  if (inited){
    free (xx);
    free (vv);
    free (ff);
    xdrfile_close (xd);
    inited = false;
  }
}


TrajLoader_trr::
TrajLoader_trr ()
    : inited (false)
{
}

TrajLoader_trr::
~TrajLoader_trr ()
{
  clear ();
}

TrajLoader_trr::
TrajLoader_trr (const char * filename,
		const TopInfo info)
    : inited (false)
{
  reinit (filename, info);
}  

void TrajLoader_trr::
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
  read_trr_natoms (tmpname, &natoms);
  step = 0;
  time = 0.;
  box = VectorType (0.,0.,0.);
  xx = (rvec *) malloc (sizeof(rvec) * natoms);
  vv = (rvec *) malloc (sizeof(rvec) * natoms);
  ff = (rvec *) malloc (sizeof(rvec) * natoms);
  lambda = 1;
  
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

bool TrajLoader_trr::
load ()
{
  if (inited){
    matrix tmpBox;
    int st = read_trr (xd, natoms, &step, &time, &lambda, tmpBox, xx, vv, ff);
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

void TrajLoader_trr::
formCoords (std::vector<std::vector<ValueType > > & ch4xx,
	    std::vector<std::vector<ValueType > > & ch4vv,
	    std::vector<std::vector<ValueType > > & ch4ff,
	    std::vector<std::vector<ValueType > > & h2oxx,
	    std::vector<std::vector<ValueType > > & h2ovv,
	    std::vector<std::vector<ValueType > > & h2off)
{
  
  ch4xx.resize(numMolCh4);
  ch4vv.resize(numMolCh4);
  ch4ff.resize(numMolCh4);
  h2oxx.resize(numMolH2o * 3);
  h2ovv.resize(numMolH2o * 3);
  h2off.resize(numMolH2o * 3);

  for (unsigned ii = 0; ii < numMolCh4; ++ii){
    ch4xx[ii].resize(3);
    ch4vv[ii].resize(3);
    ch4ff[ii].resize(3);
    for (unsigned dd = 0; dd < 3; ++dd){
      ch4xx[ii][dd] = xx[ii * tinfo.numAtomOnCh4 + tinfo.comIndexCh4][dd];
      ch4vv[ii][dd] = vv[ii * tinfo.numAtomOnCh4 + tinfo.comIndexCh4][dd];
      ch4ff[ii][dd] = ff[ii * tinfo.numAtomOnCh4 + tinfo.comIndexCh4][dd];
    }
  }
  for (unsigned ii = 0; ii < numMolH2o; ++ii){
    h2oxx[ii*3+0].resize(3);
    h2oxx[ii*3+1].resize(3);
    h2oxx[ii*3+2].resize(3);
    h2ovv[ii*3+0].resize(3);
    h2ovv[ii*3+1].resize(3);
    h2ovv[ii*3+2].resize(3);
    h2off[ii*3+0].resize(3);
    h2off[ii*3+1].resize(3);
    h2off[ii*3+2].resize(3);
    if (int(numAtomCh4 + ii * tinfo.numAtomOnH2o + tinfo.H2IndexH2o) > int(natoms)){
      std::cerr << "wrong index of water! exit" << std::endl;
      exit(1);
    }
    for (unsigned dd = 0; dd < 3; ++dd){
      h2oxx[ii*3+0][dd] = xx[numAtomCh4 + ii * tinfo.numAtomOnH2o + tinfo.OIndexH2o][dd];
      h2oxx[ii*3+1][dd] = xx[numAtomCh4 + ii * tinfo.numAtomOnH2o + tinfo.H1IndexH2o][dd];
      h2oxx[ii*3+2][dd] = xx[numAtomCh4 + ii * tinfo.numAtomOnH2o + tinfo.H2IndexH2o][dd];
      h2ovv[ii*3+0][dd] = vv[numAtomCh4 + ii * tinfo.numAtomOnH2o + tinfo.OIndexH2o][dd];
      h2ovv[ii*3+1][dd] = vv[numAtomCh4 + ii * tinfo.numAtomOnH2o + tinfo.H1IndexH2o][dd];
      h2ovv[ii*3+2][dd] = vv[numAtomCh4 + ii * tinfo.numAtomOnH2o + tinfo.H2IndexH2o][dd];
      h2off[ii*3+0][dd] = ff[numAtomCh4 + ii * tinfo.numAtomOnH2o + tinfo.OIndexH2o][dd];
      h2off[ii*3+1][dd] = ff[numAtomCh4 + ii * tinfo.numAtomOnH2o + tinfo.H1IndexH2o][dd];
      h2off[ii*3+2][dd] = ff[numAtomCh4 + ii * tinfo.numAtomOnH2o + tinfo.H2IndexH2o][dd];
    }
  }
}



