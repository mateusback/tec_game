#ifndef SPRITESHEET_RENDERER_H
#define SPRITESHEET_RENDERER_H

#include <vector>
#include <SDL2/SDL.h>
#include "../renders/Sprite.h"

namespace Manager {
    class SpriteSheetManager {
    private:
        std::vector<Renderer::Sprite> icons;

    public:
        void load(SDL_Texture* texture, int sheetWidthPixels, const std::vector<int>& indices, int tileWidth = 32, int tileHeight = 32);
        const Renderer::Sprite& get(int index) const;
    };
}

#endif