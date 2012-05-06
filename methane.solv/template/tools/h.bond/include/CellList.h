#ifndef __CellList_h_wanghan__
#define __CellList_h_wanghan__

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <cmath>

#include "Defines.h"

class CellList
{
  VectorType box;
  VectorType cellSize;
  IntVectorType nCell;
  // std::vector<unsigned > numInCell;
  std::vector<std::vector<unsigned > > list;
public:
  inline unsigned index3to1 (unsigned  ix, unsigned  iy, unsigned  iz) const;
  inline void     index1to3 (unsigned& input,
			     unsigned& ix, unsigned& iy, unsigned& iz) const;
public:
  CellList (const unsigned & numAtom,
	    const VectorType & box,
	    const ValueType & cellSize);
  void reinit (const unsigned & numAtom,
	       const VectorType & box,
	       const ValueType & cellSize);
public:
  VectorType getCellSize () const {return cellSize;}
  unsigned getTotalNumCell () const {return nCell.x * nCell.y * nCell.z;}
  IntVectorType getNumCell () const {return nCell;}
  const std::vector<std::vector<unsigned > > & getList () const {return list;}
public:
  bool isEmpty () const;
  void clear ();
  void rebuild (const std::vector<std::vector<ValueType > > & coord);
  unsigned calCellIndex (const std::vector<ValueType > & coord) const;
}
    ;


unsigned CellList::
index3to1 (unsigned  ix, unsigned  iy, unsigned  iz) const
{
  return iz + nCell.z * (iy + nCell.y * ix);
}

void CellList::
index1to3 (unsigned& input,
	   unsigned& ix, unsigned& iy, unsigned& iz) const
{
  unsigned tmp = input;
  iz = tmp % (nCell.z);
  tmp = (tmp - iz) / nCell.z;
  iy = tmp % (nCell.y);
  ix =  (tmp - iy) / nCell.y;
}


#endif
