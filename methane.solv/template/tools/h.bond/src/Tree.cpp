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

bool Tree::
isCircular () const
{
  if (generations.size() == 0){
    return false;
  }
  return (generations[0].brothers[0].numSon() > 1);
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
    std::vector<Identity > idSon, tmpIdSon;
    map.findNeighbors (me.identity, idSon);
    tmpIdSon = idSon;
    idSon.clear();
    for (unsigned kk = 0; kk < tmpIdSon.size(); ++kk){
      bool find = false;
      for (unsigned jj = 0; jj < me.numFather(); ++jj){
	TreePosition fPosi = me.vecFather[jj];
	if (getTreeNode(fPosi).identity == tmpIdSon[kk]){
	  find = true;
	  break;
	}
      }
      if (!find){
	idSon.push_back(tmpIdSon[kk]);
      }
    }
    tmpIdSon = idSon;
    idSon.clear();
    for (unsigned kk = 0; kk < tmpIdSon.size(); ++kk){
      bool find = false;
      for (unsigned jj = 0; jj < me.numBrother(); ++jj){
	TreePosition bPosi = me.vecBrother[jj];
	if (getTreeNode(bPosi).identity == tmpIdSon[kk]){
	  find = true;
	  break;
	}
      }
      if (!find){
	idSon.push_back(tmpIdSon[kk]);
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
buildCables (const TreePosition & posi)
{
  TreeNode & node (getTreeNode(posi));
  if (node.cables.size() != 0){
    return;
  }
  // root node
  if (node.numFather() == 0){
    node.cables.push_back (Cable());
    node.cables[0].push_back (posi);
    return;
  }
  // otherwise
  for (unsigned ii = 0; ii < node.numFather(); ++ii){
    buildCables (node.vecFather[ii]);
    TreeNode & fatherNode (getTreeNode(node.vecFather[ii]));    
    Cable tmpCable;
    for (unsigned jj = 0; jj < fatherNode.cables.size(); ++jj){
      tmpCable = fatherNode.cables[jj];
      tmpCable.push_back (posi);
      node.cables.push_back(tmpCable);
    }
  }
}

void Tree::
buildCables ()
{
  for (unsigned ii = 0; ii < generations.size(); ++ii){
    for (unsigned jj = 0; jj < generations[ii].brothers.size(); ++jj){
      TreePosition posi(ii, jj);
      if (getTreeNode(posi).numSon() == 0) {
	buildCables (posi);
      }
    }
  }
}      
  
//   unsigned newGenId = generations.size() - 1;
//   for (unsigned ii = 0; ii < generations[newGenId].brothers.size(); ++ii){
//     TreePosition posi(newGenId, ii);
//     buildCables (posi);
//   }
// }
    

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
      printf (" No. Cables: ");
      printf ("%d ", generations[ii].brothers[jj].cables.size());
      printf ("\n");
    }
    printf ("\n");
  }

  // unsigned newGenId = generations.size() - 1;
  // for (unsigned ii = 0; ii < generations[newGenId].brothers.size(); ++ii){
  //   TreePosition posi(newGenId, ii);
  //   const TreeNode & node (getTreeNode(posi));
  //   printf ("NodePosi: %d %d,   id: %d\n", newGenId, ii, node.identity);
  //   for (unsigned jj = 0; jj < node.cables.size(); ++jj){
  //     printf ("Cable No. %d: ", jj);
  //     for (unsigned kk = 0; kk < node.cables[jj].size(); ++kk){
  // 	printf ("%d ", getTreeNode(node.cables[jj][kk]).identity);
  //     }
  //     printf ("\n");
  //   }
  // }
}


Circles Tree::
buildCircleCable (const Cable & c0,
		  const Cable & c1) const
{
  if (c0.size() != c1.size()){
    std::cerr << "cable size do not match, cannot build circle" << std::endl;
    return Circles();
  }
  unsigned size = c0.size();
  if (size <= 1) {
    return Circles();
  }
  unsigned idx0 = 0;
  unsigned idx2 = size;
  unsigned idx1 = 1;

  Circles tmp;
  while (idx1 != idx2){
    if (c0[idx1] == c1[idx1]){
      if (idx1 - idx0 >= 2){
	Circle tmpCir;
	for (unsigned ii = idx0; ii <= idx1; ++ii){
	  tmpCir.push_back (getTreeNode(c0[ii]).identity);
	}
	for (unsigned ii = idx1 - 1; ii >= idx0+1; --ii){
	  tmpCir.push_back (getTreeNode(c1[ii]).identity);
	}
	tmp.circles.push_back(tmpCir);
      }
      idx0 = idx1;
      idx1 ++;
    }
    else {
      idx1 ++;
    }
  }

  if (c0[idx2-1] != c1[idx2-1]){
    Circle tmpCir;
    for (unsigned ii = idx0; ii <= idx2-1; ++ii){
      tmpCir.push_back (getTreeNode(c0[ii]).identity);
    }
    for (unsigned ii = idx2-1; ii >= idx0+1; --ii){
      tmpCir.push_back (getTreeNode(c1[ii]).identity);
    }
    tmp.circles.push_back(tmpCir);
  }

  return tmp;
}


void Tree::
buildCircles (Circles & cir) const
{
  // unsigned newGenId = generations.size() - 1;
  // const Generation & newGen (generations[newGenId]);
  
  cir.circles.clear();

  // // build from end nodes
  // for (unsigned ii = 0; ii < newGen.brothers.size(); ++ii){
  //   const TreeNode & me ((newGen.brothers[ii]));
  //   for (unsigned jj = 0; jj < me.cables.size(); ++jj){
  //     for (unsigned kk = jj+1; kk < me.cables.size(); ++kk){
  // 	Circles tmp = buildCircleCable (me.cables[jj], me.cables[kk]);
  // 	cir.add (tmp);
  //     }
  //   }
  // }

  // build from end nodes
  for (unsigned ii = 2; ii < generations.size(); ++ii){
    for (unsigned ll = 0; ll < generations[ii].brothers.size(); ++ll){
      const TreeNode & me (generations[ii].brothers[ll]);
      if (me.cables.size() > 1 && me.numSon() == 0 && me.numBrother() == 0){
	// printf ("build circle at node: %d\n", me.identity);
	for (unsigned jj = 0; jj < me.cables.size(); ++jj){
	  for (unsigned kk = jj+1; kk < me.cables.size(); ++kk){
	    Circles tmp = buildCircleCable (me.cables[jj], me.cables[kk]);
	    cir.add (tmp);
	  }
	}
      }
    }
  }

  // build from brothers
  for (unsigned ii = 1; ii < generations.size(); ++ii){
    for (unsigned jj = 0; jj < generations[ii].brothers.size(); ++jj){
      const TreeNode & node0 (generations[ii].brothers[jj]);
      for (unsigned kk = 0; kk < node0.numBrother(); ++kk){
	if (node0.vecBrother[kk].broId > jj){
	  const TreeNode & node1 (getTreeNode(node0.vecBrother[kk]));
	  // printf ("build circle at node: %d %d\n", node0.identity, node1.identity);
	  for (unsigned aa = 0; aa < node0.cables.size(); ++aa){
	    for (unsigned bb = 0; bb < node1.cables.size(); ++bb){
	      Circles tmp = buildCircleCable (node0.cables[aa], node1.cables[bb]);
	      cir.add (tmp);
	    }
	  }
	}
      }
    }
  }   
}

	
