#include "MazeMatrix.h"
#include "MazeLayout.h"

#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <Windows.h>

using namespace std;

int main(int argc, char* argv[]) {

    int rows = stoi(argv[1]);
    int cols = stoi(argv[2]);
    if (rows < 15 || cols < 15) {
        cout << "Broj redova i kolona mora biti preko 15" << endl;
        Sleep(1000);
        return 1;
    }

    MazeMatrix maze(rows, cols);

    char move;
    bool gameRunning = true;

    cout << "UPUTSVO:\n - Koristiti strelice za kretanje\n - Pritisni Q za izlazak\n\n";

    while (gameRunning) {
        maze.display();

        // _getch() cita jedan karakter sa inputa.
        move = _getch();

        // Arrow keys vracaju specijalnu sifru (-32 ili 224) kao prvi karakter
        // i poseban broj za svaki pravac kao drugi karakter.
        if (move == -32 || move == 224) {
            move = _getch();
        }

        if (move == 'Q' || move == 'q') {
            gameRunning = false;
            continue;
        }

        if (!maze.moveRobot(move)) {
            cout << "\nNeispravan potez!" << endl;
            Sleep(500);

            // Input buffer se prazni tokom prikaza poruke da
            // korisnik ne bi mogao da tada unosi input.

            while (_kbhit()) {  // _kbhit() vraca true ako je neki key u input buffer-u.
                (void)_getch();
            }
        }

        system("cls");
        Sleep(4);

        if (maze.isGameWon()) {
            maze.display();
            cout << "\nCestitamo, pobedili ste!" << endl;
            Sleep(3000);
            gameRunning = false;
        }
    }

    return 0;
}
