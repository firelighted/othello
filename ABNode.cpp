#include <ABNode.hpp>
#include "player.h"


ABNode::ABNode( Board * aBoard,  Move * aMove, Side aSide);
{
  board = aBoard;
  prevMove = aMove;
  side = aSide;
}

ABNode::~ABNode()
{
  if (subnodes.size() != 0)
				{
						for (std::vector <ABNode *>::iterator it = this->subnodes.begin(); 
											it != this->subnodes.end(); ++it)
								{
										if (*it)
												{ delete *it; }
								}
						delete [] subnodes;
				}
}
