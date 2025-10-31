#pragma once

#include "../MazeGame/Cell.h"
#include <vector>

namespace layout {

	int RNG(int min, int max);
	Cell** createMazeGrid(int rows, int cols);
	Cell** pathFinder(Cell** matrix, int rows, int cols, int startY, int endY, bool isPathGenerator);
	std::vector<Cell*> getNextCells(Cell** matrix, Cell* startCell, int rows, int cols, bool isPathGenerator, int randomExitY);
	bool checkMatrix(Cell** matrix, int rows, int cols);
	std::tuple<int, int> addMinotaur(Cell** matrix, int rows, int cols, int startY, int endY);
	Cell** generateMaze(int rows, int cols);
}