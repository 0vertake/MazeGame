#include "MazeMatrix.h"
#include "MazeLayout.h"

#include <fstream>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

MazeMatrix::MazeMatrix(int rows, int cols, int numberOfPowerUps) {
    this->rows = rows;
    this->cols = cols;
    matrix = layout::generateMaze(rows, cols);
    robotX = 1; // Robot ce se uvek nalaziti u prvom redu.
    robotY = 0;
    isMinotaurAlive = true;
    minotaurX = 0;
    minotaurY = 0;

    // Trazi robota i minotaura u izgenerisanom lavirintu i zapisuje njihove indekse.
    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            if (matrix[1][j].getType() == 'R') {
                robotY = j;
            }
            if (matrix[i][j].getType() == 'M') {
                minotaurX = i;
                minotaurY = j;
            }
        }
    }

    // Ubacuje nasumicno predmete u lavirint
    for (int i = 0; i < numberOfPowerUps; i++) {
        int x, y;
        while (true) {
            x = layout::RNG(1, rows - 2);
            y = layout::RNG(1, cols - 2);
            if (matrix[x][y].getType() == '.') {
                break;
            }
        }
        matrix[x][y] = 'P';
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
    // Prikazuje trenutni predmet.
    if (activePowerUp) {
        cout << "Trenutni predmet aktivan jos " << activePowerUp->getRemainingTurns() << " potez(a): " << activePowerUp->getName() << endl;
    }
    // Prikaz lavirinta.
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Smanjuje vidljivost u slucaju magle rat.
            if (activePowerUp && activePowerUp->getName() == "Magla rata") {
                if (!(abs(i - robotX) < 2 && abs(j - robotY) < 2)) {
                    cout << ' ';
                    continue;
                }
            }
            cout << matrix[i][j].getType();
        }
        cout << endl;
    }
}

// Upisuje entitete (robota i minotaura) u lavirint.
void MazeMatrix::updateEntities() {
    matrix[robotX][robotY] = 'R';
    if (isMinotaurAlive) {
        matrix[minotaurX][minotaurY] = 'M';
    }
}
 
bool MazeMatrix::isValidMove(int x, int y, bool isPlayer) const {
    // Dozvoljava robotu da "probije" zid, jedino ako nije spoljasni.
    if (isPlayer && x < rows - 1 && y < cols - 1 && activePowerUp && activePowerUp->getName() == "Cekic") {
        return true;
    }
    // Ne dozvoljava minotauru da izadje iz lavirinta.
    if (!isPlayer && matrix[x][y].getType() == 'I') {
        return false;
    }
    // Ne dozvoljava nikome da prodje kroz zid ili da izadje kroz ulaz.
    if (matrix[x][y].getType() == '#' || matrix[x][y].getType() == 'U') {
        return false;
    }
    return true;
}

// Pomera robota na osnovu prosledjenog poteza i proverava da li je robot stao na predmet.
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

    if (!isValidMove(newX, newY, true)) {
        return false;
    }

    // Proverava da li je robot stao na predmet.
    checkForPowerUp(newX, newY);

    // Ako jeste onda odbrojava poteze.
    if (activePowerUp) {
        activePowerUp->tick();
        if (!activePowerUp->isActive()) {
            activePowerUp = nullptr;
        }
    }

    matrix[robotX][robotY] = '.';
    robotX = newX;
    robotY = newY;

    return true;
}

void MazeMatrix::checkForPowerUp(int newX, int newY) {
    // Ako je robot stao na predmet, nasumicno odredjuje koji ce biti i aktivira ga.
    if (matrix[newX][newY].getType() == 'P') {
        int effectType = layout::RNG(0, 3);
        effectType = 3;
        switch (effectType) {
        case 0: 
            activePowerUp = new Fog(); 
            break;
        case 1: 
            activePowerUp = new Sword(); 
            break;
        case 2: 
            activePowerUp = new Shield(); 
            break;
        case 3: 
            activePowerUp = new Hammer(); 
            break;
        }
        activePowerUp->activate();
    }
}

// Pomera minotaura nasumicno.
void MazeMatrix::moveMinotaur() {
    if (!isMinotaurAlive) {
        return;
    }

    int newX;
    int newY;

    while (true) {
        newX = minotaurX;
        newY = minotaurY;

        // Nasumicno bira potez dok ne bude ispravan.
        int randomDirection = layout::RNG(0, 3);
        switch (randomDirection) {
        case 0:
            newX++;
            break;
        case 1:
            newX--;
            break;
        case 2:
            newY++;
            break;
        case 3:
            newY--;
            break;
        }

        if (isValidMove(newX, newY, false)) {
            break;
        }
   }
    matrix[minotaurX][minotaurY] = '.';
    minotaurX = newX;
    minotaurY = newY;
}

// Provera da li je robot pored minotaura kao na vizualizaciji:
//    .
//   .M.
//    .
bool MazeMatrix::isRobotNextToMinotaur() const {
    if (std::abs(robotX - minotaurX) <= 1 && std::abs(robotY - minotaurY) <= 1) {
        return true;
    }
    return false;
}

bool MazeMatrix::isGameLost() {
    // Proverava da li minotaur moze da ubije robota.
    if (isRobotNextToMinotaur() && isMinotaurAlive) {
        if (activePowerUp) {
            // Ako robot ima stit - ne moze da ga ubije minotaur.
            if (activePowerUp->getName() == "Stit") {
                return false;
            }
            // Ako robot ima mac - automatski ubija minotaura.
            if (activePowerUp->getName() == "Mac") {
                isMinotaurAlive = false;
                matrix[minotaurX][minotaurY] = '.';
                cout << "Ubili ste minotaura!" << endl;
                return false;
            }
        }
        return true;
    }
}

bool MazeMatrix::isGameWon() const {
    if (robotX == rows - 1) {
        return true;
    }
    return false;
}
