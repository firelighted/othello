#include "player.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <pthread.h>
#include <iostream>
#include <cstdio>
#include <ctime>

#define mindebug 0// minimum debugging output (time, method, depth, score, win/lose)
#define debug    0// all other debugging output
// # of stones are on the field when method changes
#define method_change_moves 32
#define DEPTH    8// iterative depth
// 0=random; 1=weighted; 2=minimax w/alpha-beta (mobility then stone count); 
// 3=mobility (switch to minimaxPlayer stone count); 4=minimaxPlayer;
// 5=alternating minimax, mobility;
#define METHOD   2
/*
 * Constructor for the player; initialize everything here. 
 * The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
  // Will be set to true in test_minimax.cpp, will take effect in doMove()
  testingMinimax = false;
  // 0=random; 1=weighted; 2=minimax with alpha-beta;
  // 3=mobility (switch to minimaxPlayer at end); 
  // 4=minimaxPlayer; 5=alternating
  this->method = METHOD;
  this->myBoard = Board();
  this->mySide = side;
  if (side == BLACK)
    { this->otherSide = WHITE; }
  else 
    { this->otherSide = BLACK; }
  // set up for chosen method
  if (method == 0)
    {// RANDOM METHOD
      srand(time(NULL));
    }
  else if (method == 1)
    {// WEIGHT METHOD
      if (debug) { std::cerr << "Setting up weights. "; }
      int arr0[8] =  {100, -10, 11, 6, 6, 11, -10, 100};
      int arr1[8]  = {-10, -20, 1,  2, 2, 1,  -20, -10};
      int arr2[8]  = {10,    1, 5,  4, 4, 5,    1,  10};
      int arr3[8]  = {6,     2, 4,  2, 2, 4,    2,   6};
      std::vector<int> vec0(arr0, arr0 + 7);
      std::vector<int> vec1(arr1, arr1 + 7);
      std::vector<int> vec2(arr2, arr2 + 7);
      std::vector<int> vec3(arr3, arr3 + 7);
      this->weights.push_back(vec0);
      this->weights.push_back(vec1);
      this->weights.push_back(vec2);
      this->weights.push_back(vec3);
      this->weights.push_back(vec3);
      this->weights.push_back(vec2);
      this->weights.push_back(vec1);
      this->weights.push_back(vec0);
    }
  else
    { 
      this->depth = DEPTH;
      this->switcher = 0;
    }
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 * 
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
  std::clock_t start;
  double duration;
  start = std::clock();
  int guessScore = 0;

  if (testingMinimax)
    { 
      this->depth = 2;
      char boardData[64] = {
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
	' ', 'b', ' ', ' ', ' ', ' ', ' ', ' ', 
	'b', 'w', 'b', 'b', 'b', 'b', ' ', ' ', 
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
      };
      this->myBoard.setBoard(boardData);
      this->method = 2; // set test parameters
      this->depth = 2;
    }
  // update own board with opponent's move
  this->myBoard.doMove(opponentsMove, this->otherSide);
  if (debug)
    {
      std::cerr << "  Called doMove. " << std::endl;
      if (opponentsMove != NULL)
	{
	  std::cerr << "Opponent's move:" << opponentsMove->getX() << "," 
		    << opponentsMove->getY() << std::endl;
	}
    }
  // check whether there are valid moves
  if (!this->myBoard.hasMoves(this->mySide))
    {
      if (debug)
	{ std::cerr << "Playing NULL" <<  std::endl;
	  if (this->myBoard.count(this->mySide) 
		  < this->myBoard.count(this->otherSide))
		{ std::cerr << "LOSING." << std::endl; }
	      else { std::cerr << "WINNING." << std::endl; }
	}
      return NULL;
    }
  this->validMove = new Move(0, 0);

  // change method from mobility if space constraint met
  if (this->myBoard.countBlack() + this->myBoard.countWhite() > method_change_moves )
    { this->switcher = 1; }// make endgame switch
    
  // choose a move based on the specified method
  if (this->method == 0)
    {// RANDOM METHOD
      randomPlayer();
    }
  else if (this->method == 1)
    {// WEIGHT METHOD
      weightPlayer();
    }
  else if (this->method == 2)
    {// MINIMAX METHOD
      guessScore = minimax(&(this->myBoard), this->depth, -1000, 1000, true);
    }
  else if (this->method == 3)
    { // MOBILITY METHOD
      guessScore = mobilePlayer(&(this->myBoard), this->depth, true);
    }
  else if (this->method == 4)
    { // MINIMAX PLAYER METHOD
      this->myBoard.depthRemain = this->depth;
      this->myBoard.max_min = true;
      minimaxPlayer(&(this->myBoard));
    }
  else if (this->method == 5)
    { // SWITCHER METHOD
      if (this->switcher == 0)
	{// MINIMAX PLAYER METHOD
	  this->myBoard.depthRemain = this->depth;
	  this->myBoard.max_min = true;
	  minimaxPlayer(&(this->myBoard));
	  this->switcher = 1;
	}
      else // this->switcher = 1
	{// MOBILITY METHOD
	  guessScore = mobilePlayer(&(this->myBoard), this->depth, true);
	  std::cerr << guessScore << std::endl;
	  this->switcher = 0;
	}

    }

  // Update own board with chosen move
  this->myBoard.doMove(this->validMove, this->mySide);
  if (mindebug)
    {
      std::cerr << "Playing move:" << this->validMove->getX() << "," 
		<< this->validMove->getY() << std::endl; 
      duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
      if (this->myBoard.count(this->mySide) 
	  < this->myBoard.count(this->otherSide))
	{ std::cerr << "LOSING." << std::endl; }
      else { std::cerr << "WINNING." << std::endl; }
      std::cerr << "Time=" << duration << " for method=" << this->method 
		<< " for depth=" << this->depth << " with guessScore=" 
		<< guessScore << " for side=" << this->mySide << std::endl;
    }

  return this->validMove;
}


////////////////////////////////////////////////////////////////////////////////


/**
 * @brief mobilePlayer finds a validMove based on how the maximum of the minimum
 * moves available at depth.
 */
int Player::mobilePlayer(Board * leafBoard, int depth, bool max_min)
{
  if (debug) { std::cerr << "Calling mobilePlayer(depth=" << depth << " , maxmin=" 
			 << max_min << ") \n"; }
  if ((depth == 0)||(leafBoard->isDone()))
    {
      std::vector <Move *> moves = vecMoves(leafBoard, this->mySide);
      int endvalue = moves.size();
      if (debug) { std::cerr << "mobilePlayer returns " << endvalue
			     << std::endl;}
      return endvalue; // heuristic value of node
    }
  int v;

  if (max_min) // maximizing player
    {
      int bestValue = -1000;
      std::vector <Move *> moves = vecMoves(leafBoard, this->mySide);
      if (debug) { std::cerr << "Found player moves. " << std::endl;}
      if (moves.empty())// no moves available. 
	//Go to further depth w/o modifying.
	{ return mobilePlayer(leafBoard, depth - 1, false); }
      for (int i = 0; i < (signed int) moves.size(); i++)
	{
	  Board * childBoard = leafBoard->copy();  // allocates board for recursion
	  childBoard->doMove(moves[i], this->mySide); // modify childBoard with move
	  if (debug) 
	    {
	      std::cerr << "Try player move: " << moves[i]->getX() 
			<< "," << moves[i]->getY() << std::endl;
	    }
	  v = mobilePlayer(childBoard, depth - 1, false);
	  if (v > bestValue)
	    { 
	      bestValue = v;
	      if (depth == this->depth) 
		{ // adjusts validMove only during initial minimax call for player
		  this->validMove->setX(moves[i]->getX());
		  this->validMove->setY(moves[i]->getY());
		  if (debug) { std::cerr << "Setting validMove: " 
					 << this->validMove->getX() << "," 
					 << this->validMove->getY() << std::endl;}
		}
	    }
	}
      if (debug) { std::cerr << "mobilePlayer returns "<< bestValue << std::endl; }
      for (int i = 0; i < (signed int) moves.size(); i++)  { delete moves[i]; }
      return bestValue;
    }
  else // minimizing player
    {
      int bestValue = 1000;
      std::vector <Move *> moves = vecMoves(leafBoard, this->otherSide);
      if (debug) { std::cerr << "Found opponent moves. " << std::endl;}
      if (moves.size() == 0)// no moves available. Go to further depth w/o modifying.
	{ return mobilePlayer(leafBoard, depth - 1, true); }
      for (int i = 0; i < (signed int) moves.size(); i++)
	{
	  Board * childBoard = leafBoard->copy(); // allocates board
	  childBoard->doMove(moves[i], this->otherSide);
	  if (debug) 
	    {
	      std::cerr << "Try opponent move: " << moves[i]->getX() 
			<< "," << moves[i]->getY() << std::endl;
	    }
	  v = mobilePlayer(childBoard, depth - 1, true);
	  bestValue = std::min(bestValue, v);
	}
      if (debug) { std::cerr << "mobilePlayer returns "<< bestValue << std::endl; }
      for (int i = 0; i < (signed int) moves.size(); i++)
	{
	  delete moves[i];
	}
      return bestValue;
    }
}

/**
 * @brief minimax uses alpha-beta pruning of the minimax algorithm to store
 * a strategic move in validMove.
 */
int Player::minimax(Board * leafBoard, int depth, int a, int b, bool max_min)
{
  if (debug) { std::cerr << "Calling minimax(depth=" << depth << " , maxmin=" 
			 << max_min << ") \n"; }
  if ((depth <= 0)||(leafBoard->isDone())) // returning heuristic value of this node
    {
      if (debug) { std::cerr << "minimax returns " << leafBoard->count(this->mySide) 
			     << std::endl; }
      if (this->switcher)
	{ return leafBoard->count(this->mySide); }// in endgame, use stone count
      else 
	{ return vecMoves(leafBoard, this->mySide).size(); } // early in game, use mobility
    }
  int value;
  if (max_min) // maximizing player
    {
      int v = -1000;
      std::vector <Move *> moves = vecMoves(leafBoard, this->mySide);
      if (debug) { std::cerr << "Found player moves. " << std::endl;}
      if (moves.empty()) // no moves available.
	{ return minimax(leafBoard, depth - 1, a, b, false); }
      for (unsigned int i = 0; i < moves.size(); i++)
	{
	  Board * childBoard = leafBoard->copy();     // allocates board for recursion
	  childBoard->doMove(moves[i], this->mySide); // modify childBoard with move
	  if (debug) { std::cerr << "Try player move: " << moves[i]->getX() 
				 << "," << moves[i]->getY() << std::endl; }
	  value = minimax(childBoard, depth - 1, a, b, false);
	  if (value > v)
	    { 
	      v = value;
	      if (depth == this->depth) 
		{ // adjusts validMove only during initial minimax call for player
		  this->validMove->setX(moves[i]->getX());
		  this->validMove->setY(moves[i]->getY());
		  if (debug) { std::cerr << "Setting validMove: " 
					 << this->validMove->getX() << "," 
					 << this->validMove->getY() << std::endl;}
		}
	      a = std::max(a, v);
	      if (b <= a)
		{ break; } // b cut off
	    }
	}
      if (debug) { std::cerr << "minimax returns " << v << std::endl; }
      for (int i = 0; i < (signed int) moves.size(); i++)
	{ delete moves[i]; }// clean up moves
      return v;
    }
  else // minimizing player
    {
      int v = 1000;
      std::vector <Move *> moves = vecMoves(leafBoard, this->otherSide);
      if (debug) { std::cerr << "Found opponent moves. " << std::endl;}
      if (moves.size() == 0)// no moves available. 
	{ return minimax(leafBoard, depth - 1, a, b, true); }
      for (int i = 0; i < (signed int) moves.size(); i++)
	{
	  Board * childBoard = leafBoard->copy(); // allocates board
	  childBoard->doMove(moves[i], this->otherSide);
	  if (debug) 
	    {
	      std::cerr << "Try opponent move: " << moves[i]->getX() 
			<< "," << moves[i]->getY() << std::endl;
	    }
	  v = std::min(v, minimax(childBoard, depth - 1, a, b, true));
	  b = std::min(b, v);
	  if (b <= a)
	    { break; } //a cut off
	}
      if (debug) { std::cerr << "minimax returns "<< v << std::endl; }
      for (int i = 0; i < (signed int) moves.size(); i++)
	{ delete moves[i]; }// clean up moves
      return v;
    }
} // end minimax

/**
 * @brief minimaxPlayer determines a strategic move to store 
 * in player->validMove.
 */
void Player::minimaxPlayer(Board * leafBoard)
{
  if (debug) { std::cerr << "Calling minimaxPlayer(depth=" << leafBoard->depthRemain 
			 << " , maxmin=" << leafBoard->max_min << ") \n"; }
  if ((leafBoard->depthRemain <= 0)||(leafBoard->isDone()))
    {
      if (debug) { std::cerr << "minimaxPlayer returns " 
			     << leafBoard->count(this->mySide) 
			     << std::endl;}
      leafBoard->result = leafBoard->count(this->mySide); // heuristic value = # pieces
      return;
    }
  int v;
  if (leafBoard->max_min) // maximizing player
    {
      int bestValue = -100;
      std::vector <Move *> moves = vecMoves(leafBoard, this->mySide);
      if (debug) { std::cerr << "Found player moves. " << std::endl;}
      if (moves.size() == 0)// no moves available. note: reuse, don't free leafboard
	{ 
	  leafBoard->depthRemain = leafBoard->depthRemain - 1; // change remaining board depth
	  leafBoard->max_min = false;        // change max_min
	  minimaxPlayer(leafBoard);          // will modify the same board as this call
	  return; 
	}
      for (int i = 0; i < (signed int) moves.size(); i++)
	{
	  Board * childBoard = leafBoard->copy();     // duplicate current board to use as base
	  childBoard->doMove(moves[i], this->mySide); // modify duplicate with move
	  if (debug)  {std::cerr << "Try player move: " << moves[i]->getX() 
				 << "," << moves[i]->getY() << std::endl; }
	  childBoard->depthRemain = leafBoard->depthRemain - 1;
	  childBoard->max_min = false;
	  minimaxPlayer(childBoard);
	  v = childBoard->result;
	  if (v > bestValue)
	    { 
	      bestValue = v;
	      if (leafBoard->depthRemain == this->depth) 
	      	{ // adjusts validMove only during initial minimax call for player in method 3
	      	  this->validMove->setX(moves[i]->getX());
	      	  this->validMove->setY(moves[i]->getY());
	      	  if (debug) { std::cerr << "Setting validMove: "
					 << this->validMove->getX() 
					 << ","  << this->validMove->getY() 
					 << std::endl;}
		}
	    }
	}
      if (debug) { std::cerr << "minimax returns "<< bestValue << std::endl; }
      for (int i = 0; i < (signed int) moves.size(); i++)
	{ delete moves[i]; }// clean up moves
      leafBoard->result = bestValue;
      return;
    }
  else 
    {// minimizing player
      int bestValue = 100;
      std::vector <Move *> moves = vecMoves(leafBoard, this->otherSide);
      if (debug) { std::cerr << "Found opponent moves. " << std::endl;}
      if (moves.size() == 0)// no moves available. 
	{ 
	  leafBoard->depthRemain = leafBoard->depthRemain - 1;
	  // change remaining board depth
	  leafBoard->max_min = true;   // change max_min
	  minimaxPlayer(leafBoard); 
	  return;
	}
      for (int i = 0; i < (signed int) moves.size(); i++)
	{
	  Board * childBoard = leafBoard->copy(); // allocates board
	  childBoard->doMove(moves[i], this->otherSide);
	  if (debug) { std::cerr << "Try opponent move: " << moves[i]->getX() 
				 << "," << moves[i]->getY() << std::endl; }
	  childBoard->depthRemain = leafBoard->depthRemain - 1;
	  childBoard->max_min = true;
	  minimaxPlayer(childBoard);
	  bestValue = std::min(bestValue, childBoard->result);
	}
      if (debug) { std::cerr << "minimax returns "<< bestValue << std::endl; }
      for (int i = 0; i < (signed int) moves.size(); i++)
	{ delete moves[i]; }// clean up moves
      leafBoard->result = bestValue;
      return;
    }

} //end minimaxPlayer

/**
 * @brief vecMoves searches through all squares of a board to return an array
 * of pointers to all valid moves for the specified side. Used by minimax.
 */
std::vector<Move *> Player::vecMoves(Board * leafBoard, Side side)
{
  std::vector <Move *> vec;
  Move * move;
  int a, b;
  for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
	{
	  a = i; // Desired order of row/col checking:
	  b = j; // {0, 7, 2, 3, 4, 5, 6, 1}
	  if (a == 1)      { a = 7; }
	  else if (a == 7) { a = 1; }
	  if (b == 1)      { b = 7; }
	  else if (b == 7) { b = 1; }
	  move = new Move(a, b);
	  if (leafBoard->checkMove(move, side))
	    { vec.push_back(move); }
	  else
	    { delete move; }// free if not used later
	}
    }
  return vec;
}
/**
 * @brief randomPlayer() modifies player->validMove with a 
 * randomly chosen valid move.
 */
void Player::randomPlayer()
{
  while(!this->myBoard.checkMove(this->validMove, this->mySide)) 
    // loop to find valid move
    {
      this->validMove->setX(rand() % 8);
      this->validMove->setY(rand() % 8);
      if (debug)
	{
	  std::cerr << "  Testing move:" << this->validMove->getX() << "," 
		    << this->validMove->getY() << std::endl;
	}
    }
}

/**
 * @brief weightPlayer() modifies player->validMove with 
 * the valid move with the best
 * weight, according to a table of constant weights.
 */
void Player::weightPlayer()
{
  if (debug) { std::cerr << "Max weight calculation. "; }
  double max = -100;
  int maxX, maxY;
  for (int i = 8; i > -1; i--) // X
    {
      for (int j = 0; j < 8; j++) // Y
	{
	  Move tempMove = Move(i, j);
	  if ( (this->myBoard.checkMove(&tempMove, this->mySide))
	       &&(this->weights[i][j] > max) )
	    { 
	      max = this->weights[i][j];
	      maxX = i;
	      maxY = j;
	    }
	}
    }
  this->validMove->setX(maxX);
  this->validMove->setY(maxY);
  if (debug)
    { 
      std::cerr << "Max weight found: " << max << " " 
		<< this->myBoard.checkMove(this->validMove, this->mySide)
		<< std::endl;
    }
}
