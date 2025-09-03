#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 3
#define X 'X'
#define O 'O'

typedef struct {
    int player;
    int computer;
    int draw;
} Score;

int difficulty;
Score score = {.player = 0, .computer = 0, .draw = 0};

void input_difficulty();
void clear_screen();
void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
int check_win(char board[BOARD_SIZE][BOARD_SIZE], char player);
int check_draw(char board[BOARD_SIZE][BOARD_SIZE]);
void play_game();
void player_move(char board[BOARD_SIZE][BOARD_SIZE]);
void computer_move(char board[BOARD_SIZE][BOARD_SIZE]);
int is_valid_move(char board[BOARD_SIZE][BOARD_SIZE], int row, int col);
void clean_input_buffer();

int main() {
    srand(time(NULL));
    int choice;

    input_difficulty();
    do {
        play_game();
        printf("\nPlay again? (1 for yes, 0 for no): ");
        while (scanf("%d", &choice) != 1 || (choice != 0 && choice != 1)) {
            printf("Invalid input. Please enter 1 for yes or 0 for no: ");
            clean_input_buffer();
        }
        clean_input_buffer(); 
    } while (choice == 1);

    printf("\nBye Bye, thanks for playing.\n");
    return 0;
}

void play_game() {
    char board[BOARD_SIZE][BOARD_SIZE] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '},
    };
    char current_player = rand() % 2 == 0 ? X : O;

    while (1) {
        print_board(board);
        if (current_player == X) {
            player_move(board);
            if (check_win(board, X)) {
                score.player++;
                print_board(board);
                printf("Congratulations! You have won!!!\n");
                break;
            }
            current_player = O;
        } else {
            printf("Computer's turn (O)...\n");
            computer_move(board);
            if (check_win(board, O)) {
                score.computer++;
                print_board(board);
                printf("I won!!! But you played well...\n");
                break;
            }
            current_player = X;
        }

        if (check_draw(board)) {
            score.draw++;
            print_board(board);
            printf("\nIt's a draw!\n");
            break;
        }
    }
}

int is_valid_move(char board[BOARD_SIZE][BOARD_SIZE], int row, int col) {
    return !(row < 0 || col < 0 ||
             row >= BOARD_SIZE || col >= BOARD_SIZE ||
             board[row][col] != ' ');
}

void player_move(char board[BOARD_SIZE][BOARD_SIZE]) {
    int row, col;
    do {
        printf("\nPlayer X's turn.");
        printf("\nEnter row and column (1-3): ");
        

        if (scanf("%d %d", &row, &col) != 2) {
            printf("Invalid input. Please enter two numbers.\n");
            clean_input_buffer();
            continue; 
        }
        clean_input_buffer(); 

        row--; col--; 

        if (!is_valid_move(board, row, col)) {
            printf("Invalid move. The cell is either taken or out of bounds. Try again.\n");
        }
    } while (!is_valid_move(board, row, col));
    
    board[row][col] = X;
}

void computer_move(char board[BOARD_SIZE][BOARD_SIZE]) {
    // 1. Play for an immediate win
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = O;
                if (check_win(board, O)) {
                    return; 
                }
                board[i][j] = ' '; 
            }
        }
    }

    // 2. Block the player from winning
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = X;
                if (check_win(board, X)) {
                    board[i][j] = O; 
                    return;
                }
                board[i][j] = ' '; 
            }
        }
    }

    // 3. "God Mode" strategy (if difficulty is 2)
    if (difficulty == 2) {
        if (board[1][1] == ' ') {
            board[1][1] = O;
            return;
        }

        int corners[4][2] = {{0, 0}, {0, 2}, {2, 0}, {2, 2}};
        for (int i = 0; i < 4; i++) {
            if (board[corners[i][0]][corners[i][1]] == ' ') {
                board[corners[i][0]][corners[i][1]] = O;
                return;
            }
        }
    }

    // 4. Default: Play the first available empty cell
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = O;
                return;
            }
        }
    }
}

int check_win(char board[BOARD_SIZE][BOARD_SIZE], char player) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) return 1;
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) return 1;
    }

    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[2][0] == player && board[1][1] == player && board[0][2] == player)) {
        return 1;
    }
    return 0;
}

int check_draw(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == ' ') {
                return 0; 
            }
        }
    }
    return 1; 
}

void print_board(char board[BOARD_SIZE][BOARD_SIZE]) {
    clear_screen();
    printf("Score - Player (X): %d | Computer (O): %d | Draws: %d\n", score.player, score.computer, score.draw);
    printf("-------------------\n");
    printf("   Tic-Tac-Toe\n\n");

    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("     ");
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf(" %c ", board[i][j]);
            if (j < BOARD_SIZE - 1) printf("|");
        }
        if (i < BOARD_SIZE - 1) printf("\n     ---+---+---\n");
    }
    printf("\n\n");
}

void input_difficulty() {
    do {
        printf("\nSelect difficulty level:\n");
        printf("1. Human (Standard)\n");
        printf("2. God (Impossible to win)\n");
        printf("Enter your choice: ");

        if (scanf("%d", &difficulty) != 1) {
            printf("\nInvalid input. Please enter a number.\n");
            clean_input_buffer();
            difficulty = 0; 
            continue;
        }
        clean_input_buffer();

        if (difficulty != 1 && difficulty != 2) {
            printf("\nIncorrect choice. Please enter 1 or 2.\n");
        }
    } while (difficulty != 1 && difficulty != 2);
}

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void clean_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
