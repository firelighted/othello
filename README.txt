Othello README
Sarah Asano ( Solo team)

AI Improvements:
-Weighted move choice implemented: I built a simple heuristic that chose
an available move with the highest weight, according to a 2D vector of 
constant integer weights.

- Minimax implemented: Especially for opponents with a similar heuristic,
minimax should help control the score. This method is also useful as a 
 starting place to ensure my code of a recursive heuristics worked
 before making the AI more complex.

-Multithreading attempted: Multithreading seems like it should make 
the algorithms run much faster, and the tree structure of possible boards
would work well with it. I tried to implement a thread for each of the 
initial possible moves on the current board, but there were too many 
errors I didn't understand very well, and the Stack Overflow answers 
(such as using pthread instead of std::thread, adding -pthread or 
-lpthread to the compiler,  etc.) didn't work for me. After many hours,
I gave up.

-Alpha-beta pruning implemented: 