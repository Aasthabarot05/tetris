# tetris
# Tetris Game in C++ (Console Version)

A simple console-based **Tetris** game written in **C++**, using emojis to represent the tetrominoes and a basic ASCII interface to manage the game. The game logic includes handling the falling tetrominoes, rotating pieces, clearing lines, and managing game states such as pausing and game over.

## Features
- **Tetrominoes**: Uses different shapes of tetrominoes represented by emojis.
- **Controls**: Includes move left, right, rotate, and hard drop functionality.
- **Game Over**: Displays a game-over screen with the final score and high score.
- **Pause/Resume**: Ability to pause and resume the game.
- **Difficulty Levels**: The game has three difficulty levels that affect the speed of the falling blocks.
- **Score Tracking**: Keeps track of the current score and high score.

## Controls
- **Arrow Keys**:  
  - `←` - Move Left  
  - `→` - Move Right  
  - `↓` - Move Down  
  - `↑` - Rotate (Clockwise)
  
- **Spacebar**: Hard Drop (Drop the tetromino to the bottom instantly)
- **P**: Pause/Resume the game
- **Q**: Quit the game

## Gameplay Overview
The goal of the game is to control the falling tetrominoes by rotating them and positioning them on the game board. When a row is completely filled, it is cleared, and the player earns points. The game ends when there is no room left for new tetrominoes to spawn.

## Installation
This is a console-based game written in **C++**. Follow these steps to run the game:



1. **Compile the code**:
Use a C++ compiler such as **g++**. Open your terminal in the project folder and run:
g++ -o tetris main.cpp
2. **Run the game**:
After successful compilation, run the game using the following command:
./tetris
## Screenshots
**Main Menu:**
🧱==========================🧱 
      🎮 TETRIS GAME 🎮
🧱==========================🧱 
Controls: 
← - Move Left
→- Move Right 
↓ - Move Down 
↑ - Rotate Space 
- Hard Drop
- P - Pause/Resume
- Q - Quit
- Press any key to start...



**Game Over Screen:**
🔥====================🔥 
    💀 GAME OVER 💀
🔥====================🔥 
Final Score: 500 
High Score: 1000 
Press any key to exit...
