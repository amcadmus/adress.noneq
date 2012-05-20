#ifndef __Analyzer_h_wanghan__
#define __Analyzer_h_wanghan__

#include "Tree.h"
#include "HbondMap.h"
#include "Circles.h"
#include "HydrogenBond.h"
#include "CellList.h"
#include "xdrfile/xdrfile.h"
#include "xdrfile/xdrfile_xtc.h"

struct TopInfo
{
  unsigned numAtomOnCh4;
  unsigned comIndexCh4;
  unsigned numAtomOnH2o;
  unsigned OIndexH2o;
  unsigned H1IndexH2o;
  unsigned H2IndexH2o;
  TopInfo ();
}
    ;


class Analyzer 
{
  HbondMap map;  
  Tree tree;
  Circles cs;
public:
  void clear ();
  void readData (const std::vector<Hbond> & bonds);
  void readData_exhaustive (const std::vector<Hbond> & bonds);
  const Circles & getCircles () const;
  Circles & getCircles ();
}
    ;

class PolygonAverge
{
  unsigned nframe;
  unsigned numBlock;
  unsigned sizeValue;
  ValueType numPolygon;
  std::vector<double > value;
  std::vector<std::vector<double > > record;
  std::vector<double > avg;
  std::vector<double > err;  
public:
  PolygonAverge ();
  void reinit (const unsigned & maxPolygon, const unsigned block=20);
  void deposit (const Circles & cirs);
  void average ();
  void print (FILE * fp) const;
}
    ;


class OneFrameHbonds
{
  VectorType box;
  CellList clist;
  const HydrogenBond * hbond;
  ValueType range2;
  std::vector<unsigned > firstShellIdx;
  unsigned numFistShell;
public:
  OneFrameHbonds (const VectorType & box,
		  const ValueType & cutoff,
		  const HydrogenBond *hbond_);
  void reinit (const VectorType & box,
	       const ValueType & cutoff,
	       const HydrogenBond * hbond_);
public:
  unsigned getNumFirstShell () const {return numFistShell;}
  std::vector<Hbond > calBonds (const std::vector<std::vector<ValueType > > & ch4,
	  			const std::vector<std::vector<ValueType > > & h2o);
}
    ;

class TrajLoader 
{
  XDRFILE *xd;
  int natoms;
  unsigned numMolCh4, numMolH2o;
  unsigned numAtomCh4, numAtomH2o;
  int step;
  float time;
  VectorType box;
  rvec * xx;
  float prec;
  bool inited;
  void clear ();
  TopInfo tinfo;
public:
  TrajLoader ();
  ~TrajLoader ();
  TrajLoader (const char * filename,
	      const TopInfo info = TopInfo());
  void reinit (const char * filename,
	       const TopInfo info = TopInfo());
  bool load ();
public:
  // int getNumAtom () const {return natoms;}
  // int getNumAtomCh4 () const {return numAtomCh4;}
  // int getNumAtomH2o () const {return numAtomH2o;}
  VectorType getBox () const {return box;}
  float getTime () const {return time;}
public:
  void formCoords (std::vector<std::vector<ValueType > > & ch4,
		   std::vector<std::vector<ValueType > > & h2o);
}
    ;



inline void Analyzer::
clear ()
{
  cs.clear ();
}

inline const Circles &  Analyzer::
getCircles () const
{
  return cs;
}

inline Circles &  Analyzer::
getCircles ()
{
  return cs;
}


#endif
