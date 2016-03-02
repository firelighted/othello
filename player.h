#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
#include <time.h>
#include <stdlib.h>
using namespace std;

class Player {
 private:
  Board myBoard;
  Move *validMove;
  Side mySide;
  Side otherSide;
 public:
  Player(Side side);
  ~Player();
    
  Move *doMove(Move *opponentsMove, int msLeft);

  // Flag to tell if the player is running within the test_minimax context
  bool testingMinimax;
};

#endif
