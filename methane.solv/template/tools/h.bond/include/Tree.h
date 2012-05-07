#ifndef __Tree_h_wanghan__
#define __Tree_h_wanghan__

#include <vector>
#include "Defines.h"
#include "HbondMap.h"
#include "Circles.h"

struct TreePosition
{
  unsigned genId;
  unsigned broId;
  TreePosition (const unsigned g, const unsigned b) ;
  bool operator == (const TreePosition & p) const;
  bool operator != (const TreePosition & p) const;
};

typedef std::vector<TreePosition>	Cable;
typedef std::vector<Cable>		Cables;

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
  Cables cables;
public:
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
private:
  const Generation & lastGeneration () const;
  TreeNode & getTreeNode (const TreePosition & p);
  const TreeNode & getTreeNode (const TreePosition & p) const;
  void buildBrothers (const HbondMap & map);
  void buildCables (const TreePosition & posi);
  Circles buildCircleCable (const Cable & c0,
			    const Cable & c1) const;
public:
  bool empty		() const;
  bool isCircular	() const;
  void clear		();
  void addRoot		(const Identity & id);
  bool addGeneration	(const HbondMap & map);
  void buildCables	();
  void buildCircles	(Circles & cir) const;
  void print		() const;
}
    ;

inline bool TreePosition::
operator == (const TreePosition & p) const
{
  return (genId == p.genId && broId == p.broId);
}

inline bool TreePosition::
operator != (const TreePosition & p) const
{
  return (genId != p.genId || broId != p.broId);
}

inline void Tree::
clear () {
  generations.clear();
}

inline bool Tree::
empty () const
{
  return generations.empty();
}


// const Generation & Generation::
// lastGeneration () const 
// {
//   return generations.back();
// }

#endif
