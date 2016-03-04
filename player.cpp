#include "player.h"
int debug = 1;
/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    this->myBoard = Board();
    this->mySide = side;
    if (side == BLACK)
      { this->otherSide = WHITE; }
    else 
      { this->otherSide = BLACK; }
    // // RANDOM METHOD
    // srand(time(NULL));
    // // WEIGHT METHOD
    // if (debug) { std::cerr << "Setting up weights. "; }
    // int arr0[] =  {100, -10, 11, 6, 6, 11, -10, 100};
    // int arr1[]  = {-10, -20, 1,  2, 2, 1,  -20, -10};
    // int arr2[]  = {10,    1, 5,  4, 4, 5,    1,  10};
    // int arr3[]  = {6,     2, 4,  2, 2, 4,    2,   6};
    // std::vector<int> vec0(arr0, arr0 + 7);
    // std::vector<int> vec1(arr1, arr1 + 7);
    // std::vector<int> vec2(arr2, arr2 + 7);
    // std::vector<int> vec3(arr3, arr3 + 7);
    // this->weights.push_back(vec0);
    // this->weights.push_back(vec1);
    // this->weights.push_back(vec2);
    // this->weights.push_back(vec3);
    // this->weights.push_back(vec3);
    // this->weights.push_back(vec2);
    // this->weights.push_back(vec1);
    // this->weights.push_back(vec0);
    // if (debug) { 
    //   for (int i = 0; i < 8; i++)
    // 	{ std::cerr << this->weights[i][i] << std::endl; }
    // }

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
    // MINIMAX METHOD
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
      }
    else 
      { this->depth = 2; }
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
  	{ std::cerr << "Playing NULL" <<  std::endl; }
      return NULL;
    }
  this->validMove = new Move(0, 0);
  // // RANDOM METHOD
  // while(!this->myBoard.checkMove(this->validMove, this->mySide)) 
  //   // loop to find valid move
  //   {
  //     this->validMove->setX(rand() % 8);
  //     this->validMove->setY(rand() % 8);
  //     if (debug)
  // 	{
  // 	  std::cerr << "  Testing move:" << this->validMove->getX() << "," 
  // 	  	    << this->validMove->getY() << std::endl;
  // 	}
  //   }
  //  // end RANDOM METHOD

  // // WEIGHT METHOD
  // if (debug) { std::cerr << "Max weight calculation. "; }
  // double max = -100;
  // int maxX, maxY;
  // for (int i = 0; i < 8; i++) // X
  //   {
  //     for (int j = 0; j < 8; j++) // Y
  // 	{
  // 	  Move tempMove = Move(i, j);
  // 	  if ( (this->myBoard.checkMove(&tempMove, this->mySide))
  // 	       &&(this->weights[i][j] > max) )
  // 	    { 
  // 	      max = this->weights[i][j];
  // 	      maxX = i;
  // 	      maxY = j;
  // 	    }
  // 	}
  //   }
  // this->validMove->setX(maxX);
  // this->validMove->setY(maxY);
  // if (debug)
  //   { 
  //     std::cerr << "Max weight found: " << max << " " 
  // 		<< this->myBoard.checkMove(this->validMove, this->mySide)
  // 		<< std::endl;
  //   }
  // // end WEIGHT METHOD

  // MINIMAX METHOD
  int guessScore = minimax(&(this->myBoard), this->depth, true);
  if (debug) { std::cerr << "Estimated score for depth=" << this->depth << " : " 
			 << guessScore<< std::endl;}
  //end MINIMAX METHOD

  // update own board
  this->myBoard.doMove(this->validMove, this->mySide);
  if (debug) { std::cerr << "Playing move:" << this->validMove->getX() << "," 
	       << this->validMove->getY() << std::endl; }
 
  return this->validMove;
}

/*
 * @brief minimax recursively determines a strategic move to store in player->validMove.
 */
int Player::minimax(Board * leafBoard, int depth, bool max_min)
{
  if (debug) { std::cerr << "Calling minimax(depth=" << depth << " , maxmin=" 
			 << max_min << " , board=" << leafBoard << ") \n"; }
  if ((depth == 0)||(leafBoard->isDone()))
    { // note: don't want to free this leafboard because it's a player member
      if (debug) { std::cerr << "minimax returns " << leafBoard->count(this->mySide) 
			     << std::endl;}
      return leafBoard->count(this->mySide); // heuristic value of node
    }
    int v;
    if (max_min) // maximizing player
      {
	int bestValue = -100;
	std::vector <Move *> moves = vecMoves(leafBoard, this->mySide);
	if (debug) { std::cerr << "Found player moves. " << std::endl;}
	if (moves.size() == 0)// no moves available. note: reuse, don't free leafboard
	  { return minimax(leafBoard, depth - 1, false); }
	for (int i = 0; i < (signed int) moves.size(); i++)
	  {
	    Board * childBoard = leafBoard->copy();  // allocates board for recursion
	    childBoard->doMove(moves[i], this->mySide); // modify childBoard with move
	    if (debug) 
	      {
		std::cerr << "Try player move: " << moves[i]->getX() 
			  << "," << moves[i]->getY() << std::endl;
	      }
	    v = minimax(childBoard, depth - 1, false);
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
	if (debug) { std::cerr << "minimax returns "<< bestValue << std::endl;}
	return bestValue;
      }
    else // minimizing player
      {
	int bestValue = 100;
	std::vector <Move *> moves = vecMoves(leafBoard, this->otherSide);
	if (debug) { std::cerr << "Found opponent moves. " << std::endl;}
	if (moves.size() == 0)// no moves available. note: reuse, don't free leafboard
	  { return minimax(leafBoard, depth - 1, true); }
	for (int i = 0; i < (signed int) moves.size(); i++)
	  {
	    Board * childBoard = leafBoard->copy(); // allocates board
	    childBoard->doMove(moves[i], this->otherSide);
	    if (debug) 
	      {
		std::cerr << "Try opponent move: " << moves[i]->getX() 
			  << "," << moves[i]->getY() << std::endl;
	      }
	    v = minimax(childBoard, depth - 1, true);
	    bestValue = std::min(bestValue, v);
	   }
	if (debug) { std::cerr << "minimax returns "<< bestValue << std::endl; }
	return bestValue;
      }
}
/**
 * @brief vecMoves searches through all squares of a board to return an array
 * of pointers to all valid moves for the specified side.
 *
 */
std::vector<Move *> Player::vecMoves(Board * leafBoard, Side side)
{
  std::vector <Move *> vec; 
  Move * move;
  for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
	{
	  move = new Move(i, j); // allocate for each possible square
	  if (leafBoard->checkMove(move, side))
	    {
	      vec.push_back(move);
	      if (debug) { std::cerr<<"("<< move->getX() <<"," << move->getY() <<") "; }
	    }
	  else
	    {
	      if (debug) { std::cerr << "deleting...("<< move->getX() << ","
				     << move->getY() <<") "; }
	      delete move;      // free if not used later
	      if (debug) { std::cerr << "X."; }
	    }
	}
    }
  if (debug) { std::cerr << "Finished vecMoves." << std::endl; }
  return vec;
}
