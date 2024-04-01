#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <time.h>

#define BEGIN 0
#define SIZE 4

/*This function is created to initialise the board with 0.
*/
void init_board(int board[SIZE][SIZE]) {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }
}

/*This function is being used in various places. As it can be seen in the name,
 it counts the number of cells where there is 0.
*/
int count_empty_cells(int board[SIZE][SIZE]) {
    int count = 0;
    int i, j;

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                count++;
            }
        }
    }

    return count;
}

/*We creat the menu with 3 options, as requested : 'New Game', 'Resume', 'Quit'.
Here, we also 'highlight' the option from the menu, where we are. 
But, when we enter for the firs time the game the only 2 options that 
are displayed on the window are 'New Game' and 'Quit', because it wouldn't
have any logic to continue the game.
*/
void print_initial_menu(WINDOW *initial_menu, int menu_height, int menu_width, 
                        int board[SIZE][SIZE], int highlight_position) {
    char new_game[] = "New Game";
    char resume[] = "Resume";
    char quit[] = "Quit";
    
    box(initial_menu, BEGIN, BEGIN);

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);

    if (highlight_position == 0) {
        wattron(initial_menu, COLOR_PAIR(1));
        wmove(initial_menu, menu_height / 2 - 2, menu_width / 2 - strlen(new_game) / 2);
        wprintw(initial_menu, "%s", new_game);
        wattroff(initial_menu, COLOR_PAIR(1));

        if (count_empty_cells(board) == (SIZE * SIZE)) {
            wmove(initial_menu, menu_height / 2, menu_width / 2 - strlen(quit) / 2);
            wprintw(initial_menu, "%s", quit);
        } else {
            wmove(initial_menu, menu_height / 2, menu_width / 2 - strlen(resume) / 2);
            wprintw(initial_menu, "%s", resume);

            wmove(initial_menu, menu_height / 2 + 2, menu_width / 2 - strlen(quit) / 2);
            wprintw(initial_menu, "%s", quit);
        }
    } else if (highlight_position == 1) {
        wmove(initial_menu, menu_height / 2 - 2, menu_width / 2 - strlen(new_game) / 2);
        wprintw(initial_menu, "%s", new_game);

        if (count_empty_cells(board) == (SIZE * SIZE)) {
            wmove(initial_menu, menu_height / 2, menu_width / 2 - strlen(quit) / 2);
            wprintw(initial_menu, "%s", quit);
        } else {
            wattron(initial_menu, COLOR_PAIR(1));
            wmove(initial_menu, menu_height / 2, menu_width / 2 - strlen(resume) / 2);
            wprintw(initial_menu, "%s", resume);
            wattroff(initial_menu, COLOR_PAIR(1));

            wmove(initial_menu, menu_height / 2 + 2, menu_width / 2 - strlen(quit) / 2);
            wprintw(initial_menu, "%s", quit);
        }
    } else {
        wmove(initial_menu, menu_height / 2 - 2, menu_width / 2 - strlen(new_game) / 2);
        wprintw(initial_menu, "%s", new_game);

        if (count_empty_cells(board) == (SIZE * SIZE)) {
            wattron(initial_menu, COLOR_PAIR(1));
            wmove(initial_menu, menu_height / 2, menu_width / 2 - strlen(quit) / 2);
            wprintw(initial_menu, "%s", quit);
            wattroff(initial_menu, COLOR_PAIR(1));
        } else {
            wmove(initial_menu, menu_height / 2, menu_width / 2 - strlen(resume) / 2);
            wprintw(initial_menu, "%s", resume);

            wattron(initial_menu, COLOR_PAIR(1));
            wmove(initial_menu, menu_height / 2 + 2, menu_width / 2 - strlen(quit) / 2);
            wprintw(initial_menu, "%s", quit);
            wattroff(initial_menu, COLOR_PAIR(1));
        }
    }

    wrefresh(initial_menu);
}


void determine_highlight_position(int *highlight_position, int board[SIZE][SIZE], 
                                  int *was_enter_pressed) {
    int command = getch();
    int empty_cells = count_empty_cells(board);
    
    if (command != ERR) {

        /*In case we want to move up in the menu, but we are at 'New Game', 
        we will remain there.
        */
        if (command == KEY_UP) {
            if (*highlight_position > 0) {
                (*highlight_position)--;

                if (empty_cells == (SIZE * SIZE) && (*highlight_position) == 1) {
                    (*highlight_position)--;
                }
            }

         /*In case we want to move down in the menu, but we are at 'Quit', 
        we will remain there.
        */
        } else if (command == KEY_DOWN) {
            if (*highlight_position < 2) {
                (*highlight_position)++;

                if (empty_cells == (SIZE * SIZE) && (*highlight_position) == 1) {
                    (*highlight_position)++;
                }
            }
        } else if (command == '\n') {
            *was_enter_pressed = 1;
        }
    }
}

/*In this function we draw the board.
*/
void print_board_menu(WINDOW *board_menu, int menu_height, int menu_width, int board[SIZE][SIZE]) {
    werase(board_menu);

    float height_portion = (float) menu_height / 4;
    float width_portion = (float) menu_width / 4;
    int i, j;

    box(board_menu, BEGIN, BEGIN);

    float initial_position = height_portion;

    for (i = 0; i < 3; i++) {
        wmove(board_menu, initial_position, BEGIN + 1);
        whline(board_menu, ACS_HLINE, menu_width - 2);

        initial_position += height_portion;
    }


    initial_position = width_portion;

    for (i = 0; i < 3; i++) {
        wmove(board_menu, BEGIN + 1, initial_position);
        wvline(board_menu, ACS_VLINE, menu_height - 2);

        initial_position += width_portion;
    }

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            wmove(board_menu, i * height_portion + height_portion / 2, j * width_portion + width_portion / 2 - 1);

            //In case on a cell we have 0, we will show a blank space.
            if (board[i][j] == 0) {
                wprintw(board_menu, "%s", " ");
            } else {
                wprintw(board_menu, "%d", board[i][j]);
            }
        }
    }

    wrefresh(board_menu);
}

/*This function is created to generate a random number : 2 or 4 on
a random position with the help of 'count_empty_cells'. 
*/
void random_number(int board[][SIZE], int nr_of_times) {
    int i;

    for (i = 0; i < nr_of_times; i++) {
        if (count_empty_cells(board) != 0) {
            int value = (rand() % 2 + 1) * 2;
    
            int random_position_row = rand() % 4; //There are only 4 posibilities for a row.
            int random_position_column = rand() % 4; //There are only 4 posibilities for a column.

            //If on a square we don't find 0, we generate random positions utill we find one.
            while (board[random_position_row][random_position_column] != 0) {
                random_position_row = rand() % 4;
                random_position_column = rand() % 4;
            }

            board[random_position_row][random_position_column] = value;
        }
    }
}

/*In this function we create the others additional menus that are requested in 
the task, such as 'data_menu', 'points_menu', 'legend_menu'.
*/
void print_additional_menus(WINDOW *data_menu, WINDOW *points_menu, WINDOW *legend_menu,
                            int max_height, int max_width, int punctaj_val) {
    box(data_menu, BEGIN, BEGIN);
    box(points_menu, BEGIN, BEGIN);
    box(legend_menu, BEGIN, BEGIN);

    char going_up[] = "w - going up";
    char going_down[] = "s - going down";
    char going_left[] = "a - going left";
    char going_right[] = "d - going right";
    char quit[] = "q - quit";

    wmove(legend_menu, max_height / 4 - 2, max_width / 8 - strlen(going_up) / 2);
    wprintw(legend_menu, "%s", going_up);
    wmove(legend_menu, max_height / 4 - 1, max_width / 8 - strlen(going_down) / 2);
    wprintw(legend_menu, "%s", going_down);
    wmove(legend_menu, max_height / 4, max_width / 8 - strlen(going_left) / 2);
    wprintw(legend_menu, "%s", going_left);
    wmove(legend_menu, max_height / 4 + 1, max_width / 8 - strlen(going_right) / 2);
    wprintw(legend_menu, "%s", going_right);
    wmove(legend_menu, max_height / 4 + 2, max_width / 8 - strlen(quit) / 2);
    wprintw(legend_menu, "%s", quit);

    time_t t;
    struct tm *area;
    t = time(NULL);
    area = localtime(&t);
    wmove(data_menu, max_height / 16 - 1, max_width / 8 - 5);
    wprintw(data_menu, "%d-%d-%d", area->tm_mday, area->tm_mon + 1, area->tm_year + 1900);
    wmove(data_menu, max_height / 16 + 1, max_width / 8 - 4);
    wprintw(data_menu, "%d:%d:%d", area->tm_hour, area->tm_min, area->tm_sec);

    char punctaj[] = "Punctaj: ";
    wmove(points_menu, max_height / 16, max_width / 8 - strlen(punctaj) / 2);
    wprintw(points_menu, "%s%d", punctaj, punctaj_val);

    wrefresh(data_menu);
    wrefresh(points_menu);
    wrefresh(legend_menu);
}

/*This function is created to be used when we want to move up in the game. It returns 1 
in case we didn't merge and shift, and 0 in case we did the actions.
*/
int move_up(int board[][SIZE], int *points) {
    int not_shift = 1;
    int not_merge = 1;

    int i, j, k, t;

    /*We check wether there is a free space on the i line and not on the j line.
    If yes, we move the j-k element on the i-k element.
    Where i and j are two consecutives lines.
    */
    for (t = 0; t < 3; t++) {
        for (i = 0, j = 1; i < SIZE - 1; i++, j++) {
            for (k = 0; k < SIZE; k++) {
                if (board[i][k] == 0 && board[j][k] != 0) {
                    board[i][k] = board[j][k];
                    board[j][k] = 0;

                    not_shift = 0;
                }
            }
        }
    }

    //We check whether there are 2 squares with the same number, so we can mergem them.
    for (i = 0, j = 1; i < SIZE - 1; i++, j++) {
        for (k = 0; k < SIZE; k++) {
            if (board[i][k] == board[j][k] && board[i][k] != 0) {
                board[i][k] = board[i][k] + board[j][k];
                board[j][k] = 0;

                *points = *points + board[i][k];
                not_merge = 0;
            }
        }
    }
    
    /*After we could merge some cells, we check again wether there is a 
    free space above the lines. If yes, we shift the columns.
    */
    for (t = 0; t < 3; t++) {
        for (i = 0, j = 1; i < SIZE - 1; i++, j++) {
            for (k = 0; k < SIZE; k++) {
                if (board[i][k] == 0 && board[j][k] != 0) {
                    board[i][k] = board[j][k];
                    board[j][k] = 0;

                    not_shift = 0;
                }
            }
        }
    }

    if (not_shift == 1 && not_merge == 1) {
        return 1;
    }

    return 0;
}

/*This function is created to be used when we want to move down in the game. It returns 1 
in case we didn't merge and shift, and 0 in case we did the actions.
*/
int move_down(int board[][SIZE], int *points) {
    int not_shift = 1;
    int not_merge = 1;

    int i, j, k, t;

    /*We check wether there is a free space on the i line and not on the j line.
    If yes, we move the j-k element on the i-k element.
    Where i and j are two consecutives lines.
    */
    for (t = 0; t < 3; t++) {
        for (i = SIZE - 1, j = SIZE - 2; i >= 1; i--, j--) {
            for (k = 0; k < SIZE; k++) {
                if (board[i][k] == 0 && board[j][k] != 0) {
                    board[i][k] = board[j][k];
                    board[j][k] = 0;

                    not_shift = 0;
                }
            }
        }
    }

    //We check whether there are 2 squares with the same number, so we can mergem them.
    for (i = SIZE - 1, j = SIZE - 2; i >= 1; i--, j--) {
        for (k = 0; k < SIZE; k++) {
            if (board[i][k] == board[j][k] && board[i][k] != 0) {
                board[i][k] = board[i][k] + board[j][k];
                board[j][k] = 0;

                *points = *points + board[i][k];
                not_merge = 0;
            }
        }
    }

    /*After we could have merged some cells, we check again wether there is a free 
    space bellow the lines. If yes, we shift the columns.
    */
    for (t = 0; t < 3; t++) {
        for (i = SIZE - 1, j = SIZE - 2; i >= 1; i--, j--) {
            for (k = 0; k < SIZE; k++) {
                if (board[i][k] == 0 && board[j][k] != 0) {
                    board[i][k] = board[j][k];
                    board[j][k] = 0;

                    not_shift = 0;
                }
            }
        }
    }

    if (not_shift == 1 && not_merge == 1) {
        return 1;
    }

    return 0;
}

/*This function is created to be used when we want to move left in the game. 
It returns 1 in case we didn't merge and shift, and 0 in case we did the actions.
*/
int move_left(int board[][SIZE], int *points) {
    int not_shift = 1;
    int not_merge = 1;

    int i, j, k, t;

    //We check wether there is a free space to the left side of the cells.
    //If yes, we shift the lines.
    for (t = 0; t < 3; t++) {
        for (i = 0, j = 1; i < SIZE - 1; i++, j++) {
            for (k = 0; k < SIZE; k++) {
                if (board[k][i] == 0 && board[k][j] != 0) {
                    board[k][i] = board[k][j];
                    board[k][j] = 0;

                    not_shift = 0;
                }
            }
        }
    }

    //We check whether there are 2 squares with the same number, so we can mergem them.
    for (i = 0, j = 1; i < SIZE - 1; i++, j++) {
        for (k = 0; k < SIZE; k++) {
            if (board[k][i] == board[k][j] && board[k][i] != 0) {
                board[k][i] = board[k][i] + board[k][j];
                board[k][j] = 0;

                *points = *points + board[k][i];
                not_merge = 0;
            }
        }
    }

     /*After we could have merged some cells, we check again wether there is a 
    free space in the left side.If yes, we shift the lines.
    */
    for (t = 0; t < 3; t++) {
        for (i = 0, j = 1; i < SIZE - 1; i++, j++) {
            for (k = 0; k < SIZE; k++) {
                if (board[k][i] == 0 && board[k][j] != 0) {
                    board[k][i] = board[k][j];
                    board[k][j] = 0;

                    not_shift = 0;
                }
            }
        }
    }

    if (not_shift == 1 && not_merge == 1) {
        return 1;
    }

    return 0;
}

/*This function is created to be used when we want to move right in the game. 
It returns 1 in case we didn't merge and shift, and 0 in case we did the actions.
*/
int move_right(int board[][SIZE], int *points) {
    int not_shift = 1;
    int not_merge = 1;

    int i, j, k, t;

    //We check wether there is a free space to the right side of the cells.
    //If yes, we shift the lines.
    for (t = 0; t < 3; t++) {
        for (i = SIZE - 1, j = SIZE - 2; i >= 1; i--, j--) {
            for (k = 0; k < SIZE; k++) {
                if (board[k][i] == 0 && board[k][j] != 0) {
                    board[k][i] = board[k][j];
                    board[k][j] = 0;

                    not_shift = 0;
                }
            }
        }
    }

    //We check whether there are 2 squares with the same number, so we can mergem them.
    for (i = SIZE - 1, j = SIZE - 2; i >= 1; i--, j--) {
        for (k = 0; k < SIZE; k++) {
            if (board[k][i] == board[k][j] && board[k][i] != 0) {
                board[k][i] = board[k][i] + board[k][j];
                board[k][j] = 0;

                *points = *points + board[k][i];
                not_merge = 0;
            }
        }
    }

    /*After we could have merged some cells, we check again wether there is a 
    free space in the right side.If yes, we shift the lines.
    */
    for (t = 0; t < 3; t++) {
        for (i = SIZE - 1, j = SIZE - 2; i >= 1; i--, j--) {
            for (k = 0; k < SIZE; k++) {
                if (board[k][i] == 0 && board[k][j] != 0) {
                    board[k][i] = board[k][j];
                    board[k][j] = 0;

                    not_shift = 0;
                }
            }
        }
    }

    if (not_shift == 1 && not_merge == 1) {
        return 1;
    }

    return 0;
}

/*We check if we win the game.
*/
int is_win(int board[][SIZE]) {
    int i, j;

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (board[i][j] == 2048) {
                return 1;
            }
        }
    }

    return 0;
}

int no_merge_possible(int board[][SIZE]) {
    int i, j, k;

    for (i = 0; i < SIZE; i++) {
        for (j = 0, k = 1; j < SIZE - 1; j++, k++) {
            if (board[i][j] == board[i][k]) {
                return 1;
            }
        }
    }

    for (i = 0; i < SIZE; i++) {
        for (j = 0, k = 1; j < SIZE - 1; j++, k++) {
            if (board[j][i] == board[k][i]) {
                return 1;
            }
        }
    }

    return 0;
}

/*This function is created to be used when we finished the game(doesn't 
matter if we won or no). It shows the final board.
*/
void print_final(int board[][SIZE]) {
    printf("%s\n", "Tabla finala:");

    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            printf("%4d ", board[i][j]);
        }

        printf("\n");
    }
}

/*This function copies the board and it's going to be used for the 
automatic moves.
*/
void copy_board(int board[][SIZE], int copy_board[][SIZE]) {
    int i, j;

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            copy_board[i][j] = board[i][j];
        }
    }
}

/*This function is created to be used for the automatic moves. We copy the 
board in 4 different places. We will make in the copy boards all the possible moves
(up, down, left, right) and we check in which one is the best (after which move we have 
the maximum free cells).
*/
void automatic_move(int board[][SIZE], int *real_points) {
    int up_board[SIZE][SIZE];
    int down_board[SIZE][SIZE];
    int left_board[SIZE][SIZE];
    int right_board[SIZE][SIZE];

    copy_board(board, up_board);
    copy_board(board, down_board);
    copy_board(board, left_board);
    copy_board(board, right_board);

    int tmp_points = 0;
    move_up(up_board, &tmp_points);
    move_down(down_board, &tmp_points);
    move_left(left_board, &tmp_points);
    move_right(right_board, &tmp_points);

    int free_cells_up = count_empty_cells(up_board);
    int free_cells_down = count_empty_cells(down_board);
    int free_cells_left = count_empty_cells(left_board);
    int free_cells_right = count_empty_cells(right_board);

    //Here we check which move is the best.
    int max = free_cells_up;
    if (free_cells_down > max) {
        max = free_cells_down;
    }

    if (free_cells_left > max) {
        max = free_cells_left;
    }

    if (free_cells_right > max) {
        max = free_cells_right;
    }

    //After we check which move is the best one, we will generate anothor number.
    if (max == free_cells_up) {
        if (move_up(board, real_points) == 0) {
            random_number(board, 1);
        }
    } else if (max == free_cells_down) {
        if (move_down(board, real_points) == 0) {
            random_number(board, 1);
        }
    } else if (max == free_cells_left) {
        if (move_left(board, real_points) == 0) {
            random_number(board, 1);
        }
    } else if (max == free_cells_right) {
        if (move_right(board, real_points) == 0) {
            random_number(board, 1);
        }
    }
}

int main() {
    srand(time(NULL));

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    int max_height, max_width;
    getmaxyx(stdscr, max_height, max_width);

    int board[SIZE][SIZE];
    init_board(board);

    int highlight_position = 0;
    int was_enter_pressed = 0;
    int is_first_time = 1;
    int punctaj_val = 0;

    clock_t start, end;
    
    //We initiliase all the screens that are going to be used.
    WINDOW *initial_menu = newwin(max_height, max_width / 4, BEGIN, BEGIN);
    WINDOW *board_menu = newwin(max_height / 2, max_width / 2, max_height / 4, max_width / 4);
    WINDOW *data_menu = newwin(max_height / 8, max_width / 4, BEGIN, 3 * max_width / 4);
    WINDOW *points_menu = newwin(max_height / 8, max_width / 4, 7 * max_height / 8, 3 * max_width / 4);
    WINDOW *legend_menu = newwin(max_height / 2, max_width / 4, max_height / 4, 3 * max_width / 4);

    while(1) {
        if (was_enter_pressed == 1) {
            nodelay(stdscr, TRUE);
            
            //In case we press quit.
            if (highlight_position == 2) {
                break;
            } else if (highlight_position == 0 || highlight_position == 1) {
                if (highlight_position == 0 && is_first_time == 1) {
                    init_board(board);
                    random_number(board, 2);
                    is_first_time = 0;
                }

                //If we won, we will display a message, the points and the final board.
                if (is_win(board) == 1) {
                    endwin();
                    printf("%s\n", "Felicitari! Ai castigat!");
                    printf("Punctaj total: %d\n", punctaj_val);
                    print_final(board);

                    return 0;
                }

                /*If we haven't won, but there are no possible moves, we will show  
                a message, the final board and the points.
                */
                if (no_merge_possible(board) == 0 && count_empty_cells(board) == 0) {
                    endwin();

                    printf("%s\n", "Nu mai sunt mutari posibile");
                    print_final(board);

                    return 0;
                }

                print_board_menu(board_menu, max_height / 2, max_width / 2, board);
                print_additional_menus(data_menu, points_menu, legend_menu, max_height, max_width, punctaj_val);

                int ch = getch();
                if (ch != ERR) { // s a apasat o tasta, resetez startul
                    start = clock();

                    /*If we press 'w', we will call the function 'move_up' to do the move.
                    If we press 's', we will call the function 'move_down' to do the move.
                    If we press 'd', we will call the function 'move_right' to do the move.
                    If we press 'a', we will call the function 'move_left' to do the move.
                    */
                    if (ch == 'q') {
                        was_enter_pressed = 0;
                        is_first_time = 1;
                    } else if (ch == 'w') {
                        if (move_up(board, &punctaj_val) == 0) {
                            random_number(board, 1);
                        }
                    } else if (ch == 's') {
                        if (move_down(board, &punctaj_val) == 0) {
                            random_number(board, 1);
                        }
                    } else if (ch == 'a') {
                        if (move_left(board, &punctaj_val) == 0) {
                            random_number(board, 1);
                        }
                    } else if (ch == 'd') {
                        if (move_right(board, &punctaj_val) == 0) {
                            random_number(board, 1);
                        }
                    }
                } else {
                    end = clock();

                    //If we don't make any move for 5 seconds, we will call the 'automatic_move'
                    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
                    if (seconds >= 5) {
                        automatic_move(board, &punctaj_val);
                        start = clock();
                    }
                }
            }
        } else if (was_enter_pressed == 0) {
            nodelay(stdscr, FALSE);

            determine_highlight_position(&highlight_position, board, &was_enter_pressed);
            print_initial_menu(initial_menu, max_height, max_width / 4, board, highlight_position);
        }
    }

    endwin();
    return 0;
}
