#ifndef ANIMATION_LOADER_H
#define ANIMATION_LOADER_H

#include <vector>
#include <SDL2/SDL.h>
#include "../renders/Sprite.h"

namespace Manager {
    class AnimationLoader {
    public:
        static std::vector<Renderer::Sprite> loadFromIndices(SDL_Texture* texture, int sheetWidthPixels, const std::vector<int>& indices, int tileWidth = 32, int tileHeight = 32);
        static std::vector<Renderer::Sprite> loadRange(SDL_Texture* texture, int sheetWidthPixels, int startFrame, int frameCount, int tileWidth = 32, int tileHeight = 32);
        static Renderer::Sprite loadSingleFrame(SDL_Texture* texture, int sheetWidthPixels, int frameIndex, int tileWidth = 32, int tileHeight = 32);
    };
}

#endif
