#include "CellList.h"
#include <iostream>

CellList::
CellList (const unsigned & numAtom,
	  const VectorType & box_,
	  const ValueType & cellSize_)
{
  reinit (numAtom, box_, cellSize_);
}

void CellList::
reinit (const unsigned & numAtom,
	const VectorType & box_,
	const ValueType & cellSize_)
{
  box = box_;
  cellSize.x = cellSize_;
  cellSize.y = cellSize_;
  cellSize.z = cellSize_;
  nCell.x = box.x / cellSize.x;
  nCell.y = box.y / cellSize.y;
  nCell.z = box.z / cellSize.z;
  cellSize.x = box.x / ValueType(nCell.x);
  cellSize.y = box.y / ValueType(nCell.y);
  cellSize.z = box.z / ValueType(nCell.z);

  list.clear ();
  if (nCell.x <= 1 || nCell.y <= 1 || nCell.z <=1){
    std::cerr << "number of cell on each diretion should be no less than 1. do nothing" << std::endl;
    return ;
  }
  
  // numInCell.resize (nCell.x * nCell.y * nCell.z);
  // std::fill (numInCell.begin(), numInCell.end(), 0);
  list.resize (nCell.x * nCell.y * nCell.z);

  ValueType rho = ValueType(numAtom) / (box.x * box.y * box.z);
  int expectedNum = rho * cellSize.x * cellSize.y * cellSize.z;
  expectedNum *= 2;
  for (unsigned i = 0; i < unsigned(nCell.x * nCell.y * nCell.z); ++i){
    list[i].reserve(expectedNum);
    list[i].clear ();
    // std::cout << "the capacity is " << list[i].capacity() << std::endl;
  }
}

bool CellList::
isEmpty () const
{
  return list.empty();
}

void CellList::
clear ()
{
  for (unsigned i = 0; i < unsigned(nCell.x * nCell.y * nCell.z); ++i){
    list[i].clear ();
    // std::cout << "the capacity is " << list[i].capacity() << std::endl;
  }
}

void CellList::
rebuild (const std::vector<std::vector<ValueType > > & coord)
{
  clear ();
  for (unsigned i = 0; i < coord.size(); ++i){
    unsigned index = calCellIndex (coord[i]);
    list[index].push_back (i);
  }
}

unsigned CellList::
calCellIndex (const std::vector<ValueType > & coord) const
{
  unsigned ix = coord[0] / cellSize.x;
  unsigned iy = coord[1] / cellSize.y;
  unsigned iz = coord[2] / cellSize.z;
  if (ix >= unsigned(nCell.x)){
    std::cerr << "error index " << std::endl;
    ix = nCell.x - 1;
  }
  if (iy >= unsigned(nCell.y)){
    std::cerr << "error index " << std::endl;
    iy = nCell.y - 1;
  }
  if (iz >= unsigned(nCell.z)){
    std::cerr << "error index " << std::endl;
    iz = nCell.z - 1;
  }
  return index3to1 (ix, iy, iz);
}



