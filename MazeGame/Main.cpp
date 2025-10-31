#include "GameLogic.h"
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        return 1;
    }

    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    int numberOfPowerUps = atoi(argv[3]);

    GameLogic::run(rows, cols, numberOfPowerUps);

    return 0;
}
