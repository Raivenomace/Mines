// Michael Zang
// FA23-CSE-20311-01
// Dec 10, 2023
// A casino game where the user selects tiles on a grid until they either cashout and earn their winnings or click on a bomb and lose their bet. They can customize the number of bombs (leads to higher reward), the bet size, and the balance they woud like to start with. The game continues to loop and ask the user to play the game until they either quit or lose all their money.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "gfx.h"

#define BOARD_SIZE 5
#define MAX_MINES 6
#define MULTIPLIER 10
#define CELL_SIZE 100

// Structure definition
typedef struct{
    bool ismine;
    bool isreveal;
} mines;

// Function declarations
void initializeBoard(mines board[BOARD_SIZE][BOARD_SIZE], int numMines);
void drawBoard(mines board[BOARD_SIZE][BOARD_SIZE]);
void playGame(double *money, int *count, mines board[BOARD_SIZE][BOARD_SIZE], double multiplierData[][6], int numMines, double bet);
void checkGameOver(double money, bool *gameOver);
void displayPlayAgainPrompt(bool *gameOver, int *count);

int main() {

    // Hardcoded multiplier data for the number of mines (rows) and the number of tiles that are not mines (columns)
    // Hardcoded as recommended by a TA
    double multiplierData[][MAX_MINES] = {
        {1.03, 1.03, 1.07, 1.13, 1.18, 1.25},
        {1.04, 1.12, 1.23, 1.35, 1.5, 1.66},
        {1.05, 1.23, 1.41, 1.64, 1.91, 2.25},
        {1.05, 1.35, 1.64, 2.0, 2.48, 3.1},
        {1.06, 1.5, 1.91, 2.48, 3.25, 4.34},
        {1.07, 1.66, 2.25, 3.1, 4.34, 6.2},
        {1.07, 1.86, 2.67, 3.92, 5.89, 9.06},
        {1.08, 2.09, 3.21, 5.04, 8.15, 13.59},
        {1.09, 2.37, 3.9, 6.6, 11.55, 21},
        {1.1, 2.71, 4.8, 8.8, 16.8, 33.61},
        {1.12, 3.13, 6, 12, 25.21, 56.02},
        {1.14, 3.65, 7.63, 16.8, 39.21, 98.04},
        {1.16, 4.31, 9.93, 24.27, 63.72, 182}, 
        {1.19, 5.18, 13.24, 36.41, 109.25, 364.16},
        {1.23, 6.33, 18.2, 57.22, 200.29, 801.16},
        {1.28, 7.91, 26.01, 95.37, 400.58, 2000},
        {1.34, 10.17, 39.01, 171.67, 901.31, 6001},
        {1.44, 13.57, 62.42, 343.55, 2400, 24004},
        {1.59, 19, 109.25, 801.16, 8410, 168000},
        {1.82, 28.5, 218.5, 2400, 50470, 0},
        {2.24, 47.5, 546.25, 12020, 0, 0},
        {3.06, 95, 2190, 0, 0, 0},
        {5.12, 285, 0, 0, 0, 0},
        {23.74, 0, 0, 0, 0, 0},
    };

    // Variable declarations
    int count;
    int numMines;
    double money, bet;
    bool gameOver = false;

    // Seed for randomization
    srand(time(NULL));
    // Create the screen and display a welcome message
    gfx_open(BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE, "Mines Casino Game");
    char welcome[] = "Welcome to the Mines Casino Game!";
    gfx_color(255, 255, 255);
    gfx_text(150, 220, welcome);
    // Warning as clicking on the screen before the grid, reveals the tile beforehand
    char warning[] = "Please do not click on the screen. Look at the terminal!";
    gfx_color(255, 0, 0);
    gfx_text(80, 280, warning);

    gfx_flush();

    // Ask the user for the settings to start the game
    printf("Enter your initial money: ");
    scanf("%lf", &money);

    // Check conditions for all the options
    if (money <= 0) {
        printf("Invalid amount. Please try again.\n");
        return 1;
    }

    while (1){
        count = 0;
        
        printf("Enter the number of mines (1-6): ");
        scanf("%d", &numMines);

        while (numMines > MAX_MINES || numMines < 1) {
            printf("Invalid number of mines. Please enter a valid number.\n");
            printf("Enter the number of mines (1-6): ");
            scanf("%d", &numMines);
        }

        printf("Place your bet (Higher than 0.1$): ");
        scanf("%lf", &bet);

        while (bet > money || bet < 0.1) {
            printf("Insufficient balance. Please enter a valid bet.\n");
            printf("Place your bet (Higher than 0.1$): ", numMines);
            scanf("%lf", &bet);
        }

        printf("Game has started. Press e whenever you would like to cash out!\n");

        gfx_clear();

        // Create the grid for the game to work on
        mines board[BOARD_SIZE][BOARD_SIZE];
        initializeBoard(board, numMines);
        drawBoard(board);

        // Start the logic of the game
        while (!gameOver) {
            playGame(&money, &count, board, multiplierData, numMines, bet);
            // Checks to see if their money is 0 to end the game
            checkGameOver(money, &gameOver);
            // Keeps asking the user to play again
            if (!gameOver) {
                displayPlayAgainPrompt(&gameOver, &count);
                if(gameOver) break;
            }

            printf("Enter the number of mines (1-6): ");
            scanf("%d", &numMines);

            while (numMines > MAX_MINES || numMines < 1) {
                printf("Invalid number of mines. Please enter a valid number.\n");
                printf("Enter the number of mines (1-6): ");
                scanf("%d", &numMines);
            }

            printf("Place your bet (Higher than 0.1$): ");
            scanf("%lf", &bet);

            while (bet > money || bet < 0.1) {
                printf("Insufficient balance. Please enter a valid bet.\n");
                printf("Place your bet (Higher than 0.1$): ", numMines);
                scanf("%lf", &bet);
            }

            printf("Game has started. Press e whenever you would like to cash out!\n");

            gfx_clear();
            initializeBoard(board, numMines);
            drawBoard(board);
            
        }

        if (gameOver) {
            break;
        }
    }
    return 0;
}

// Creates the grid logic by setting conditions of if the tile is a mine or not
void initializeBoard(mines board[BOARD_SIZE][BOARD_SIZE], int numMines) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j].ismine = false;
            board[i][j].isreveal = false;
        }
    }

    int minesPlaced = 0;
    while (minesPlaced < numMines) {
        // Randomize the mines
        srand(time(0));
        int row = rand() % BOARD_SIZE;
        int col = rand() % BOARD_SIZE;

        // Place the mines
        if (!board[row][col].ismine) {
            board[row][col].ismine = true;
            minesPlaced++;
        }
    }
}

// Creates the actual board and displayes the mines and not mine tiles
void drawBoard(mines board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            int x = j * CELL_SIZE;
            int y = i * CELL_SIZE;

            gfx_color(255, 255, 255);
            // Draw lines to create a grid
            gfx_line(x, y, x + CELL_SIZE, y);
            gfx_line(x, y, x, y + CELL_SIZE);

            if (board[i][j].isreveal) {
                if (board[i][j].ismine) {
                    // Red and X for revealed mines
                    gfx_color(255, 0, 0); 
                    gfx_text(x + CELL_SIZE / 2, y + CELL_SIZE / 2, "X"); 
                } 
                else {
                    // Green and O for revealed tiles
                    gfx_color(0, 255, 0); 
                    gfx_text(x + CELL_SIZE / 2, y + CELL_SIZE / 2, "O");
                }
                // Reset color to white
                gfx_color(255, 255, 255);
            }
        }
    }
}

// Game logic
void playGame(double *money, int *count, mines board[BOARD_SIZE][BOARD_SIZE], double multiplierData[][6], int numMines, double bet) {
    double multiplier, temp;
    while(1) {
        // Wait for user input
        char c = gfx_wait();
        int row = gfx_xpos() / CELL_SIZE;
        int col = gfx_ypos() / CELL_SIZE;

        // Cashout option
        if(c == 'e'){
            *money += temp;
            printf("Congratulations! Money won: %.2f\n", temp);
            printf("Current Balance: %.2f\n", *money);
            break;
        }

        // Checks to see if the user is clicking on the same tile and to not increment if they are
        if(!board[row][col].isreveal){
            multiplier = multiplierData[*count][numMines - 1];
            temp = bet * multiplier - bet;
        }

        // Reveals the tile
        if(c == 1) {
            // If it is not a mine, reveal it and increment the counter
            if(!board[row][col].ismine) {
                gfx_color(0, 255, 0);
                gfx_text(row * CELL_SIZE + CELL_SIZE / 2, col * CELL_SIZE + CELL_SIZE / 2, "O");
                printf("Your Bet: %.2f, Multiplier: %.2f, Money won: %.2f\n", bet, multiplier, temp);
                if(!board[row][col].isreveal) {
                    (*count)++;
                    board[row][col].isreveal = true;
                }
            }
            // If it is a mine, break out of the loop to ask the user to play again
            else if (board[row][col].ismine) {
                    *money -= bet;
                    gfx_color(255, 0, 0); // Red for revealed mines
                    gfx_text(row * CELL_SIZE + CELL_SIZE / 2, col * CELL_SIZE + CELL_SIZE / 2, "X");
                    gfx_flush();
                    printf("Oops! You hit a mine.\n");
                    printf("Current Balance: %.2f\n", *money);
                    break;
            }
            else {
                // Check for win condition
                int tilesRemaining = BOARD_SIZE * BOARD_SIZE - numMines;
                if (tilesRemaining == 0) {
                    gfx_flush();
                    printf("Congratulations! You cleared the board. Money won: %.2f\n", *money);
                    printf("Current Balance: %.2f\n", *money);
                    break;
                }
            }
        }
    }

    // 1 second pause to allow the user to see the mine they hit or the board they cleared
    usleep(1000000);
    gfx_clear();
}

// Check to see if the user ran out of money
void checkGameOver(double money, bool *gameOver) {
    if (money <= 0) {
        printf("Game over. You ran out of money.\n");
        *gameOver = true;
    }
}

// Ask the user to play again
void displayPlayAgainPrompt(bool *gameOver, int *count) {
    while (1) {
        char text[] = "Do you want to play again (y/n)";
        gfx_color(255, 255, 255);
        gfx_text(150, 220, text);
        char warning2[] = "Please do not click on the screen. Look at the terminal!";
        gfx_color(255, 0, 0);
        gfx_text(80, 280, warning2);
        char d = gfx_wait();
        printf("%c\n", d);
        // If user plays again, the loop is reset and the count is reset
        if (d == 'y') {
            *gameOver = false;
            *count = 0;
            break;
        // If user does not play again, the loop breaks, and quits the program
        } else if (d == 'n') {
            *gameOver = true;
            break;
        }
    }

    if (*gameOver) {
        return;
    }
}
