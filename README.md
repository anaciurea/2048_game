CIUREA ANA-SORINA
311 CC 

README:

The main idea of the project was to learn how to create the 2048 game using ncurses. I worked around 30 hours and learning how to use this library was probably the most difficult part. I did all the tasks that were mandatory.
-I created different windows to implement the tasks that were asked ( for example a window for the 'initial_menu' or for 'data_menu');
-I initialise the game board with 0 in function 'init_board' and in function 'count_empty_cells' I counted the cells where I can generate a random number;
-Using 'start colour', I coloured the menu when going through it with the 'KEY_UP' or 'KEY-DOWN';
-Using the functions 'move_up', 'move_down', 'move_right', 'move_left' we will make the moves in the game using the keys : 'w' - up, 's' - down, 'd' - right, 'a' left;
-If the player doesn't make any move for 5 seconds, it will automatically do a move (the best one is the one who frees the most cells). In case there are more moves which free the same number of cells, there is going to move up. If that's not possible, there is going to waited a move from the player;
-EXTRA: When there are no possible moves and the board is full with numbers, we will quit the game, but it's going to be printed the message "Nu mai sunt mutari posibile", the points and the final board. If we won, it's going to print the message "Felicitari! Ai castigat!", the points and the final board.
