#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
#include <time.h>
#include <stdlib.h>
#include <vector>
using namespace std;

class Player {
 private:
  Board myBoard;
  Move *validMove;
  Side mySide;
  Side otherSide;
  std::vector < std::vector <int> > weights;
  int depth;
 public:
  Player(Side side);
  ~Player();
    
  Move *doMove(Move *opponentsMove, int msLeft);
  int minimax(Board * leafBoard, int depth, bool max_min);
  std::vector<Move *> vecMoves(Board * leafBoard, Side side);

  // Flag to tell if the player is running within the test_minimax context
  bool testingMinimax;
};

#endif
