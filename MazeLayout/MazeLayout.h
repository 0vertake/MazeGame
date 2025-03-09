#pragma once

#include "../MazeGame/Cell.h"
#include <vector>

namespace layout {

	int RNG(int min, int max);
	Cell** initializeMatrix(int rows, int cols);
	bool checkMatrix(Cell** matrix, int rows, int cols);
	std::vector<Cell*> getNextCells(Cell** matrix, Cell* startCell, int rows, int cols);
}