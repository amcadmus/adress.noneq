#ifndef __Tree_h_wanghan__
#define __Tree_h_wanghan__

#include <vector>
#include "Defines.h"

struct TreeNode;
struct TreeNode 
{
  Indentity identity;
  // unsigned numFather;
  // unsigned numBother;
  // unsigned numSon;
  std::vector<TreeNode *> vecFather;
  std::vector<TreeNode *> vecBrother;
  std::vector<TreeNode *> vecSon;
  TreeNode (const Indentity & id);
  unsigned numFather  () const {return vecFather.size();}
  unsigned numBrother () const {return vecBrother.size();}
  unsigned numSon     () const {return vecSon.size();}
}
    ;

Struct Generation
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
  void addGeneration (const HbondMap & map);
}
    ;


TreeNode::
TreeNode(const Identity & id)
    // : numFather(0), numBother(0), numSon(0)
    : identity (id)
{
}

unsigned Generations::
size () const 
{
  return brothers.size();
}

const Generation & Generation::
lastGeneration () const 
{
  return generations.back();
}



#endif
