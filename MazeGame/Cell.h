#pragma once

class Cell {
private:
    int x;
    int y;
    char cellType;
    int id;
    bool wasVisited;

public:
    Cell(int x = 0, int y = 0, char cellType = ' ');

    char getType() const;
    int getX() const;
    int getY() const;
    bool getWasVisited() const;
    void setWasVisited(bool visited);
    int getId() const;
    void setId(int id);

    Cell& operator=(char newType);

    Cell& operator=(const Cell& other);


};
