#ifndef __Analyzer_h_wanghan__
#define __Analyzer_h_wanghan__

#include "Tree.h"
#include "HbondMap.h"
#include "Circles.h"
#include "HydrogenBond.h"
#include "CellList.h"
#include "xdrfile/xdrfile.h"
#include "xdrfile/xdrfile_xtc.h"

class Analyzer 
{
  HbondMap map;  
  Tree tree;
  Circles cs;
public:
  void clear ();
  void readData (const std::vector<Hbond> & bonds);
  const Circles & getCircles () const;
}
    ;

class OneFrameHbonds
{
  VectorType box;
  CellList clist;
  HydrogenBond_Geo_1 hbond;
  ValueType range2;
  unsigned numFistShell;
public:
  OneFrameHbonds (const unsigned & numResd,
		  const VectorType & box,
		  const ValueType & cutoff,
		  const HydrogenBond_Geo_1 hbond_ = HydrogenBond_Geo_1 ());
  void reinit (const unsigned & numResd,
	       const VectorType & box,
	       const ValueType & cutoff,
	       const HydrogenBond_Geo_1 hbond_ = HydrogenBond_Geo_1 ());
  std::vector<Hbond > calBonds (const std::vector<std::vector<ValueType > > & ch4,
	  			const std::vector<std::vector<ValueType > > & h2o);
}
    ;

class TrajLoader 
{
  XDRFILE *xd;
  int natoms;
  int step;
  float time;
  VectorType box;
  rvec * xx;
  float prec;
  bool inited;
  void clear ();
public:
  TrajLoader ();
  TrajLoader (const char * filename);
  void reinit (const char * filename);
  void load ();
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

#endif
