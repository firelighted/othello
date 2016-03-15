Othello README
Sarah Asano (1-woman team)

AI Improvements:
-Debugging improvements: Global variables debug and mindebug were
added to control the degree of debugging output. Global variables for
depth and method choice were also added to make changing parameters
simple. Another variable controls the definition of the endgame.

-Weighted move choice implemented: I built a simple heuristic that chose
an available move with the highest weight, according to a 2D vector of 
constant integer weights.(weightPlayer, METHOD 1)

- Minimax implemented: Especially for opponents with a similar heuristic,
minimax should help control the score. This method is also useful as a 
 starting place to ensure my code of a recursive heuristics worked
 before making the AI more complex. The depth was set low to avoid
memory errors. (minimax, METHOD 2)

-Mobility move-choice implemented: Since mobility measurements are
very frequently used in the papers I skimmed about Othello heuristics,
I implemented this method to test it. It is also fairly simple, since it uses
the same structure as minimax, but simply evaluates the final boards 
differently. (mobilePlayer, METHOD 4)

-Multithreading attempted: Multithreading seems like it should make 
the algorithms run much faster, and the tree structure of possible boards
would work well with it. I tried to implement a thread for each of the 
initial possible moves on the current board, but there were too many 
errors I didn't understand very well, and the Stack Overflow answers 
(such as using pthread instead of std::thread, adding -pthread or 
-lpthread to the compiler,  etc.) didn't work for me. After many hours,
I gave up. I added another version of minimax (minimaxPlayer) that 
returns void in order to have a function to call with the thread constructor.
minimaxPlayer uses stone counting throughout the game, so it was useful
for comparing mobility and stone counting heuristic success. 
(minimaxPlayer, METHOD 3)

-Alpha-beta pruning implemented: Alpha-beta pruning made the minimax
process much faster. It also fixed many of the memory errors I had
been running into with minimax. Since alpha-beta pruning is typically
described as a node tree, I began implementing a node structure 
with my own node classes. I deleted that since it was overcomplicating
the issue. (minimax, METHOD 2)

-Switching method implemented: A method that alternated between 
mobility and a stone-count heuristics was implemented out of curiosity
to see if this compromise between heuristics was useful. It wasn't 
particularly good at games, though it is functional. (METHOD 5)

-Move ordering implemented: To improve alpha-beta pruning further,
basic move-ordering was added to vecMoves. Row and columns are 
now searched in the order: {0, 7, 2, 3, 4, 5, 6, 1}, since edges are the most
advantageous, the squares touching edges are the least, and the central
squares are comparatively neutral. vecMoves generates a vector of 
possible moves for a given board and side, so all methods use it.
 (vecMoves)

This AI will be successful because it uses a mobility heuristic for most of
the game, but then switches to stone counting at the endgame. The 
definition of endgame has been tweaked to make it effective against
BetterPlayer and ConstantTimePlayer. Although it can only go to a depth
of 8 without constant memory errors, it should work well for most games.
