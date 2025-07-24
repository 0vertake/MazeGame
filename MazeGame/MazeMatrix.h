#pragma once

#include "Cell.h"
#include <string>

class MazeMatrix {
private:
    int rows;
    int cols;
    Cell** matrix;
    int robotX;
    int robotY;
    bool isMinotaurAlive;
    int minotaurX;
    int minotaurY;

public:
    MazeMatrix(int rows, int cols);
    ~MazeMatrix();

    const Cell* operator[](int index) const;
    void display() const;
    bool moveRobot(char direction);
    void moveMinotaur();
    bool isRobotNextToMinotaur() const;
    bool isGameWon() const;
    bool isGameLost() const;

private:
    bool isValidMove(int x, int y) const;
};
