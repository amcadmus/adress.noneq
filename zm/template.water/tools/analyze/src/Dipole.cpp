#include "Dipole.h"

void KirkwoodFactor::
reinit (const ValueType rup_,
	const ValueType refh_,
	const unsigned nDataBlock)
{
  rup = rup_;
  binSize = refh_;
  nbins = int(double(rup) / double(refh_));
  rup = binSize * (nbins);
  offset = 0.0;
  hist.resize (nbins);
  for (int ii = 0; ii < nbins; ++ii){
    hist[ii].reinit (nDataBlock);
  }
  nframe = 0;
}

void KirkwoodFactor::
deposit (const std::vector<std::vector<ValueType> > & coord,
	 const CellList & clist,
	 const std::vector<std::vector<ValueType> > & dipole,
	 const VectorType & box)
{
  int xiter = rup / clist.getCellSize().x;
  if (xiter * clist.getCellSize().x < rup) xiter ++;
  int yiter = rup / clist.getCellSize().y;
  if (yiter * clist.getCellSize().y < rup) yiter ++;
  int ziter = rup / clist.getCellSize().z;
  if (ziter * clist.getCellSize().z < rup) ziter ++;

  IntVectorType nCell = clist.getNumCell();
  ValueType myNatom = 0.;
  for (unsigned ii = 0; ii < clist.getTotalNumCell(); ++ii){
    myNatom += clist.getList()[ii].size();
  }
  double myNatomi = 1./myNatom;
  
  for (unsigned iCellIndex = 0;
       iCellIndex < unsigned(nCell.x * nCell.y * nCell.z);
       ++iCellIndex){
    std::vector<unsigned > neighborCellIndex =
	clist.neighboringCellIndex (iCellIndex, IntVectorType (xiter, yiter, ziter));
    for (unsigned iNeighborCellIndex = 0;
	 iNeighborCellIndex < neighborCellIndex.size();
	 ++iNeighborCellIndex){
      unsigned jCellIndex = neighborCellIndex[iNeighborCellIndex];
      for (unsigned ii = 0; ii < clist.getList()[iCellIndex].size(); ++ii){
	VectorType icoord;
	icoord.x = coord[clist.getList()[iCellIndex][ii]][0];
	icoord.y = coord[clist.getList()[iCellIndex][ii]][1];
	icoord.z = coord[clist.getList()[iCellIndex][ii]][2];
	double dipoleMi = 0;
	for (int dd = 0; dd < 3; ++dd){
	  dipoleMi += dipole[ii][dd] * dipole[ii][dd];
	}
	dipoleMi = sqrt(dipoleMi);	
	bool sameCell (iCellIndex == jCellIndex);
	for (unsigned jj = 0; jj < clist.getList()[jCellIndex].size(); ++jj){
	  if (sameCell && ii == jj) continue;	    
	  VectorType jcoord;
	  jcoord.x = coord[clist.getList()[jCellIndex][jj]][0];
	  jcoord.y = coord[clist.getList()[jCellIndex][jj]][1];
	  jcoord.z = coord[clist.getList()[jCellIndex][jj]][2];
	  VectorType diff;
	  diff.x = - icoord.x + jcoord.x;
	  diff.y = - icoord.y + jcoord.y;
	  diff.z = - icoord.z + jcoord.z;
	  if      (diff.x < -.5 * box.x) diff.x += box.x;
	  else if (diff.x >= .5 * box.x) diff.x -= box.x;
	  if      (diff.y < -.5 * box.y) diff.y += box.y;
	  else if (diff.y >= .5 * box.y) diff.y -= box.y;
	  if      (diff.z < -.5 * box.z) diff.z += box.z;
	  else if (diff.z >= .5 * box.z) diff.z -= box.z;
	  ValueType dr = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
	  dr = sqrt (dr);
	  unsigned index = (dr + offset) / binSize;

	  if (dr < rup){
	    double dipoleCorr = 0;
	    double dipoleMj = 0;
	    for (int dd = 0; dd < 3; ++dd){
	      dipoleCorr += dipole[ii][dd] * dipole[jj][dd];
	      dipoleMj += dipole[jj][dd] * dipole[jj][dd];
	    }
	    dipoleMj = sqrt(dipoleMj);
	    dipoleCorr /= (dipoleMi * dipoleMj);
	    // if (index >= unsigned(nbins)){
	    //   // printf ("# dr: %f, index: %d, rup: %f, nbins: %d\n",
	    //   // 	      dr, index, rup, nbins);
	    //   index = nbins - 1;
	    // }
	    // hist[index] += 1.;
	    for (unsigned ll = index+1; ll < hist.size(); ++ll){
	      hist[ll].deposite (dipoleCorr * myNatomi);
	    }
	  }
	}
      }
    }
  }
}

void KirkwoodFactor::
calculate ()
{
  for (int ii = 0; ii < nbins; ++ii){
    hist[ii].calculate();
  }
}


double KirkwoodFactor::
getAvg (const unsigned & ii) const
{
  return hist[ii].getAvg();
}

double KirkwoodFactor::
getAvgError (const unsigned & ii) const
{
  return hist[ii].getAvgError();
}

