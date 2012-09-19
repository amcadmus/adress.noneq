#ifndef __Circles_h_wanghan__
#define __Circles_h_wanghan__

#include "Defines.h"
#include <vector>
#include <algorithm>

typedef std::vector<Identity> Circle;

struct Circles
{
  std::vector<Circle > circles;
  std::vector<unsigned > count;
private:
  bool find (const Circle & c);
public:
  void clear ();
  void uniqueCircles ();
  int  simplifyCircles ();
  void combineSimilar ();
  void sortCircles ();
  void add (const Circles & c);
  void print () const;
  void countPoly (const unsigned maxPoly=14);
  const std::vector<Circle > & getCircles () const {return circles;}
}
    ;

namespace CircleOperations{
  Circle diffCircle (const Circle & c0,
		     const Circle & c1,
		     int & numFail);
  int normIdx (const Circle & c0,
	       const int i);
  bool find3Pattern (const Circle & c,
		     const std::vector<Identity > & pattern);
  bool findCommonPatterns (const Circle & c0_,
			   const Circle & c1_,
			   std::vector<std::vector<unsigned > > & c0_pattern,
			   std::vector<std::vector<unsigned > > & c1_pattern,
			   int & numFail);
}

inline void Circles::
clear ()
{
  circles.clear ();
}

// inline const std::vector<Circle > & Circle::
// getCircles () const
// {
//   return circles;
// }

inline void Circles::
add (const Circles & c)
{
  circles.insert (circles.end(), c.circles.begin(), c.circles.end());
}

inline void Circles::
sortCircles ()
{
  std::sort (circles.begin(), circles.end());
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

