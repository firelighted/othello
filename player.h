#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <array>
using namespace std;

class Player {
 private:
  Board myBoard;
  Move *validMove;
  Side mySide;
  Side otherSide;
  std::vector < std::vector <int> > weights;
  void randomPlayer();
  void weightPlayer();
  int minimax(Board * leafBoard, int depth, int a, int b, bool max_min);
  void minimaxPlayer(Board * leafBoard);
  int mobilePlayer(Board * leafBoard, int depth, bool max_min);
  std::vector<Move *> vecMoves(Board * leafBoard, Side side);
  Move * vecMovesHelper(Board * leafBoard, Side side, int i, int j);
  int depth, method, switcher;
 public:
  Player(Side side);
  ~Player();
    
  Move *doMove(Move *opponentsMove, int msLeft);
  // Flag to tell if the player is running within the test_minimax context
  bool testingMinimax;
};

#endif
