#include "src/Core/Interface/Game.h"

int main() {
    Core::Game game;
    if (game.Init("TEC_GAME", 640, 480)) {
        game.Run();
    }
    return 0;
}