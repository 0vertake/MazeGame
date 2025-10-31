#include "GameLogic.h"
#include "MazeMatrix.h"
#include "MazeLayout.h"

#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <chrono>
#include <Windows.h>

using namespace std;

// Helper function to set cursor position in console
void setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(hConsole, pos);
}

// Helper function to hide the console cursor
void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// Helper function to show the console cursor
void showCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void GameLogic::run(int rows, int cols, int numberOfPowerUps) {
    if (rows < 15 || cols < 15) {
        cout << "Number of rows and columns must be over 15";
        Sleep(1000);
        return;
    }

    // Measures how long it took to generate the maze.
    auto start = chrono::high_resolution_clock::now();
    MazeMatrix maze(rows, cols, numberOfPowerUps);
    auto end = chrono::high_resolution_clock::now();
    auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Maze created in " << elapsed_time.count() << " milliseconds.\n\n";

    char move;
    bool gameRunning = true;

    cout << "INSTRUCTIONS:\n - Use arrow keys to move\n - Press Q to quit\n\n";
    Sleep(1000);
    
    // Clear screen once at the start
    system("cls");
    
    // Hide cursor for smoother display
    hideCursor();

    while (gameRunning) {
        // Set cursor to top-left corner instead of clearing screen
        setCursorPosition(0, 0);
        
        maze.display();

        // _getch() reads one character from input.
        move = _getch();

        // Arrow keys return a special code (-32 or 224) as the first character
        // and a specific number for each direction as the second character.
        if (move == -32 || move == 224) {
            move = _getch();
        }

        if (move == 'Q' || move == 'q') {
            gameRunning = false;
            continue;
        }

        if (!maze.moveRobot(move)) {
            // Set cursor below maze for error message
            setCursorPosition(0, rows + 2);
            cout << "Invalid move!     " << endl;
            Sleep(500);

            // Input buffer is cleared during message display so
            // the user cannot enter input during that time.
            while (_kbhit()) {  // _kbhit() returns true if any key is in the input buffer.
                (void)_getch();
            }
            
            // Clear the error message
            setCursorPosition(0, rows + 2);
            cout << "                  " << endl;
        }
        else {
            // If the robot moved, the minotaur also moves.
            maze.moveMinotaur();
        }
        // Writes entities into the maze.
        maze.updateEntities();

        if (maze.isGameLost()) {
            setCursorPosition(0, 0);
            maze.display();
            cout << "\nYou lost!" << endl;
            showCursor();
            Sleep(3000);
            gameRunning = false;
        }

        if (maze.isGameWon()) {
            setCursorPosition(0, 0);
            maze.display();
            cout << "\nYou won!" << endl;
            showCursor();
            Sleep(1000);
            gameRunning = false;
        }
    }
    
    // Show cursor again when game ends
    showCursor();
}
