#define SDL_MAIN_HANDLED
#include "include/core/Game.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Core::Game game("TEC_GAME", 1280, 720);
    game.run();
    return 0;
}