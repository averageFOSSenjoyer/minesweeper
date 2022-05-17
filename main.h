#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

typedef struct {
    int rows;
    int cols;
    int mines;
    char** cells;
    char** cells_sol;
    char** cells_adj;
} board;

board* create_board(int rows, int cols);
int create_adjacency(board* game);
int spread_board(board* game, int s_row, int s_col);
void print_board(board* game, int s_row, int s_col, int selected);
int process_game(board* game);
void destroy_board(board* game);
void print_menu(int option);
int menu();
void adjustWindowSize();