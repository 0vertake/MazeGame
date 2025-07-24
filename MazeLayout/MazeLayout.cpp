#include "pch.h"
#include "framework.h"

#include "MazeLayout.h"
#include "../MazeGame/Cell.h"

#include <random>
#include <iostream>
#include <tuple>
#include <chrono>


namespace layout {

    // Nasumicno generise broj u intervalu: [min, max].
    int RNG(int min, int max) {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<int> distribution(min, max);

        return distribution(generator);
    }

    Cell** createMatrixGrid(int rows, int cols, int randomEntranceY, int randomExitY) {
        Cell** matrix = new Cell * [rows];

        // Deli matricu u polja odvojena zidovima, kako bi DFS algoritam napravio bolji lavirint.
        int counter = 0;
        for (int i = 0; i < rows; i++) {
            matrix[i] = new Cell[cols];

            for (int j = 0; j < cols; j++) {
                if (i != 0 && j != 0 && i != rows - 1 && j != cols - 1 && i % 2 != 0 && j % 2 != 0) {
                    matrix[i][j] = Cell(i, j, '.');
                    matrix[i][j].setId(counter);
                    counter++;
                }
                else {
                    matrix[i][j] = Cell(i, j, '#');
                    matrix[i][j].setWasVisited(true);
                }
            }
        }

        // Nasumicno rasporedjuje ulaz.
        matrix[0][randomEntranceY] = 'U';

        // Nasumicno rasporedjuje izlaz i oznacava Cell iznad izlaza
        // da nije posecen, kako bi put do izlaza bio moguc.
        matrix[rows - 2][randomExitY].setWasVisited(false);
        matrix[rows - 1][randomExitY] = 'I';

        // Nastaje problem, ako je broj redova ili kolona paran broj. Prethodni raspored 
        // polja odvojenih zidovima nije koristan - pretposlednji red ili kolona budu samo
        // zidovi. Zbog ovoga nasumicno oznacavamo pola polja u tom redu ili koloni da bi ih
        // DFS algoritam posetio.
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

    Cell** pathFinder(Cell** matrix, int rows, int cols, int startY, int endY, bool isPathGenerator) {
        // Postavljanje pocetka za DFS Algoritam.
        int x = 1;
        int y = startY;

        std::vector<Cell*> cellsVisited;
        cellsVisited.push_back(&matrix[x][y]);

        // DFS Algoritam.
        while (true) {
            // Svi moguci Cell-ovi koje DFS moze da poseti iz trenutnog Cell-a.
            std::vector<Cell*> nextCells = getNextCells(matrix, &matrix[x][y], rows, cols, isPathGenerator);

            // Ako nema narednih Cell-ova za DFS, vracamo se unazad dok ne nadjemo
            // Cell iz kog mozemo posetiti neposecen Cell.
            if (nextCells.size() == 0) {
                for (int i = cellsVisited.size() - 1; i >= 0; i--) {
                    nextCells = getNextCells(matrix, cellsVisited[i], rows, cols, isPathGenerator);
                    x = cellsVisited[i]->getX();
                    y = cellsVisited[i]->getY();
                    if (nextCells.size() != 0) {
                        break;
                    }
                    // Ako smo stigli do pocetka posecenih Cell-ova, znaci da nijedan
                    // nema naredni Cell i da je DFS zavrsen.
                    if (i == 0) {
                        if (!isPathGenerator && !matrix[rows - 2][endY].getWasVisited()) {
                            matrix[0][0] = 'X';
                        }
                        return matrix;
                    }
                }
            }

            // Nasumicno biranje narednog Cell-a.
            Cell* nextCell = nextCells[RNG(0, nextCells.size() - 1)];

            // Ako smo preskocili jedan Cell, i tu "rusimo" zid.
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

    std::tuple<int, int> addMinotaur(Cell** matrix, int rows, int cols, int startY, int endY) {
        int minotaurX;
        int minotaurY;

        while (true) {

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (matrix[i][j].getType() == '.') {
                        matrix[i][j].setWasVisited(false);
                    }
                }
            }

            while (true) {
                minotaurX = RNG(2, rows - 2);
                minotaurY = RNG(1, cols - 2);

                if (matrix[minotaurX][minotaurY].getType() == '.') {
                    break;
                }
            }

            matrix[minotaurX][minotaurY].setWasVisited(true);
            matrix[minotaurX + 1][minotaurY].setWasVisited(true);
            matrix[minotaurX - 1][minotaurY].setWasVisited(true);
            matrix[minotaurX][minotaurY + 1].setWasVisited(true);
            matrix[minotaurX][minotaurY - 1].setWasVisited(true);
            matrix = pathFinder(matrix, rows, cols, startY, endY, false);

            if (matrix[0][0].getType() != 'X') {
                break;
            }
            matrix[0][0] = '#';
        }
        return std::make_tuple(minotaurX, minotaurY);
    }

    // Proverava da li izgenerisan lavirint ima minimalni broj unutrasnjih zidova:
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

    // Pronalazi moguce poteze za DFS algoritam u matrici. Potez je moguc, ako
    // Cell u pitanju nije posecen. Cell moze biti udaljen za jedan ili dva mesta
    // u bilo kom pravcu od startnog Cella.
    std::vector<Cell*> getNextCells(Cell** matrix, Cell* startCell, int rows, int cols, bool isPathGenerator) {
        std::vector<Cell*> nextCells;
        int x = startCell->getX();
        int y = startCell->getY();

        int diff = 2;
        if (!isPathGenerator) {
            diff = 1;
        }

        for (int i = x - diff; i <= x + diff; i++) {
            if (i > 0 && i < rows - 1 && !matrix[i][y].getWasVisited()) {
                nextCells.push_back(&matrix[i][y]);
            }
        }

        for (int i = y - diff; i <= y + diff; i++) {
            if (i > 0 && i < cols - 1 && !matrix[x][i].getWasVisited()) {
                nextCells.push_back(&matrix[x][i]);
            }
        }

        return nextCells;
    }

    int Id(Cell* cell) {
        int x = cell->getX();
        int y = cell->getY();
        return x + x/2 + (y + 1) / 2;
    }
    
    Cell** initializeMatrix(int rows, int cols) {
        int randomEntranceY = RNG(1, cols - 2);
        int randomExitY = RNG(1, cols - 2);

        Cell** matrix;
        matrix = createMatrixGrid(rows, cols, randomEntranceY, randomExitY);
        matrix = pathFinder(matrix, rows, cols, randomEntranceY, randomExitY, true);

        int minotaurX, minotaurY;
        std::tie(minotaurX, minotaurY) = addMinotaur(matrix, rows, cols, randomEntranceY, randomExitY);

        matrix[1][randomEntranceY] = 'R';
        matrix[minotaurX][minotaurY] = 'M';
        return matrix;
    }

}
