#include "Tree.h"

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

bool Tree::
addGeneration (const HbondMap & map)
{
  Generation & oldGen (generations.back());
  generations.push_back(Generation());
  Generation & newGen (generations.back());
  
  for (unsigned ii = 0; ii < oldGen.size(); ++ii){
    TreeNode & me (oldGen.brothers[ii]);
    std::vector<Identity > idSon;
    map.findNeighbors (me.identity, idSon);
    std::vector<Identity >::iterator itIdSon = idSon.begin();
    // remove father and brothers from neighbors
    for (; itIdSon != idSon.end(); itIdSon ++){
      for (unsigned jj = 0; jj < me.numFather(); ++jj){
	if (me.vecFather[jj]->identity == (*itIdSon)){
	  idSon.erase(itIdSon);
	  break;
	}
      }
    }
    for (; itIdSon != idSon.end(); itIdSon ++){
      for (unsigned jj = 0; jj < me.numBrother(); ++jj){
	if (me.vecBrother[jj]->identity == (*itIdSon)){
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
	  me.vecBrother.push_back (&newGen.brothers[kk]);
	  newGen.brothers[kk].vecFather.push_back (&me);
	  break;
	}
      }
      // if do not fine, creat new tree node.
      if (find == false){
	newGen.brothers.push_back (TreeNode(idSon[jj]));
	me.vecSon.push_back(&(newGen.brothers.back()));
	newGen.brothers.back().vecFather.push_back (&me);
      }
    }
  }

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
      
      
      
	
