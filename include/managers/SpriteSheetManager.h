#ifndef SPRITESHEET_RENDERER_H
#define SPRITESHEET_RENDERER_H

#include <vector>
#include <SDL2/SDL.h>
#include "../renders/Sprite.h"

namespace Manager {
    class SpriteSheetManager {
    private:
        std::vector<Renderer::Sprite> sprites;

    public:
        void load(SDL_Texture* texture, const std::vector<int>& indices);
        const Renderer::Sprite& get(int index) const;
    };
}

#endif