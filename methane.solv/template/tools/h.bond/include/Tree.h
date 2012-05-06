#ifndef __Tree_h_wanghan__
#define __Tree_h_wanghan__

#include <vector>
#include "Defines.h"
#include "HbondMap.h"

struct TreeNode;
struct TreeNode 
{
  Identity identity;
  // unsigned numFather;
  // unsigned numBother;
  // unsigned numSon;
  std::vector<TreeNode *> vecFather;
  std::vector<TreeNode *> vecBrother;
  std::vector<TreeNode *> vecSon;
  TreeNode (const Identity & id);
  unsigned numFather  () const {return vecFather.size();}
  unsigned numBrother () const {return vecBrother.size();}
  unsigned numSon     () const {return vecSon.size();}
}
    ;

struct Generation
{
  std::vector<TreeNode > brothers;
  unsigned size () const;
};

class Tree 
{
  std::vector<Generation > generations;
public:
  void clear ();
  const Generation & lastGeneration () const;
  bool addGeneration (const HbondMap & map);
}
    ;

// const Generation & Generation::
// lastGeneration () const 
// {
//   return generations.back();
// }

#endif
