#ifndef HUD_RENDERER_H
#define HUD_RENDERER_H

#include <SDL2/SDL.h>
#include "../entities/PlayerBody.h"
#include "../managers/SpriteSheetManager.h"



namespace Renderer {
    class HudRenderer {
    private:
        Manager::SpriteSheetManager hudIcons;
    public:
        HudRenderer(SDL_Renderer* renderer);
        void render(SDL_Renderer* renderer, const Entities::PlayerBody* player);
        void drawHearts(SDL_Renderer* renderer, int hp, int maxHp);
        void drawBombs(SDL_Renderer* renderer, int bombs);
        void drawCoins(SDL_Renderer* renderer, int coins);
    };
}

#endif
