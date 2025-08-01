# DX-Ball

DX-Ball is a classic 2D brick-breaking arcade game developed as a CSE 102 Level-1 Term-1 project. The main objective is to break all the bricks in each level using a ball while controlling a paddle. The game is built with the iGraphics library in C/C++.

## Features

- **Multiple Levels:** Progress through levels with increasing difficulty and unique brick arrangements.
- **Dynamic Ball Physics:** Realistic ball movement with collision detection for bricks, walls, and paddle.
- **Score and Lives:** Track your score by breaking bricks and try to survive with limited lives.
- **Theme Song:** Enjoy background music while playing.
- **Save/Load System:** Save your game progress and top scores.

## Getting Started

### Prerequisites

- **C/C++ Compiler:** GCC, MinGW, or any compatible C/C++ compiler.
- **iGraphics Library:** [iGraphics](https://github.com/monzurulkader/igraphics) must be installed and configured.
  
### Installation

1. **Clone the repository:**
   ```sh
   git clone https://github.com/Wahidul-Hoque/DX-Ball.git
   cd DX-Ball
   ```

2. **Set up iGraphics:**
   - Download and configure iGraphics for your platform.
   - Ensure the library files are linked in your build settings.

3. **Build the project:**
   - Compile `iMain.cpp` along with iGraphics and `stb_image.h`.
   - Example (Windows/MinGW):
     ```sh
     g++ iMain.cpp -o DXBall -lgraphics
     ```

### Running the Game

Run the executable generated after compilation:
```sh
./DXBall
```
Use your mouse and keyboard to control the paddle and navigate menus.

## Gameplay Instructions

- **Move Paddle:** Use mouse movement to control the paddle horizontally.
- **Start Game:** Select level and press the play button.
- **Break Bricks:** Bounce the ball off the paddle to break bricks and score points.
- **Lose a Life:** If the ball falls below the paddle, you lose a life.
- **Game Over:** The game ends when all lives are lost.

## Code Structure

- `iMain.cpp`: Main game logic, rendering, input handling, and level management.
- `stb_image.h`: Image loading for graphics.
- `txt_files/`: Stores saved games and high scores.


## Acknowledgments

Supervised by Lecturer Saem Hasan (Dept. of CSE, BUET).

---
