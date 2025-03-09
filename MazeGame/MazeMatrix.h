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

public:
    MazeMatrix(int rows, int cols);
    ~MazeMatrix();

    const Cell* operator[](int index) const;
    void display() const;
    bool moveRobot(char direction);
    bool isGameWon() const;

private:
    bool isValidMove(int x, int y) const;
};
