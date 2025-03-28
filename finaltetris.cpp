
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

const int WIDTH = 10, HEIGHT = 20;
string board[HEIGHT][WIDTH];
bool isPaused = false, isRunning = true;
int score = 0, highScore = 0;

struct Tetrimino {
    int x, y;
    int shape[4][4];
    string emoji;
};

Tetrimino pieces[7] = {
    {0, 0, {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, "🟦"},
    {0, 0, {{1, 1}, {1, 1}}, "🟨"},
    {0, 0, {{0, 1, 0}, {1, 1, 1}, {0, 0, 0}}, "🟪"},
    {0, 0, {{0, 1, 1}, {1, 1, 0}, {0, 0, 0}}, "🟥"},
    {0, 0, {{1, 1, 0}, {0, 1, 1}, {0, 0, 0}}, "🟩"},
    {0, 0, {{1, 0, 0}, {1, 1, 1}, {0, 0, 0}}, "🟧"},
    {0, 0, {{0, 0, 1}, {1, 1, 1}, {0, 0, 0}}, "⬜"}
};

Tetrimino current;

void hideCursor() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(console, &cursorInfo);
}

void setCursorPosition(int x, int y) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {x, y};
    SetConsoleCursorPosition(console, pos);
}

void startScreen() {
    system("cls");
    cout << "🧱==========================🧱\n";
    cout << "        🎮 TETRIS GAME 🎮     \n";
    cout << "🧱==========================🧱\n";
    cout << "Controls:\n";
    cout << "← - Move Left  → - Move Right\n";
    cout << "↓ - Move Down  ↑ - Rotate\n";
    cout << "Space - Hard Drop\n";
    cout << "P - Pause/Resume  Q - Quit\n";
    cout << "Press any key to start...\n";
    _getch();
}

void endScreen() {
    system("cls");
    cout << "🔥====================🔥\n";
    cout << "       💀 GAME OVER 💀      \n";
    cout << "🔥====================🔥\n";
    cout << "Final Score: " << score << endl;
    cout << "High Score: " << highScore << endl;
    cout << "Press any key to exit...\n";
    _getch();
    exit(0);
}

void initBoard() {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            board[i][j] = "⬛";
}

void drawBoard() {
    setCursorPosition(0, 0);
    cout << "Score: " << score << "   High Score: " << highScore << "\n";

    cout << "🧱";
    for (int i = 0; i < WIDTH; i++) cout << "🧱";
    cout << "🧱\n";

    for (int i = 0; i < HEIGHT; i++) {
        cout << "🧱";
        for (int j = 0; j < WIDTH; j++) {
            bool isPartOfCurrent = false;

            for (int pi = 0; pi < 4; pi++) {
                for (int pj = 0; pj < 4; pj++) {
                    if (current.shape[pi][pj] && i == current.y + pi && j == current.x + pj) {
                        cout << current.emoji;
                        isPartOfCurrent = true;
                        break;
                    }
                }
                if (isPartOfCurrent) break;
            }

            if (!isPartOfCurrent) {
                cout << board[i][j];
            }
        }
        cout << "🧱\n";
    }

    cout << "🧱";
    for (int i = 0; i < WIDTH; i++) cout << "🧱";
    cout << "🧱\n";
}

bool isValidMove(Tetrimino piece, int dx, int dy) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (piece.shape[i][j]) {
                int newX = piece.x + j + dx;
                int newY = piece.y + i + dy;
                if (newX < 0 || newX >= WIDTH || newY >= HEIGHT || (newY >= 0 && board[newY][newX] != "⬛"))
                    return false;
            }
        }
    }
    return true;
}

void spawnPiece() {
    current = pieces[rand() % 7];
    current.x = WIDTH / 2 - 2;
    current.y = 0;
    if (!isValidMove(current, 0, 0)) {
        if (score > highScore) highScore = score;
        endScreen();
    }
}

void clearLines() {
    for (int i = HEIGHT - 1; i >= 0; i--) {
        bool full = true;
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == "⬛") {
                full = false;
                break;
            }
        }
        if (full) {
            score += 10;
            for (int k = i; k > 0; k--)
                for (int j = 0; j < WIDTH; j++)
                    board[k][j] = board[k - 1][j];

            for (int j = 0; j < WIDTH; j++) board[0][j] = "⬛";
            i++;
        }
    }
}

void placePiece() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (current.shape[i][j])
                board[current.y + i][current.x + j] = current.emoji;
}

void hardDrop() {
    while (isValidMove(current, 0, 1)) current.y++;
    placePiece();
    clearLines();
    spawnPiece();
    Beep(700, 100);
}

void movePiece(int dx, int dy) {
    if (isValidMove(current, dx, dy)) {
        current.x += dx;
        current.y += dy;
    }
}

void rotatePiece() {
    int temp[4][4] = {0};
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            temp[j][3 - i] = current.shape[i][j];

    Tetrimino rotated = current;
    memcpy(rotated.shape, temp, sizeof(temp));

    if (isValidMove(rotated, 0, 0))
        current = rotated;
}

int getSpeed(int level) {
    switch (level) {
        case 1: return 500;   
        case 2: return 300;   
        case 3: return 200;   
        default: return 500;
    }
}

int chooseDifficulty() {
    int level;
    system("cls");
    cout << "🧱==========================🧱\n";
    cout << "       🎮 Choose Difficulty 🎮\n";
    cout << "🧱==========================🧱\n";
    cout << "1. Easy\n2. Medium\n3. Hard\n";
    cout << "Select level (1-3): ";
    cin >> level;
    return level;
}

void gameLoop(int level) {
    while (isRunning) {
        if (!isPaused) {
            drawBoard();

            if (_kbhit()) {
                int ch = _getch();

               
                if (ch == 224) {
                    ch = _getch();  
                    if (ch == 75) movePiece(-1, 0);  
                    else if (ch == 77) movePiece(1, 0);  
                    else if (ch == 80) movePiece(0, 1);  
                    else if (ch == 72) rotatePiece();    
                }
                else if (ch == ' ') hardDrop(); 
                else if (ch == 'p' || ch == 'P') isPaused = !isPaused;
                else if (ch == 'q' || ch == 'Q') { isRunning = false; return; }
            }

            if (isValidMove(current, 0, 1)) current.y++;
            else {
                placePiece();
                clearLines();
                spawnPiece();
                Beep(500, 100);
            }
        }

        Sleep(getSpeed(level));  
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    srand(time(0)); hideCursor(); startScreen(); system("cls"); 
    int level = chooseDifficulty(); 
    system("cls"); 
    initBoard(); 
    spawnPiece(); 
    gameLoop(level); 
    return 0;
}