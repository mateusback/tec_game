#include "include/core/Game.h"

int main() {
    Core::Game game;
    if (game.Init("TEC_GAME", 640, 480)) {
        game.Run();
    }
    return 0;
}