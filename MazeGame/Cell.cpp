#include "Cell.h"

Cell::Cell(int x, int y, char cellType) {
    this->x = x;
    this->y = y;
    this->cellType = cellType;
    wasVisited = false;
}

char Cell::getType() const {
    return cellType;
}

int Cell::getX() const {
    return x;
}

int Cell::getY() const {
    return y;
}

bool Cell::getWasVisited() const {
    return wasVisited;
}

void Cell::setWasVisited(bool visited) {
    wasVisited = visited;
}

Cell& Cell::operator=(char newType) {
    cellType = newType;
    return *this;
}

Cell& Cell::operator=(const Cell& other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
        cellType = other.cellType;
    }
    return *this;
}
