#include "MazeMatrix.h"
#include "MazeLayout.h"

#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <chrono>
#include <Windows.h>

using namespace std;

int main() {
    int rows;
    int cols;
    int numberOfPowerUps;

    cout << "Broj redova lavirinta: ";
    cin >> rows;

    cout << "Broj kolona lavirinta: ";
    cin >> cols;

    if (rows < 15 || cols < 15) {
        cout << "Broj redova i kolona mora biti preko 15";
        Sleep(1000);
        return 1;
    }

    cout << "Broj predmeta u lavirintu: ";
    cin >> numberOfPowerUps; cout << endl;

    // Meri koliko je vremena trebalo da se lavirint izgenerise.
    auto start = chrono::high_resolution_clock::now();
    MazeMatrix maze(rows, cols, numberOfPowerUps);
    auto end = chrono::high_resolution_clock::now();
    auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Lavirint kreiran za " << elapsed_time.count() << " milisekundi.\n\n";

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
        else {
            // Ako se robot pomerio, znaci da se pomera i minotaur.
            maze.moveMinotaur();
        }
        // Upisuje entite u lavirint.
        maze.updateEntities();
        system("cls");
        Sleep(4);

        if (maze.isGameLost()) {
            maze.display();
            cout << "\Izgubili ste!" << endl;
            Sleep(3000);
            gameRunning = false;
        }

        if (maze.isGameWon()) {
            maze.display();
            cout << "\Pobedili ste!" << endl;
            Sleep(3000);
            gameRunning = false;
        }
    }
    return 0;
}
