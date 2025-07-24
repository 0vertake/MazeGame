#pragma once

#include "Cell.h"
#include "PowerUps.h"
#include <string>
#include <vector>

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

    // Pokazivac na trenutni aktivni predmet.
    PowerUp* activePowerUp = nullptr;
    
public:
    MazeMatrix(int rows, int cols, int numberOfPowerUps);
    ~MazeMatrix();

    const Cell* operator[](int index) const;
    void display() const;
    bool moveRobot(char direction);
    void checkForPowerUp(int newX, int newY);
    void moveMinotaur();
    bool isRobotNextToMinotaur() const;
    bool isGameLost();
    bool isGameWon() const;
    void updateEntities();
    bool isValidMove(int x, int y, bool isPlayer) const;
};
