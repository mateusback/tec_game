#include "include/core/Game.h"

int main() {
    Core::Game game;
    if (game.init("TEC_GAME", 640, 480)) {
        game.run();
    }
    return 0;
}