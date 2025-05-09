#ifndef ANIMATION_LOADER_H
#define ANIMATION_LOADER_H

#include <SDL2/SDL.h>
#include <vector>
#include <string>

#include "../renders/Sprite.h"
#include "../managers/AnimationManager.h"

namespace Manager {

    struct AnimationInfo {
        std::string name;
        int row;
        int frameCount;
        bool loop = true;
    };

    struct StaticAnimationInfo {
        std::string name;
        int row;
        int column;
    };

    class AnimationLoader {
    public:
        static constexpr int SPRITE_WIDTH  = 32;
        static constexpr int SPRITE_HEIGHT = 32;

        static std::vector<Renderer::Sprite> loadFromIndices(SDL_Texture* texture, const std::vector<int>& indices);
        static std::vector<Renderer::Sprite> loadRange(SDL_Texture* texture, int startFrame, int frameCount);
        static Renderer::Sprite loadSingleFrame(SDL_Texture* texture, int frameIndex);
        static void loadNamedAnimations(SDL_Texture* texture, const std::vector<AnimationInfo>& animations, AnimationManager& manager, float frameDuration = 0.15f);
        static void loadStaticAnimations(SDL_Texture* texture, const std::vector<StaticAnimationInfo>& animations, AnimationManager& manager);
        static int calculateStartIndex(SDL_Texture* texture, int row);
        
        private:
        static SDL_Rect computeSourceRect(SDL_Texture* texture, int frameIndex);        
    };
}

#endif
