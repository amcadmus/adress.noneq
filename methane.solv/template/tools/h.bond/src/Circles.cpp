#include "Circles.h"
#include <stdio.h>
#include <iostream>
#include <algorithm>

using namespace CircleOperations;

void Circles::
print () const
{
  for (unsigned ii = 0; ii < circles.size(); ++ii){
    for (unsigned jj = 0; jj < circles[ii].size(); ++jj){
      printf ("%d ", circles[ii][jj]);
    }
    printf("\n");
  }
}

static bool
sameCircle (const Circle & c0,
	    const Circle & c1)
{
  bool value = false;
  if (c0.size() == c1.size()){
    bool findDiff = false;
    for (unsigned ii = 0; ii < c0.size(); ++ii){
      if (c0[ii] != c1[ii]){
	findDiff = true;
	break;
      }
    }
    if (!findDiff) value = true;
  }
  return value;
}


void Circles::
uniqueCircles ()
{
  unsigned size = circles.size();
  std::vector<bool > delMark (size, false);
  std::vector<Circle > sortedCircles (circles);

  for (unsigned ii = 0; ii < size; ++ii){
    std::sort(sortedCircles[ii].begin(), sortedCircles[ii].end());
  }

  for (unsigned ii = 0; ii < size; ++ii){
    if (delMark[ii]) continue;
    for (unsigned jj = ii+1; jj < size; ++jj){
      if (sameCircle(sortedCircles[ii], sortedCircles[jj])){
	delMark[jj] = true;
      }
    }
  }

  std::vector<Circle > tmpCircles (circles);
  circles.clear();
  for (unsigned ii = 0; ii < size; ++ii){
    if (!delMark[ii]){
      circles.push_back(tmpCircles[ii]);
    }
  }
}


bool CircleOperations::
findCommonPatterns (const Circle & c0_,
		    const Circle & c1_,
		    std::vector<std::vector<unsigned > > & c0_pattern,
		    std::vector<std::vector<unsigned > > & c1_pattern)
{
  c0_pattern.clear();
  c1_pattern.clear();
  bool find = false;  
  if (c0_.size() < 3 || c1_.size() < 3){
    return find;
  }
  Circle c0(c0_);
  Circle c1(c1_);
  
  int iId0, iId1, iId2;
  int jId0, jId1, jId2;
  std::vector<unsigned > tmp0;
  std::vector<unsigned > tmp1;

  iId1 = 0;
  iId2 = 1;
  for (int ii = 0; ii < int(c0.size()); ++ii){
    iId0 = iId1;
    iId1 = iId2;
    iId2 ++;
    iId2 = normIdx (c0, iId2);
    jId1 = 0;
    jId2 = 1;
    for (int jj = 0; jj < int(c1.size()); ++jj){
      jId0 = jId1;
      jId1 = jId2;
      jId2 ++;
      jId2 = normIdx (c1, jId2);
      if (c0[iId0] == c1[jId0] &&
	  c0[iId1] == c1[jId1] &&
	  c0[iId2] == c1[jId2] ){
	find = true;
	tmp0.push_back(iId0);
	tmp0.push_back(iId1);
	tmp0.push_back(iId2);
	tmp1.push_back(jId0);
	tmp1.push_back(jId1);
	tmp1.push_back(jId2);
	iId2 ++;
	jId2 ++;
	iId2 = normIdx (c0, iId2);
	jId2 = normIdx (c1, jId2);
	while (c0[iId2] == c1[jId2]){
	  tmp0.push_back(iId2);
	  tmp1.push_back(jId2);	  
	  iId2 ++;
	  jId2 ++;
	  iId2 = normIdx (c0, iId2);
	  jId2 = normIdx (c1, jId2);
	}
	iId0 --;
	jId0 --;
	iId0 = normIdx (c0, iId0);
	jId0 = normIdx (c1, jId0);
	while (c0[iId0] == c1[jId0]){
	  tmp0.insert (tmp0.begin(), iId0);
	  tmp1.insert (tmp1.begin(), jId0);	  
	  iId0 --;
	  jId0 --;
	  iId0 = normIdx (c0, iId0);
	  jId0 = normIdx (c1, jId0);
	}
	goto out100;
      }
    }
  }
  
  out100:
  if (find){
    if (c0.size() - tmp0.size() <= 2 ||
	c1.size() - tmp1.size() <= 2 ){
      goto out200;
    }
    bool find2 = false;
    int iId_Id0, iId_Id1, iId_Id2;
    int jId_Id0, jId_Id1, jId_Id2;
    for (iId_Id2 = iId2; iId_Id2 != iId0;){
      iId_Id0 = iId_Id2 + 1;
      iId_Id1 = iId_Id2 + 2;
      iId_Id0 = normIdx(c0, iId_Id0);
      iId_Id1 = normIdx(c0, iId_Id1);
      for (jId_Id2 = jId2; jId_Id2 != jId0;){
	jId_Id0 = jId_Id2 + 1;
	jId_Id1 = jId_Id2 + 2;
	jId_Id0 = normIdx(c1, jId_Id0);
	jId_Id1 = normIdx(c1, jId_Id1);
	// printf ("%d %d\n", iId_Id2, jId_Id2);
  	if (c0[iId_Id0] == c1[jId_Id0] &&
  	    c0[iId_Id1] == c1[jId_Id1] &&
  	    c0[iId_Id2] == c1[jId_Id2] ){
  	  find2 = true;
  	  break;
  	}
	++jId_Id2;
	jId_Id2 = normIdx(c1, jId_Id2);
      }
      ++iId_Id2;
      iId_Id2 = normIdx(c0, iId_Id2);
    }
    if (find2){
      std::cerr << "find 2 matched patterns" << std::endl;
      return false;
    }
  }
  
  out200:  
  if (find){
    c0_pattern.push_back(tmp0);
    c1_pattern.push_back(tmp1);
    for (unsigned ii = 0; ii < tmp0.size(); ++ii){
      printf ("%d ", tmp0[ii]);
    }
    printf ("\n");
    for (unsigned ii = 0; ii < tmp1.size(); ++ii){
      printf ("%d ", tmp1[ii]);
    }
    printf ("\n");
  }
  return find;
}


  
// Circle diffCircle (const Circle & c0,
// 		   const Circle & c1)
// {
//   int iId0, iId1, iId2;
//   int jId0, jId1, jId2;
//   iId0 = 0;
//   iId1 = 1;
//   iId2 = 2;
  
//   for (int ii = 0; ii < int(c0.size()); ++ii){
//     jId0 = 0;
//     jId1 = 1;
//     jId2 = 2;
//     for (int jj = 0; jj < int(c1.size()); ++jj){
//       if (c0[ii+0] == c1[jj+0] &&
// 	  c0[c0.normIdx(ii+1)] == c1[c1.normIdx(jj+1)] &&
// 	  c0[c0.normIdx(ii+2)] == 
