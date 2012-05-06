#include "HydrogenBond.h"

static ValueType
calCosTheta (const std::vector<ValueType > & r0,
	     const std::vector<ValueType > & r1)
{
  ValueType dr0 = 0.;
  ValueType dr1 = 0.;
  ValueType r0r1 = 0.;
  
  for (unsigned dd = 0; dd < 3; ++dd){
    dr0 += r0[dd] * r0[dd];
    dr1 += r1[dd] * r1[dd];
    r0r1 += r0[dd] * r1[dd];
  }
  return r0r1 / (sqrt(dr0) * sqrt(dr1));
}

  

virtual bool
operator () (const std::vector<std::vector<ValueType > > & denotor,
	     const std::vector<std::vector<ValueType > > & acceptor) const
{
  std::vector<ValueType> & o0  (denotor[0]);
  std::vector<ValueType> & h00 (denotor[1]);
  std::vector<ValueType> & h01 (denotor[2]);
  std::vector<ValueType> & o1  (acceptor[0]);
  
  ValueType dist2 = 0.;
  std::vector<ValueType > doo (3);
  for (unsigned dd = 0; dd < 3; ++dd){
    doo[dd] = o1[dd] - o0[dd];
    dist2 += doo[dd] * doo[dd];
  }
  if (dist2 > roo2){
    return false;
  }

  ValueType cosTheta;
  std::vector<ValueType > dho (3);

  for (unsigned dd = 0; dd < 3; ++dd){
    dho[dd] = h00[dd] - o0[dd];
  }  
  cosTheta = calCosTheta (dho, doo);
  if (cosTheta > costhetahoo){
    return true;
  }

  for (unsigned dd = 0; dd < 3; ++dd){
    dho[dd] = h01[dd] - o0[dd];
  }  
  cosTheta = calCosTheta (dho, doo);
  if (cosTheta > costhetahoo){
    return true;
  }

  return false;
}

