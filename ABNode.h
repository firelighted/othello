#ifndef __ABNODE_H__
#define __ABNODE_H__

#include <vector>
#include "player.h"
#include <stdlib.h>


class ABNode
{
public:
		ABNode( Board * aBoard,  Move * aMove, 
									 Side aSide);
		~ABNode();
		Move * prevMove;
		Side side;
		std::vector <ABNode *> subnodes;
		Board * board;
}


#endif
