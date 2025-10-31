#include "pch.h"
#include "framework.h"

#include "MazeLayout.h"
#include "../MazeGame/Cell.h"

#include <random>
#include <iostream>
#include <tuple>
#include <chrono>


namespace layout {

    // Randomly generates a number in the range: [min, max].
    int RNG(int min, int max) {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<int> distribution(min, max);

        return distribution(generator);
    }

    // Initializes the matrix on which pathFinder will generate the maze and prepares it.
    Cell** createMazeGrid(int rows, int cols) {
        Cell** matrix = new Cell * [rows];

        // Divides the matrix into cells separated by walls, so pathFinder can generate the maze.
        // Example:
        // #########
        // #.#.#.#.#
        // #########
        // #.#.#.#.#
        // #########
        // #.#.#.#.#
        // #########
        for (int i = 0; i < rows; i++) {
            matrix[i] = new Cell[cols];

            for (int j = 0; j < cols; j++) {
                if (i != 0 && j != 0 && i != rows - 1 && j != cols - 1 && i % 2 != 0 && j % 2 != 0) {
                    matrix[i][j] = Cell(i, j, '.');
                }
                else {
                    matrix[i][j] = Cell(i, j, '#');
                    matrix[i][j].setWasVisited(true);
                }
            }
        }

        // A problem arises if the number of rows or columns is even. The previous layout 
        // of cells separated by walls is not useful - the second-to-last row or column becomes only
        // walls. Because of this, we randomly mark half the cells in that row or column so that the
        // DFS algorithm can visit them.
        if (rows % 2 == 0) {
            for (int i = 0; i < cols / 2 - 2; i++) {
                matrix[rows - 2][RNG(1, cols - 2)].setWasVisited(false);
            }
        }
        if (cols % 2 == 0) {
            for (int i = 0; i < rows / 2 - 2; i++) {
                matrix[RNG(1, rows - 2)][cols - 2].setWasVisited(false);
            }
        }

        return matrix;
    }

    // Used for generating the maze, but also for checking whether it's possible to reach the exit.
    // Works based on DFS algorithm and does what its name says - finds a path.
    Cell** pathFinder(Cell** matrix, int rows, int cols, int startY, int endY, bool isPathGenerator) {
        // Setting the start (entrance).
        int x = 1;
        int y = startY;

        std::vector<Cell*> cellsVisited;
        cellsVisited.push_back(&matrix[x][y]);

        // DFS Algorithm.
        while (true) {
            // All possible Cells that pathFinder can visit from the current Cell.
            std::vector<Cell*> nextCells = getNextCells(matrix, &matrix[x][y], rows, cols, isPathGenerator, endY);

            // If there are no next Cells for pathFinder, we go back until we find
            // a Cell from which we can visit an unvisited Cell.
            if (nextCells.size() == 0) {
                for (int i = cellsVisited.size() - 1; i >= 0; i--) {
                    nextCells = getNextCells(matrix, cellsVisited[i], rows, cols, isPathGenerator, endY);
                    x = cellsVisited[i]->getX();
                    y = cellsVisited[i]->getY();
                    if (nextCells.size() != 0) {
                        break;
                    }
                    // If we've reached the beginning of visited Cells, it means none
                    // have a next Cell and pathFinder has finished its job.
                    if (i == 0) {
                        // Checks if the algorithm finished but didn't reach the end.
                        // This ensures passage is possible past the minotaur.
                        if (!isPathGenerator && !matrix[rows - 2][endY].getWasVisited()) {
                            matrix[0][0] = 'X';
                        }
                        return matrix;
                    }
                }
            }

            // Randomly choosing the next Cell.
            Cell* nextCell = nextCells[RNG(0, nextCells.size() - 1)];

            // If pathFinder is forming the maze, then we "break down" the wall.
            if (isPathGenerator) {
                int betweenX = (nextCell->getX() - x) / 2;
                int betweenY = (nextCell->getY() - y) / 2;
                matrix[x + betweenX][y + betweenY] = '.';
                *nextCell = '.';
            }

            x = nextCell->getX();
            y = nextCell->getY();
            cellsVisited.push_back(nextCell);
            matrix[x][y].setWasVisited(true);
        }
    }

    // Randomly places the Minotaur in the maze with a check to ensure the exit is reachable.
    std::tuple<int, int> addMinotaur(Cell** matrix, int rows, int cols, int startY, int endY) {
        int minotaurX;
        int minotaurY;

        while (true) {
            // Sets all passages (not walls) as unvisited cells, which pathFinder
            // will navigate through.
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (matrix[i][j].getType() == '.') {
                        matrix[i][j].setWasVisited(false);
                    }
                }
            }

            while (true) {
                // Random placement of minotaur in the maze.
                minotaurX = RNG(2, rows - 2);
                minotaurY = RNG(1, cols - 2);

                if (matrix[minotaurX][minotaurY].getType() == '.') {
                    break;
                }
            }

            // Marks all cells around the minotaur as visited, which prevents
            // pathFinder from passing right next to it.
            matrix[minotaurX][minotaurY].setWasVisited(true);
            matrix[minotaurX + 1][minotaurY].setWasVisited(true);
            matrix[minotaurX - 1][minotaurY].setWasVisited(true);
            matrix[minotaurX][minotaurY + 1].setWasVisited(true);
            matrix[minotaurX][minotaurY - 1].setWasVisited(true);
            matrix = pathFinder(matrix, rows, cols, startY, endY, false);

            // pathFinder will set an X flag on the first Cell if it's impossible
            // to reach the goal because of the minotaur.
            if (matrix[0][0].getType() != 'X') {
                break;
            }
            matrix[0][0] = '#';
        }
        return std::make_tuple(minotaurX, minotaurY);
    }

    // Checks if the generated maze has a minimum number of internal walls:
    // min = 2 * (cols + rows)
    bool checkMatrix(Cell** matrix, int rows, int cols) {
        int min = 2 * (cols + rows);

        int insideWalls = 0;
        for (int i = 1; i < rows - 1; i++) {
            for (int j = 1; j < cols - 1; j++) {
                if (matrix[i][j].getType() == '#') {
                    insideWalls++;
                }
            }
        }

        if (insideWalls < min) {
            return false;
        }
        return true;
    }

    // Finds possible moves for the DFS algorithm in the matrix. A move is possible if
    // the Cell in question hasn't been visited. Cell can be one or two places away
    // in any direction from the starting Cell.
    std::vector<Cell*> getNextCells(Cell** matrix, Cell* startCell, int rows, int cols, bool isPathGenerator, int randomExitY) {
        std::vector<Cell*> nextCells;
        int x = startCell->getX();
        int y = startCell->getY();

        int diff = 2;
        if (!isPathGenerator) {
            diff = 1;
        }

        // Finds all possible next Cells in the column (up-down).
        for (int i = x - diff; i <= x + diff; i++) {
            // Allows the next Cell to be on the last row if the exit is there.
            if ((i > 0 && i < rows - 1) || (i == rows - 1 && y == randomExitY)) {
                if (!matrix[i][y].getWasVisited()) {
                    nextCells.push_back(&matrix[i][y]);
                }
            }
        }

        // Finds all possible next Cells in the row (left-right).
        for (int i = y - diff; i <= y + diff; i++) {
            if (i > 0 && i < cols - 1 && !matrix[x][i].getWasVisited()) {
                nextCells.push_back(&matrix[x][i]);
            }   
        }

        return nextCells;
    }

    // Creates a complete maze matrix for MazeMatrix.
    Cell** generateMaze(int rows, int cols) {
        int randomEntranceY = RNG(1, cols - 2);
        int randomExitY = RNG(1, cols - 2);

        // Initial grid for pathFinder.
        Cell** matrix = createMazeGrid(rows, cols);
        // Marks the exit as a Cell that needs to be visited and ensures the exit is accessible.
        matrix[rows - 2][randomExitY].setWasVisited(false);
        matrix[rows - 2][randomExitY] = '.';
        matrix[rows - 1][randomExitY].setWasVisited(false);
        // pathFinder functions as PathGenerator to form the maze.
        matrix = pathFinder(matrix, rows, cols, randomEntranceY, randomExitY, true);

        int minotaurX, minotaurY;
        std::tie(minotaurX, minotaurY) = addMinotaur(matrix, rows, cols, randomEntranceY, randomExitY);
        
        // Writes the robot, minotaur, entrance, exit.
        matrix[0][randomEntranceY] = 'U';
        matrix[rows - 1][randomExitY] = 'I';
        matrix[1][randomEntranceY] = 'R';
        matrix[minotaurX][minotaurY] = 'M';

        return matrix;
    }

}
