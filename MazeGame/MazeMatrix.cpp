#include "MazeMatrix.h"
#include "MazeLayout.h"

#include <fstream>
#include <iostream>

using namespace std;

MazeMatrix::MazeMatrix(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    matrix = layout::initializeMatrix(rows, cols);
    robotX = 1;
    robotY = 0;
    for (int i = 1; i < cols - 1; i++) {
        if (matrix[1][i].getType() == 'R') {
            robotY = i;
        }
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
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j].getType();
        }
        cout << endl;
    }
}

bool MazeMatrix::isValidMove(int x, int y) const {
    if (x >= rows || y >= cols) {
        return false;
    }

    if (matrix[x][y].getType() == '#' || matrix[x][y].getType() == 'U') {
        return false;
    }
    return true;
}

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

    if (!isValidMove(newX, newY)) {
        return false;
    }

    matrix[robotX][robotY] = '.';
    matrix[newX][newY] = 'R';
    robotX = newX;
    robotY = newY;

    return true;
}

bool MazeMatrix::isGameWon() const {
    if (robotX == rows - 1) {
        return true;
    }
    return false;
}
