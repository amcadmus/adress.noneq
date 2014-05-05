#include "CellList.h"
#include <iostream>

CellList::
CellList (const unsigned & numAtom,
	  const VectorType & box_,
	  const ValueType & cellSize_)
{
  reinit (numAtom, box_, cellSize_);
}

CellList::
CellList (const VectorType & box_,
	  const ValueType & cellSize_)
{
  reinit (box_, cellSize_);
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

void CellList::
reinit (const VectorType & box_,
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

  for (unsigned i = 0; i < unsigned(nCell.x * nCell.y * nCell.z); ++i){
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


std::vector<unsigned > CellList::
neighboringCellIndex (const unsigned cellIndex,
		      const IntVectorType nNei_) const
{
  std::vector<unsigned > cells;
  IntVectorType nNei (nNei_);
  if (nNei.x > ((nCell.x - 1) >> 1)) nNei.x = ((nCell.x - 1) >> 1);
  if (nNei.y > ((nCell.y - 1) >> 1)) nNei.y = ((nCell.y - 1) >> 1);
  if (nNei.z > ((nCell.z - 1) >> 1)) nNei.z = ((nCell.z - 1) >> 1);

  IntVectorType refIdx, tgtIdx;
  index1to3 (cellIndex, refIdx.x, refIdx.y, refIdx.z);

  IntVectorType dd;
  for (dd.x = -nNei.x; dd.x <= nNei.x; ++dd.x){
    tgtIdx.x = refIdx.x + dd.x;
    if (tgtIdx.x < 0) tgtIdx.x += nCell.x;
    else if (tgtIdx.x >= nCell.x) tgtIdx.x -= nCell.x;
    for (dd.y = -nNei.y; dd.y <= nNei.y; ++dd.y){
      tgtIdx.y = refIdx.y + dd.y;
      if (tgtIdx.y < 0) tgtIdx.y += nCell.y;
      else if (tgtIdx.y >= nCell.y) tgtIdx.y -= nCell.y;
      for (dd.z = -nNei.z; dd.z <= nNei.z; ++dd.z){
	tgtIdx.z = refIdx.z + dd.z;
	if (tgtIdx.z < 0) tgtIdx.z += nCell.z;
	else if (tgtIdx.z >= nCell.z) tgtIdx.z -= nCell.z;
	if (0 == dd.x && 0 == dd.y && 0 == dd.z) continue;
	cells.push_back (index3to1(tgtIdx.x, tgtIdx.y, tgtIdx.z));
      }
    }
  }
  // count the cell itself
  cells.push_back (cellIndex);

  std::sort (cells.begin(), cells.end());
  std::unique (cells.begin(), cells.end());
  
  return cells;
}



