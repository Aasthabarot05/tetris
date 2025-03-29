#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

// Constants defining the dimensions of the game board
const int WIDTH = 10, HEIGHT = 20;
string board[HEIGHT][WIDTH];  // The game board (grid)
bool isPaused = false, isRunning = true;  // Game state variables
int score = 0, highScore = 0;  // Variables to store the current score and high score

// Structure to represent a Tetrimino (the falling pieces)
struct Tetrimino {
    int x, y;               // Position of the Tetrimino
    int shape[4][4];        // Shape of the Tetrimino (4x4 grid)
    string emoji;          // Emoji representation of the Tetrimino
};

// Array of all 7 possible Tetriminos (with emoji shapes)
Tetrimino pieces[7] = {
    {0, 0, {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, "🟦"}, // I-shape (Blue)
    {0, 0, {{1, 1}, {1, 1}}, "🟨"},                                    // O-shape (Yellow)
    {0, 0, {{0, 1, 0}, {1, 1, 1}, {0, 0, 0}}, "🟪"},                   // T-shape (Purple)
    {0, 0, {{0, 1, 1}, {1, 1, 0}, {0, 0, 0}}, "🟥"},                   // S-shape (Red)
    {0, 0, {{1, 1, 0}, {0, 1, 1}, {0, 0, 0}}, "🟩"},                   // Z-shape (Green)
    {0, 0, {{1, 0, 0}, {1, 1, 1}, {0, 0, 0}}, "🟧"},                   // J-shape (Orange)
    {0, 0, {{0, 0, 1}, {1, 1, 1}, {0, 0, 0}}, "⬜"}                    // L-shape (White)
};

// Current Tetrimino that is falling
Tetrimino current;

// Function to hide the console cursor (for better game visuals)
void hideCursor() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(console, &cursorInfo);
}

// Function to set the cursor position on the console screen
void setCursorPosition(int x, int y) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {x, y};
    SetConsoleCursorPosition(console, pos);
}

// Function to display the starting screen of the game
void startScreen() {
    system("cls");  // Clear the console screen
    cout << "🧱==========================🧱\n";
    cout << "        🎮 TETRIS GAME 🎮     \n";
    cout << "🧱==========================🧱\n";
    cout << "Controls:\n";
    cout << "← - Move Left  → - Move Right\n";
    cout << "↓ - Move Down  ↑ - Rotate\n";
    cout << "Space - Hard Drop\n";
    cout << "P - Pause/Resume  Q - Quit\n";
    cout << "Press any key to start...\n";
    _getch();  // Wait for user input to start the game
}

// Function to display the end screen (game over screen)
void endScreen() {
    system("cls");  // Clear the console screen
    cout << "🔥====================🔥\n";
    cout << "       💀 GAME OVER 💀      \n";
    cout << "🔥====================🔥\n";
    cout << "Final Score: " << score << endl;  // Display final score
    cout << "High Score: " << highScore << endl;  // Display high score
    cout << "Press any key to exit...\n";
    _getch();  // Wait for user input to exit the game
    exit(0);  // Exit the game
}

// Function to initialize the game board (set all cells to "⬛" which represents empty space)
void initBoard() {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            board[i][j] = "⬛";  // Set all cells to empty
}

// Function to draw the game board and display the current state
void drawBoard() {
    setCursorPosition(0, 0);  // Move cursor to the top-left corner
    cout << "Score: " << score << "   High Score: " << highScore << "\n";  // Display scores

    // Draw the top border of the game board
    cout << "🧱";
    for (int i = 0; i < WIDTH; i++) cout << "🧱";
    cout << "🧱\n";

    // Draw the game grid and the falling Tetriminoes
    for (int i = 0; i < HEIGHT; i++) {
        cout << "🧱";
        for (int j = 0; j < WIDTH; j++) {
            bool isPartOfCurrent = false;

            // Check if the current Tetrimino is part of this cell
            for (int pi = 0; pi < 4; pi++) {
                for (int pj = 0; pj < 4; pj++) {
                    if (current.shape[pi][pj] && i == current.y + pi && j == current.x + pj) {
                        cout << current.emoji;  // Print the emoji of the Tetrimino
                        isPartOfCurrent = true;
                        break;
                    }
                }
                if (isPartOfCurrent) break;
            }

            // If this cell doesn't contain part of the current Tetrimino, print the board cell
            if (!isPartOfCurrent) {
                cout << board[i][j];
            }
        }
        cout << "🧱\n";
    }

    // Draw the bottom border of the game board
    cout << "🧱";
    for (int i = 0; i < WIDTH; i++) cout << "🧱";
    cout << "🧱\n";
}

// Function to check if a given move for a Tetrimino is valid
bool isValidMove(Tetrimino piece, int dx, int dy) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (piece.shape[i][j]) {
                int newX = piece.x + j + dx;  // New x position
                int newY = piece.y + i + dy;  // New y position
                // Check if the move is out of bounds or collides with another block
                if (newX < 0 || newX >= WIDTH || newY >= HEIGHT || (newY >= 0 && board[newY][newX] != "⬛"))
                    return false;
            }
        }
    }
    return true;  // Return true if the move is valid
}

// Function to spawn a new Tetrimino (choose a random piece from the list)
void spawnPiece() {
    current = pieces[rand() % 7];  // Randomly select a Tetrimino
    current.x = WIDTH / 2 - 2;     // Start the piece in the middle horizontally
    current.y = 0;                 // Start the piece at the top of the screen
    // If the piece cannot be placed at the top, the game is over
    if (!isValidMove(current, 0, 0)) {
        if (score > highScore) highScore = score;  // Update high score if needed
        endScreen();  // Show the game over screen
    }
}

// Function to clear full lines on the board
void clearLines() {
    for (int i = HEIGHT - 1; i >= 0; i--) {
        bool full = true;
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == "⬛") {  // If there's an empty cell, the line is not full
                full = false;
                break;
            }
        }
        // If the line is full, clear it and move all lines above it down
        if (full) {
            score += 10;  // Increase score for clearing a line
            for (int k = i; k > 0; k--)
                for (int j = 0; j < WIDTH; j++)
                    board[k][j] = board[k - 1][j];  // Move lines down

            // Clear the top line
            for (int j = 0; j < WIDTH; j++) board[0][j] = "⬛";
            i++;  // Recheck the current line
        }
    }
}

// Function to place the current Tetrimino on the board
void placePiece() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (current.shape[i][j])
                board[current.y + i][current.x + j] = current.emoji;  // Place the emoji on the board
}

// Function to perform a hard drop (instant drop to the bottom)
void hardDrop() {
    while (isValidMove(current, 0, 1)) current.y++;  // Keep moving the piece down
    placePiece();  // Place the piece at the bottom
    clearLines();  // Clear any completed lines
    spawnPiece();  // Spawn a new piece
    Beep(700, 100);  // Play a beep sound
}

// Function to move the Tetrimino in the specified direction
void movePiece(int dx, int dy) {
    if (isValidMove(current, dx, dy)) {
        current.x += dx;  // Update x position
        current.y += dy;  // Update y position
    }
}

// Function to rotate the current Tetrimino
void rotatePiece() {
    int temp[4][4] = {0};  // Temporary 4x4 array for rotation

    // Rotate the shape of the Tetrimino by 90 degrees
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            temp[j][3 - i] = current.shape[i][j];

    Tetrimino rotated = current;
    memcpy(rotated.shape, temp, sizeof(temp));  // Copy the rotated shape

    // If the rotated piece is valid, update the current piece
    if (isValidMove(rotated, 0, 0))
        current = rotated;
}

// Function to determine the speed based on the difficulty level
int getSpeed(int level) {
    switch (level) {
        case 1: return 500;  // Easy speed
        case 2: return 300;  // Medium speed
        case 3: return 200;  // Hard speed
        default: return 500; // Default to easy speed
    }
}

// Function to choose the difficulty level of the game
int chooseDifficulty() {
    int level;
    system("cls");
    cout << "🧱==========================🧱\n";
    cout << "       🎮 Choose Difficulty 🎮\n";
    cout << "🧱==========================🧱\n";
    cout << "1. Easy\n2. Medium\n3. Hard\n";
    cout << "Select level (1-3): ";
    cin >> level;  // Get the user's choice
    return level;  // Return the chosen level
}

// Main game loop that handles all the gameplay mechanics
void gameLoop(int level) {
    while (isRunning) {
        if (!isPaused) {
            drawBoard();  // Draw the current board state

            if (_kbhit()) {  // Check if a key has been pressed
                int ch = _getch();  // Get the key pressed

                if (ch == 224) {  // Arrow key press
                    ch = _getch();  // Get the second byte for arrow keys
                    if (ch == 75) movePiece(-1, 0);  // Left arrow
                    else if (ch == 77) movePiece(1, 0);  // Right arrow
                    else if (ch == 80) movePiece(0, 1);  // Down arrow
                    else if (ch == 72) rotatePiece();  // Up arrow (rotate)
                }
                else if (ch == ' ') hardDrop();  // Spacebar (hard drop)
                else if (ch == 'p' || ch == 'P') isPaused = !isPaused;  // Pause/Resume
                else if (ch == 'q' || ch == 'Q') { isRunning = false; return; }  // Quit
            }

            // Move the piece down automatically
            if (isValidMove(current, 0, 1)) current.y++;
            else {
                placePiece();  // Place the piece when it can't move down
                clearLines();  // Clear full lines
                spawnPiece();  // Spawn a new piece
                Beep(500, 100);  // Play a sound
            }
        }

        Sleep(getSpeed(level));  // Sleep to control game speed based on difficulty
    }
}

// Main function that starts the game
int main() {
    SetConsoleOutputCP(CP_UTF8);  // Set the console to support UTF-8 characters (for emojis)
    srand(time(0));  // Seed the random number generator
    hideCursor();  // Hide the cursor for better gameplay experience
    startScreen();  // Display the start screen
    system("cls");  // Clear the screen after the start screen
    int level = chooseDifficulty();  // Let the user choose the difficulty level
    system("cls");  // Clear the screen after choosing difficulty
    initBoard();  // Initialize the game board
    spawnPiece();  // Spawn the first Tetrimino
    gameLoop(level);  // Start the main game loop
    return 0;
}
