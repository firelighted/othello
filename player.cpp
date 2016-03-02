#include "player.h"

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
    srand(time(NULL));
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
  int debug = 1;
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
  // update own board
  this->myBoard.doMove(this->validMove, this->mySide);
  if (debug)
    {
      std::cerr << "Playing move:" << this->validMove->getX() << "," 
		<< this->validMove->getY() << std::endl;
    }
 
  return this->validMove;
}
