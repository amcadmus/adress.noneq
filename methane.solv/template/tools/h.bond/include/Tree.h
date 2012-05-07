#ifndef __Tree_h_wanghan__
#define __Tree_h_wanghan__

#include <vector>
#include "Defines.h"
#include "HbondMap.h"

struct TreePosition
{
  unsigned genId;
  unsigned broId;
  TreePosition (const unsigned g, const unsigned b) ;
};

struct TreeNode;
struct TreeNode 
{
  Identity identity;
  // unsigned numFather;
  // unsigned numBother;
  // unsigned numSon;
  std::vector<TreePosition> vecFather;
  std::vector<TreePosition> vecBrother;
  std::vector<TreePosition> vecSon;
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
  const Generation & lastGeneration () const;
  TreeNode & getTreeNode (const TreePosition & p);
  const TreeNode & getTreeNode (const TreePosition & p) const;
  void buildBrothers (const HbondMap & map);
public:
  void clear ();
  void addRoot (const Identity & id);
  bool addGeneration (const HbondMap & map);
  void print () const;
}
    ;

// const Generation & Generation::
// lastGeneration () const 
// {
//   return generations.back();
// }

#endif
