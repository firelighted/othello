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
    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
    this->myBoard = Board();
    this->mySide = side;
    if (side == BLACK)
      { this->otherSide = WHITE; }
    else 
      { this->otherSide = BLACK; }
    // RANDOM METHOD
    srand(time(NULL));
    // WEIGHT METHOD
    if (debug) { std::cerr << "Setting up weights. "; }
    int arr0[] =  {100, -10, 11, 6, 6, 11, -10, 100};
    int arr1[]  = {-10, -20, 1,  2, 2, 1,  -20, -10};
    int arr2[]  = {10,    1, 5,  4, 4, 5,    1,  10};
    int arr3[]  = {6,     2, 4,  2, 2, 4,    2,   6};
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
    if (debug) { 
      for (int i = 0; i < 8; i++)
    	{ std::cerr << this->weights[i][i] << std::endl; }
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
  /* 
   * TODO: Implement how moves your AI should play here. You should first
   * process the opponent's opponents move before calculating your own move
   */
  // update own board with opponent's move
  this->myBoard.doMove(opponentsMove, this->otherSide);
  if (debug)
    {
      std::cerr << "  Called doMove. " << std::endl;
      if (opponentsMove != NULL)
	{
	  std::cerr << "Opponent's move:" << opponentsMove->getX() << "," 
		    << opponentsMove->getY() << " " << opponentsMove << std::endl;
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

  // WEIGHT METHOD
  if (debug) { std::cerr << "Max weight calculation. "; }
  double max = -100;
  int maxX, maxY;
  for (int i = 0; i < 8; i++) // X
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
  if (debug)
    { 
      std::cerr << "Max weight found: " << max << " " 
		<< this->myBoard.checkMove(this->validMove, this->mySide)
		<< std::endl;
    }
  this->validMove->setX(maxX);
  this->validMove->setY(maxY);
  // end WEIGHT METHOD

  // update own board
  this->myBoard.doMove(this->validMove, this->mySide);
  if (debug)
    {
      std::cerr << "Playing move:" << this->validMove->getX() << "," 
		<< this->validMove->getY() << std::endl;
    }
 
  return this->validMove;
}
