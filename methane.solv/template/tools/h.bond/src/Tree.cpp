#include "Tree.h"
#include <iostream>
#include <fstream>

inline TreePosition::
TreePosition (const unsigned g, const unsigned b)
    : genId(g), broId(b)
{
}

TreeNode::
TreeNode(const Identity & id)
    // : numFather(0), numBother(0), numSon(0)
    : identity (id)
{
}

unsigned Generation::
size () const 
{
  return brothers.size();
}

void Tree::
buildBrothers(const HbondMap & map)
{
  unsigned newGenId = generations.size() - 1;
  if (generations[newGenId].brothers.size() < 2){
    return;
  }

  for (unsigned ii = 0; ii < generations[newGenId].size(); ++ii){
    std::vector<Identity > ineighbor;
    map.findNeighbors (generations[newGenId].brothers[ii].identity, ineighbor);
    TreePosition iposi (newGenId, ii);
    for (unsigned jj = ii+1; jj < generations[newGenId].size(); ++jj){
      for (unsigned kk = 0; kk < ineighbor.size(); ++kk){
	if (ineighbor[kk] == generations[newGenId].brothers[jj].identity){
	  TreePosition jposi (newGenId, jj);
	  generations[newGenId].brothers[ii].vecBrother.push_back (jposi);
	  generations[newGenId].brothers[jj].vecBrother.push_back (iposi);
	  break;
	}
      }
    }
  }
} 

void Tree::
addRoot (const Identity & id)
{
  generations.push_back(Generation());
  generations[0].brothers.push_back(TreeNode(id));
}

inline TreeNode & Tree::
getTreeNode (const TreePosition & p)
{
  return generations[p.genId].brothers[p.broId];
}

inline const TreeNode & Tree::
getTreeNode (const TreePosition & p) const
{
  return generations[p.genId].brothers[p.broId];
}

bool Tree::
addGeneration (const HbondMap & map)
{
  if (generations.size() == 0){
    std::cerr << "cannot add genaration without adding a root"<< std::endl;
    return false;
  }
  // std::cout << generations.size() << std::endl;
  generations.push_back(Generation());  
  Generation & oldGen (generations[generations.size() - 2]);
  Generation & newGen (generations[generations.size() - 1]);
  
  for (unsigned ii = 0; ii < oldGen.size(); ++ii){
    TreeNode & me (oldGen.brothers[ii]);
    TreePosition myPosi (generations.size() - 2, ii);
    std::vector<Identity > idSon;
    map.findNeighbors (me.identity, idSon);
    std::vector<Identity >::iterator itIdSon = idSon.begin();
    // remove father and brothers from neighbors
    for (; itIdSon != idSon.end(); itIdSon ++){
      for (unsigned jj = 0; jj < me.numFather(); ++jj){
	TreePosition fPosi = me.vecFather[jj];
	if (getTreeNode(fPosi).identity == (*itIdSon)){
	  idSon.erase(itIdSon);
	  break;
	}
      }
    }
    itIdSon = idSon.begin();
    for (; itIdSon != idSon.end(); itIdSon ++){
      for (unsigned jj = 0; jj < me.numBrother(); ++jj){
	TreePosition bPosi = me.vecBrother[jj];
	if (getTreeNode(bPosi).identity == (*itIdSon)){
	  idSon.erase(itIdSon);
	  break;
	}
      }
    }
    // end remove father and brothers from neighbors
    for (unsigned jj = 0; jj < idSon.size(); ++jj){
      // check if the son is already in the generation.
      bool find = false;
      for (unsigned kk = 0; kk < newGen.brothers.size(); ++kk){
	if (newGen.brothers[kk].identity == idSon[jj]){
	  find = true;
	  // me.vecBrother.push_back (&newGen.brothers[kk]);
	  me.vecSon.push_back (TreePosition(generations.size() - 1, kk));
	  newGen.brothers[kk].vecFather.push_back (myPosi);
	  break;
	}
      }
      // if do not find, creat new tree node.
      if (find == false){
	newGen.brothers.push_back (TreeNode(idSon[jj]));
	me.vecSon.push_back(TreePosition(generations.size()-1, newGen.brothers.size()-1));
	newGen.brothers.back().vecFather.push_back (myPosi);
      }
    }
  }

  buildBrothers(map);

  if (newGen.brothers.size() != 0){
    return true;
  }
  else {
    generations.pop_back();
    return false;
  }
}

      
  // me.vecSon.push_back (generations.back().identity);
  // generations.back().push_back (TreeNode (idSon[jj]));
  // generations.back().back().vecFather.push_back (&me);
      

void Tree::
print () const
{
  for (unsigned ii = 0; ii < generations.size(); ++ii){
    printf ("Level: %d\n", ii);
    for (unsigned jj = 0; jj < generations[ii].size(); ++jj){
      printf ("item %d:\t\t", generations[ii].brothers[jj].identity);
      printf ("Fathers: ");
      for (unsigned kk = 0; kk < generations[ii].brothers[jj].numFather(); ++kk){
	TreePosition fatherPosi = generations[ii].brothers[jj].vecFather[kk];
	printf ("%d ", getTreeNode(fatherPosi).identity);
      }
      printf (" Sons: ");
      for (unsigned kk = 0; kk < generations[ii].brothers[jj].numSon(); ++kk){
	TreePosition sonPosi = generations[ii].brothers[jj].vecSon[kk];
	printf ("%d ", getTreeNode(sonPosi).identity);
      }
      printf (" Brothers: ");
      for (unsigned kk = 0; kk < generations[ii].brothers[jj].numBrother(); ++kk){
	TreePosition sonPosi = generations[ii].brothers[jj].vecBrother[kk];
	printf ("%d ", getTreeNode(sonPosi).identity);
      }
      printf ("\n");
    }
    printf ("\n");
  }
}

      
	
