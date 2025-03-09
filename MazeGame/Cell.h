#pragma once

class Cell {
private:
    int x;
    int y;
    char cellType;
    bool wasVisited;

public:
    Cell(int x = 0, int y = 0, char cellType = ' ');

    char getType() const;
    int getX() const;
    int getY() const;
    bool getWasVisited() const;
    void setWasVisited(bool visited);

    Cell& operator=(char newType);

    Cell& operator=(const Cell& other);


};
