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
        std::string currentAnimationName;

    public:
    void addAnimation(const std::string& name, const Renderer::Animation& animation) {
        animations.insert({std::string(name), animation});
    }
    
    void setAnimation(const std::string& name, std::function<void()> onComplete = nullptr, bool force = false) {
        if (!animations.contains(name)) {
            std::cout << "[AnimationManager] Animação não encontrada: " << name << std::endl;
            return;
        }

        if (!force && currentAnimation == &animations[name]) {
            return;
        }

        this->currentAnimationName = name;
        this->currentAnimation = &animations[name];
        this->currentAnimation->reset();
        this->currentAnimation->onComplete = onComplete;
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

    void clear() {
        animations.clear();
        currentAnimation = nullptr;
    }

    void setFrameTime(const std::string& name, float frameTime) {
        if (animations.contains(name)) {
            animations[name].setFrameTime(frameTime);
        }
    }    
        
    const Renderer::Sprite& getFrame(const std::string& animationName, int index) const {
        return animations.at(animationName).getFrame(index);
    }
    
    const Renderer::Sprite& getFrame(int index) const {
        return currentAnimation->getFrame(index);
    }
    
     SDL_Texture* getTexture() const{
        return currentAnimation->getFrame(0).getTexture();
    }

    std::string getCurrentAnimationName() const {
        return currentAnimationName;
    }
    
    };
}

#endif
