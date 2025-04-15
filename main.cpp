#include "include/core/Game.h"
#include <iostream>

int main() {
    Core::Game game("TEC_GAME", 1280, 720);
    game.run();
    return 0;
}