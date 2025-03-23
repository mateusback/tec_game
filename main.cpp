#include "src/Core/Interface/Game.h"

int main() {
    Game game;
    if (game.Init("TEC_GAME", 640, 480)) {
        game.Run();
    }
    return 0;
}