#include "Rdf.h"
#include <iostream>
#include <stdio.h>

void Rdf::
reinit (const ValueType rup_,
	const ValueType refh_)
{
  rup = rup_;
  binSize = refh_;
  
  nbins = rup / refh_ + 1;
  rup = binSize * (nbins - .5);
  offset = .5 * binSize;
  hist.resize (nbins);
  std::fill (hist.begin(), hist.end(), 0.);
  nframe = 0;
  rho = 0.;
}

void Rdf::
deposit (const std::vector<std::vector<ValueType> > & coord,
	 const VectorType & box,
	 const CellList & clist)
{
  int xiter = rup / clist.getCellSize().x;
  if (xiter * clist.getCellSize().x < rup) xiter ++;
  int yiter = rup / clist.getCellSize().y;
  if (yiter * clist.getCellSize().y < rup) yiter ++;
  int ziter = rup / clist.getCellSize().z;
  if (ziter * clist.getCellSize().z < rup) ziter ++;

  IntVectorType nCell = clist.getNumCell();
  
  for (int ix = 0; ix < nCell.x; ++ix){
    for (int iy = 0; iy < nCell.y; ++iy){
      for (int iz = 0; iz < nCell.z; ++iz){
	unsigned iid = clist.index3to1 (ix, iy, iz);
	for (int dx = -xiter; dx <= xiter; ++dx){
	  int jx = ix + dx;
	  if (jx < 0) jx += nCell.x;
	  else if (jx >= nCell.x) jx -= nCell.x;
	  for (int dy = -yiter; dy <= yiter; ++dy){
	    int jy = iy + dy;
	    if (jy < 0) jy += nCell.y;
	    else if (jy >= nCell.y) jy -= nCell.y;
	    for (int dz = -ziter; dz <= ziter; ++dz){
	      int jz = iz + dz;
	      if (jz < 0) jz += nCell.z;
	      else if (jz >= nCell.z) jz -= nCell.z;
	      unsigned jid = clist.index3to1 (jx, jy, jz);
	      bool sameCell (0 == dx && 0 == dy && 0 == dz);
	      for (unsigned ii = 0; ii < clist.getList()[iid].size(); ++ii){
		VectorType icoord;
		icoord.x = coord[clist.getList()[iid][ii]][0];
		icoord.y = coord[clist.getList()[iid][ii]][1];
		icoord.z = coord[clist.getList()[iid][ii]][2];
		for (unsigned jj = 0; jj < clist.getList()[jid].size(); ++jj){
		  if (sameCell && ii == jj) continue;
		  VectorType jcoord;
		  jcoord.x = coord[clist.getList()[jid][jj]][0];
		  jcoord.y = coord[clist.getList()[jid][jj]][1];
		  jcoord.z = coord[clist.getList()[jid][jj]][2];
		  VectorType diff;
		  diff.x = icoord.x - jcoord.x;
		  diff.y = icoord.y - jcoord.y;
		  diff.z = icoord.z - jcoord.z;
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
		    if (index >= unsigned(nbins)){
		      // printf ("# dr: %f, index: %d, rup: %f, nbins: %d\n",
		      // 	      dr, index, rup, nbins);
		      index = nbins - 1;
		    }
		    hist[index] += 1.;
		  }
		}
	      }	    
	    }
	  }
	}
      }
    }
  }

  nframe ++;
  rho += coord.size() / (box.x * box.y * box.z);
  natom = coord.size();
}

void Rdf::
calculate()
{
  rho /= double(nframe);
  for (int i = 0; i < nbins; ++i){
    hist[i] /= double(nframe * natom);
  }
  {
    double r = 0.5 * binSize;
    hist[0] /= 4. / 3. * M_PI * r * r * r * rho;
  }
  for (int i = 1; i < nbins; ++i){
    double r0 = (i-0.5) * binSize;
    double r1 = (i+0.5) * binSize;
    double r01 = i * binSize;
    hist[i] /= 4. / 3. * M_PI * (r1*r1*r1 - r0*r0*r0) * rho;
    // hist[i] /= 4. * M_PI * r0 * r1 * (r1 - r0) * rho;
  }
}



