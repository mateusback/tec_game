#include "include/core/Game.h"
#include <iostream>

int main() {
    Core::Game game("TEC_GAME", 640, 480);
    game.run();
    return 0;
}