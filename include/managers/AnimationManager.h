#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include <string>
#include <my-lib/utils.h>
#include "renders/Animation.h"

namespace Manager {
    class AnimationManager {
    private:
        Mylib::unordered_map_string_key<Renderer::Animation> animations;
        Renderer::Animation* currentAnimation = nullptr;

    public:
    void addAnimation(const std::string& name, const Renderer::Animation& animation) {
        std::cout << "[AnimationManager] Adicionando animação: " << name << std::endl;
        animations.insert({std::string(name), animation});
    }
    
    void setAnimation(const std::string& name, std::function<void()> onComplete = nullptr, bool force = false) {
        if (!animations.contains(name)) {
            std::cout << "[AnimationManager] Animação não encontrada: " << name << std::endl;
            return;
        }
    
        if (force || currentAnimation != &animations[name]) {
            currentAnimation = &animations[name];
            currentAnimation->reset();
            currentAnimation->onComplete = onComplete;
        }
    }
    
    void update(float deltaTime) {
        if (this->currentAnimation) {
            this->currentAnimation->update(deltaTime);
        }
    }
    
    const Renderer::Sprite* getCurrentSprite() const {
        if (!currentAnimation) {
            return nullptr;
        }
        
        return &currentAnimation->getCurrentFrame();
    }

        std::vector<Renderer::Sprite> loadFromIndices(SDL_Texture* texture, int sheetWidthPixels, const std::vector<int>& indices, int tileWidth, int tileHeight);

        std::vector<Renderer::Sprite> loadRange(SDL_Texture* texture, int sheetWidthPixels, int startFrame, int frameCount, int tileWidth, int tileHeight);
    
        Renderer::Sprite loadSingleFrame(SDL_Texture* texture, int sheetWidthPixels, int frameIndex, int tileWidth, int tileHeight);

        std::vector<Renderer::Sprite> loadSquareTiles(SDL_Texture* texture, int sheetWidthPixels, const std::vector<int>& indices, int tileSize)
        {
            return loadFromIndices(texture, sheetWidthPixels, indices, tileSize, tileSize);
        }
        

    };
}

#endif
