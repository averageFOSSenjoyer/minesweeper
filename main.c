#include "main.h"

int include_boarder = 0;

board* create_board(int rows, int cols) {
    board* game = malloc(sizeof(board));
    game->cells = malloc(sizeof(*game->cells) * rows);
    game->cells_sol = malloc(sizeof(*game->cells_sol) * rows);
    game->cells_adj = malloc(sizeof(*game->cells_adj) * rows);
    game->rows = rows;
    game->cols = cols;
    game->mines = 0;
    for (int i = 0; i < cols; i++) {
        game->cells[i] = malloc(sizeof(*game->cells[i]) * cols);
        game->cells_sol[i] = malloc(sizeof(*game->cells_sol[i]) * cols);
        game->cells_adj[i] = malloc(sizeof(*game->cells_adj[i]) * cols);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            game->cells[i][j] = '*';
        }
    }

    srand(time(0));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (rand() % 5 == 0) {
                game->cells_sol[i][j] = '!';
                game->mines++;
            }
            else {
                game->cells_sol[i][j] = '*';
            }
        }
    }
    return game;
}

int create_adjacency(board* game) {
    int total = 0;
    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            game->cells_adj[i][j] = 0;
        }
    }
    for (int s_row = 0; s_row < game->rows; s_row++) {
        for (int s_col = 0; s_col < game->cols; s_col++) {
            for (int i = s_row - 1; i <= s_row + 1; i++) {
                if (i >= 0 && i < game->rows) {
                    for (int j = s_col - 1; j <= s_col + 1; j++) {
                        if (j >= 0 && j < game->cols) {
                            if (i != s_row || j != s_col){
                                if (game->cells_sol[i][j] == '!') {
                                    game->cells_adj[s_row][s_col]++;
                                }
                            }
                        }
                    }
                }
            }
            if (game->cells_adj[s_row][s_col] > 4) {
                return 1;
            }
        }
    }
    
    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            if (game->cells_sol[i][j] == '!') {
                game->cells_adj[i][j] = '!';
            }
        }
    }
    for (int s_row = 0; s_row < game->rows; s_row++) {
        for (int s_col = 0; s_col < game->cols; s_col++) {
            if (game->cells_adj[s_row][s_col] == '!') {
                total++;
            }
        }
    }
    if (total < (game->rows * game->cols / 6)) {
        return 1;
    }
    return 0;
}

int spread_board(board* game, int s_row, int s_col) {
    if (game->cells_adj[s_row][s_col] > 0) {
        game->cells[s_row][s_col] = game->cells_adj[s_row][s_col] + 48;
        return 1;
    }
    if (game->cells_adj[s_row][s_col] < 0) {
        return 1;
    } 
    if (game->cells_adj[s_row][s_col] == 0) {
        game->cells[s_row][s_col] = 48;
        game->cells_adj[s_row][s_col] = -1;
    }
    
    if (s_row - 1 >= 0) {
        if (game->cells_adj[s_row - 1][s_col] >= 0) {
            spread_board(game, s_row - 1, s_col);
        }
        if (s_col - 1 >= 0) {
            if (game->cells_adj[s_row - 1][s_col - 1] >= 0) {
                spread_board(game, s_row - 1, s_col - 1);
            }
        }
        if (s_col + 1 < game->cols) {
            if (game->cells_adj[s_row - 1][s_col + 1] >= 0) {
                spread_board(game, s_row - 1, s_col + 1);
            }
        }
    }
    if (s_col - 1 >= 0) {
        if (game->cells_adj[s_row][s_col - 1] >= 0) {
            spread_board(game, s_row, s_col - 1);
        }
    }
    if (s_row + 1 < game->rows) {
        if (game->cells_adj[s_row + 1][s_col] >= 0) {
            spread_board(game, s_row + 1, s_col);
        }
        if (s_col - 1 >= 0) {
            if (game->cells_adj[s_row + 1][s_col - 1] >= 0) {
                spread_board(game, s_row + 1, s_col - 1);
            }
        }
        if (s_col + 1 < game->cols) {
            if (game->cells_adj[s_row + 1][s_col + 1] >= 0) {
                spread_board(game, s_row + 1, s_col + 1);
            }
        }
    }
    if (s_col + 1 < game->cols) {
        if (game->cells_adj[s_row][s_col + 1] >= 0) {
            spread_board(game, s_row, s_col + 1);
        }
    }
    return 0;
}

// void print_adjacency(board* game) {
//     for (int i = 0; i < game->rows; i++) {
//         for (int j = 0; j < game->cols; j++) {
//             printf("%c ", game->cells_adj[i][j] + 48);
//         }
//         printf("\n");
//     }
// }

// void print_sol(board* game) {
//     for (int i = 0; i < game->rows; i++) {
//         for (int j = 0; j < game->cols; j++) {
//             printf("%c ", game->cells_sol[i][j]);
//         }
//         printf("\n");
//     }
// }

void print_board(board* game, int s_row, int s_col, int selected) {
    system("cls");
    for (int i = 0; i < game->cols + 2; i++) {
        printf("- ");
    }
    printf("\n");
    for (int i = 0; i < game->rows; i++) {
        printf("| ");
        for (int j = 0; j < game->cols; j++) {
            if (game->cells[i][j] == '!') {
                printf("\x1b[1;33m%c\x1b[0m ", game->cells[i][j]);
            }
            else {
                printf("%c ", game->cells[i][j]);
            }
        }
        printf("|");
        if (i == s_row) {
            if (selected) {
                printf(" \x1b[0;36m<\x1b[0m");
            }
            else {
                printf(" <");
            }
        }
        printf("\n");
    }
    for (int i = 0; i < game->cols + 2; i++) {
        printf("- ");
    }
    printf("\n");
    for (int i = 0; i < s_col + 1; i++) {
        printf("  ");
    }
    if (selected) {
        printf("\x1b[0;36m^\x1b[0m\n");
    }
    else {
        printf("^\n");
    }
}

int process_game(board* game) {
    int s_row = 0;
    int s_col = 0;
    int mine;
    int first_step = 1;
    char c;
    deselect:
    mine = 0;
    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            if (game->cells[i][j] == '!' && game->cells_sol[i][j] != '!') {
                goto out;
            }
            if (game->cells[i][j] == '!' && game->cells_sol[i][j] == '!') {
                mine++;
            }
        }
    }
    if (mine == game->mines) {
        return 2;
    }
    out:
    do {
        // print_sol(game);
        // print_adjacency(game);
        print_board(game, s_row, s_col, 0);
        c = getch();
        if ((c == 'w' || c == 'W') && s_row > 0) {
            s_row--;
        }
        if ((c == 'a' || c == 'A') && s_col > 0) {
            s_col--;
        }
        if ((c == 's' || c == 'S') && s_row < game->rows - 1) {
            s_row++;
        }
        if ((c == 'd' || c == 'D') && s_col < game->cols - 1) {
            s_col++;
        }
        if ((c == 'q' || c == 'Q')) {
            return 0;
        }
    } while (c != '\r');
    if (game->cells[s_row][s_col] == '*' || game->cells[s_row][s_col] == '!' || game->cells[s_row][s_col] == '?') {
        do {
            print_board(game, s_row, s_col, 1);
            c = getch();
        } while (c != 'm' && c != 'u' && c != 27 && c != 'l');
    }
    else {
        printf("Invalid selection\n");
        goto deselect;
    }
    switch (c) {
        case 27:
            goto deselect;
        case 'l':
            game->cells[s_row][s_col] = '!';
            goto deselect;
        case 'u':
            game->cells[s_row][s_col] = '*';
            goto deselect;
    }
    if (game->cells_sol[s_row][s_col] == '!') {
        if (first_step) {
            for (int i = s_row - 1; i <= s_row + 1; i++) {
                if (i >= 0 && i < game->rows) {
                    for (int j = s_col - 1; j <= s_col + 1; j++) {
                        if (j >= 0 && j < game->cols) {
                            if (i != s_row || j != s_col){
                                if (game->cells_sol[i][j] != '!') {
                                    game->cells_sol[i][j] = '!';
                                    game->cells_sol[s_row][s_col] = '*';
                                    create_adjacency(game);
                                    spread_board(game, s_row, s_col);
                                    goto deselect;
                                }
                            }
                        }
                    }
                }
            }
        }
        else {
            system("cls");
            for (int i = 0; i < game->cols + 2; i++) {
                printf("- ");
            }
            printf("\n");
            for (int i = 0; i < game->rows; i++) {
                printf("| ");
                for (int j = 0; j < game->cols; j++) {
                    if (i == s_row && j == s_col) {
                        printf("\x1b[1;31m!\x1b[0m ");
                    }
                    else if (game->cells_sol[i][j] == '!') {
                        printf("! ");
                    }
                    else {
                        printf("%c ", game->cells[i][j]);
                    }
                }
                printf("|\n");
            }
            for (int i = 0; i < game->cols + 2; i++) {
                printf("- ");
            }
            printf("\n");
            return 1;
        }
    }
    first_step = 0;
    spread_board(game, s_row, s_col);
    goto deselect;
    return 0;
}

void destroy_board(board* game) {
    for (int i = 0; i < game->cols; i++) {
        free(game->cells[i]);
        free(game->cells_sol[i]);
        free(game->cells_adj[i]);
    }
    free(game->cells);
    free(game->cells_sol);
    free(game->cells_adj);
    free(game);
    game = NULL;
}

void print_menu(int option) {
    printf("            _                                                   \n");
    printf("           (_)                                                  \n");
    printf("  _ __ ___  _ _ __   ___  _____      _____  ___ _ __   ___ _ __ \n");
    printf(" | '_ ` _ \\| | '_ \\ / _ \\/ __\\ \\ /\\ / / _ \\/ _ \\ '_ \\ / _ \\ '__|\n");
    printf(" | | | | | | | | | |  __/\\__ \\\\ V  V /  __/  __/ |_) |  __/ |   \n");
    printf(" |_| |_| |_|_|_| |_|\\___||___/ \\_/\\_/ \\___|\\___| .__/ \\___|_|   \n");
    printf("                                               | |              \n");
    printf("                                               |_|              \n");
    printf("\n");
    printf("\n");
    if (option == 0) {
        printf("                      ---->  Start\n");
        printf("\n");
        printf("                             Manual\n");
        printf("\n");
        printf("                            Options\n");
        printf("\n");
        printf("                              Exit\n");
    }
    if (option == 1) {
        printf("                             Start\n");
        printf("\n");
        printf("                      ---->  Manual\n");
        printf("\n");
        printf("                            Options\n");
        printf("\n");
        printf("                              Exit\n");
    } 
    if (option == 2) {
        printf("                             Start\n");
        printf("\n");
        printf("                             Manual\n");
        printf("\n");
        printf("                      ----> Options\n");
        printf("\n");
        printf("                              Exit\n");
    } 
    if (option == 3) {
        printf("                             Start\n");
        printf("\n");
        printf("                             Manual\n");
        printf("\n");
        printf("                            Options\n");
        printf("\n");
        printf("                      ---->   Exit\n");
    }
}

int menu() {
    int option = 0;
    char c;
    menu:
    do {
        system("cls");
        print_menu(option);
        c = getch();
        if (c == 'w' || c == 'W') {
            if (option != 0) {
                option--;
            }
            else {
                option = 3;
            }
        }
        if (c == 's' || c == 'S') {
            if (option != 3) {
                option++;
            }
            else {
                option = 0;
            }
        }
        if (c == 27) {
            return 0;
        }
    } while (c != '\r');
    if (option == 3) {
        return 0;
    }
    if (option == 2) {
        do {
            system("cls");
            printf("# i honestly thought i had something to put here but ig not, press esc to leave");
        } while (getch() != 27);
        goto menu;
    }
    if (option == 1) {
        do {
            system("cls");
            printf("# /*--------------------Selecting-Cell--------------------*/\n");
            printf("# W A S D -  Move the indicator arrow for selecting a cell\n");
            printf("#\n");
            printf("# ENTER - Select the indicated cell\n");
            printf("#\n");
            printf("# ESC -  Deselect the indicated cell\n");
            printf("#\n");
            printf("#\n");
            printf("# /*--------------------Selected-Cell---------------------*/ (When the indicator arrows are red)\n");
            printf("# M - Step(?) on the selected cell\n");
            printf("#\n");
            printf("# L - Mark the selected cell as a mine\n");
            printf("#\n");
            printf("# U - Mark the selected cell as unknown\n");
            printf("#\n");
            printf("# Q - Force quit to menu\n\n");
            printf("press esc to exit this menu...");
        } while (getch() != 27);
        goto menu;
    }
    return 1;
}

int main() {
    HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTitle("Minesweeper");
    SMALL_RECT windowSize = {0, 0, 80, 20};
    SetConsoleWindowInfo(wHnd, 1, &windowSize);
    COORD bufferSize = {10, 10};
    SetConsoleScreenBufferSize(wHnd, bufferSize);

    int rows, cols;
    restart:
    if (menu() == 0) {
        return 0;
    }

    printf("\nPlease enter game dimension: ");
    scanf("%d %d", &rows, &cols);
    
    board* game = create_board(rows, cols);
    while (create_adjacency(game)) {
        destroy_board(game);
        game = create_board(rows,cols);
    }
    switch (process_game(game)) {
        case 1:
            printf("rip bozo\n");
            break;
        case 2:
            printf("gj lmao\n");
            break;
    }
    destroy_board(game);
    system("pause");
    goto restart;
}