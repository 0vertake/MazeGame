#include "MazeMatrix.h"
#include "MazeLayout.h"

#include <fstream>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

MazeMatrix::MazeMatrix(int rows, int cols, int numberOfPowerUps) {
    this->rows = rows;
    this->cols = cols;
    matrix = layout::generateMaze(rows, cols);
    robotX = 1; // Robot will always be in the first row.
    robotY = 0;
    isMinotaurAlive = true;
    minotaurX = 0;
    minotaurY = 0;

    // Searches for the robot and minotaur in the generated maze and records their indices.
    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            if (matrix[1][j].getType() == 'R') {
                robotY = j;
            }
            if (matrix[i][j].getType() == 'M') {
                minotaurX = i;
                minotaurY = j;
            }
        }
    }

    // Randomly places power-ups in the maze.
    for (int i = 0; i < numberOfPowerUps; i++) {
        int x, y;
        while (true) {
            x = layout::RNG(1, rows - 2);
            y = layout::RNG(1, cols - 2);
            if (matrix[x][y].getType() == '.') {
                break;
            }
        }
        matrix[x][y] = 'P';
    }
}

MazeMatrix::~MazeMatrix() {
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

const Cell* MazeMatrix::operator[](int index) const {
    return matrix[index];
}

void MazeMatrix::display() const {
    // Displays the current power-up.
    if (activePowerUp) {
        cout << "Current power-up active for " << activePowerUp->getRemainingTurns() << " more turn(s): " << activePowerUp->getName() << endl;
    }
    // Display the maze.
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Reduces visibility in case of fog of war.
            if (activePowerUp && activePowerUp->getName() == "Fog of War") {
                if (!(abs(i - robotX) < 2 && abs(j - robotY) < 2)) {
                    cout << ' ';
                    continue;
                }
            }
            cout << matrix[i][j].getType();
        }
        cout << endl;
    }
}

// Writes entities (robot and minotaur) into the maze.
void MazeMatrix::updateEntities() {
    matrix[robotX][robotY] = 'R';
    if (isMinotaurAlive) {
        matrix[minotaurX][minotaurY] = 'M';
    }
}
 
bool MazeMatrix::isValidMove(int x, int y, bool isPlayer) const {
    // Allows the robot to "break through" a wall, only if it's not an outer wall.
    if (isPlayer && x < rows - 1 && y < cols - 1 && activePowerUp && activePowerUp->getName() == "Hammer") {
        return true;
    }
    // Doesn't allow the minotaur to exit the maze.
    if (!isPlayer && matrix[x][y].getType() == 'I') {
        return false;
    }
    // Doesn't allow anyone to pass through a wall or exit through the entrance.
    if (matrix[x][y].getType() == '#' || matrix[x][y].getType() == 'U') {
        return false;
    }
    return true;
}

// Moves the robot based on the provided move and checks if the robot stepped on a power-up.
bool MazeMatrix::moveRobot(char move) {
    int newX = robotX;
    int newY = robotY;

    switch (move) {
    case 72:
        newX--;
        break;
    case 80:
        newX++;
        break;
    case 75:
        newY--;
        break;
    case 77:
        newY++;
        break;
    default:
        return false;
    }

    if (!isValidMove(newX, newY, true)) {
        return false;
    }

    // Checks if the robot stepped on a power-up.
    checkForPowerUp(newX, newY);

    // If yes, then counts down the turns.
    if (activePowerUp) {
        activePowerUp->tick();
        if (!activePowerUp->isActive()) {
            activePowerUp = nullptr;
        }
    }

    matrix[robotX][robotY] = '.';
    robotX = newX;
    robotY = newY;

    return true;
}

void MazeMatrix::checkForPowerUp(int newX, int newY) {
    // If the robot stepped on a power-up, randomly determines which one it will be and activates it.
    if (matrix[newX][newY].getType() == 'P') {
        int effectType = layout::RNG(0, 3);
        switch (effectType) {
        case 0: 
            activePowerUp = new Fog(); 
            break;
        case 1: 
            activePowerUp = new Sword(); 
            break;
        case 2: 
            activePowerUp = new Shield(); 
            break;
        case 3: 
            activePowerUp = new Hammer(); 
            break;
        }
        activePowerUp->activate();
    }
}

// Moves the minotaur randomly.
void MazeMatrix::moveMinotaur() {
    if (!isMinotaurAlive) {
        return;
    }

    int newX;
    int newY;

    while (true) {
        newX = minotaurX;
        newY = minotaurY;

        // Randomly chooses a move until it's valid.
        int randomDirection = layout::RNG(0, 3);
        switch (randomDirection) {
        case 0:
            newX++;
            break;
        case 1:
            newX--;
            break;
        case 2:
            newY++;
            break;
        case 3:
            newY--;
            break;
        }

        if (isValidMove(newX, newY, false)) {
            break;
        }
   }
    matrix[minotaurX][minotaurY] = '.';
    minotaurX = newX;
    minotaurY = newY;
}

// Check if the robot is next to the minotaur as in the visualization:
//    .
//   .M.
//    .
bool MazeMatrix::isRobotNextToMinotaur() const {
    if (std::abs(robotX - minotaurX) <= 1 && std::abs(robotY - minotaurY) <= 1) {
        return true;
    }
    return false;
}

bool MazeMatrix::isGameLost() {
    // Checks if the minotaur can kill the robot.
    if (isRobotNextToMinotaur() && isMinotaurAlive) {
        if (activePowerUp) {
            // If the robot has a shield - the minotaur can't kill it.
            if (activePowerUp->getName() == "Shield") {
                return false;
            }
            // If the robot has a sword - it automatically kills the minotaur.
            if (activePowerUp->getName() == "Sword") {
                isMinotaurAlive = false;
                matrix[minotaurX][minotaurY] = '.';
                cout << "You killed the minotaur!" << endl;
                return false;
            }
        }
        return true;
    }
}

bool MazeMatrix::isGameWon() const {
    if (robotX == rows - 1) {
        return true;
    }
    return false;
}
