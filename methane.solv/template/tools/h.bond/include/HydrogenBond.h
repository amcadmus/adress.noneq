#ifndef __HydrogenBond_h_wanghan__
#define __HydrogenBond_h_wanghan__

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <cmath>

#include "Defines.h"

// denotor and acceptor are two water molecules,
// we assume the 0th variable is the oxygen
// the 1st and 2nd variables are the two hydrogens
class HydrogenBond 
{
public:
  virtual bool operator () (const std::vector<std::vector<ValueType > > & denotor,
			    const std::vector<std::vector<ValueType > > & acceptor) const = 0;
}
    ;

// r [nm] is the O-O distance thread 
// theta [deg] is the H-O-O angle thread 
class HydrogenBond_Geo_1
{
  ValueType roo2;
  ValueType costhetahoo;
public:
  HydrogenBond_Geo_1 (const ValueType r = 0.35,
		      const ValueType theta = 30);
  virtual bool operator () (const std::vector<std::vector<ValueType > > & denotor,
			    const std::vector<std::vector<ValueType > > & acceptor) const;
}
    ;


HydrogenBond_Geo_1::
HydrogenBond_Geo_1 (const ValueType r,
		    const ValueType theta)
    : roo2(r*r), costhetahoo(cos(theta / 180. * M_PI))
{
}


#endif
