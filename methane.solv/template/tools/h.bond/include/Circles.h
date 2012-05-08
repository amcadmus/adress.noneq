#ifndef __Circles_h_wanghan__
#define __Circles_h_wanghan__

#include "Defines.h"
#include <vector>

typedef std::vector<Identity> Circle;

struct Circles
{
  std::vector<Circle > circles;
private:
  bool find (const Circle & c);
public:
  void uniqueCircles ();
  void simplifyCircles ();
  void add (const Circles & c);
  void print () const;
}
    ;

namespace CircleOperations{
  Circle diffCircle (const Circle & c0,
		     const Circle & c1);
  int normIdx (const Circle & c0,
	       const int i);
  bool findCommonPatterns (const Circle & c0_,
			   const Circle & c1_,
			   std::vector<std::vector<unsigned > > & c0_pattern,
			   std::vector<std::vector<unsigned > > & c1_pattern);
}


inline void Circles::
add (const Circles & c)
{
  circles.insert (circles.end(), c.circles.begin(), c.circles.end());
}


inline int CircleOperations::
normIdx (const Circle & c0,
	     const int i)
{
  int size = c0.size();
  if (i < 0){
    return i + size;
  }
  else if (i >= size){
    return i - size;
  }
  else{
    return i;
  }
}




#endif

