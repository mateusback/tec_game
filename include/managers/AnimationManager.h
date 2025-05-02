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
            animations.insert({std::string(name), animation});
            if (!currentAnimation) {
                currentAnimation = &animations[name];
            }
        }

        void setAnimation(const std::string& name) {
            if (animations.contains(name) && currentAnimation != &animations[name]) {
                currentAnimation = &animations[name];
                currentAnimation->reset();
            }
        }

        void update(float deltaTime) {
            if (currentAnimation) {
                currentAnimation->update(deltaTime);
            }
        }

        const Renderer::Sprite* getCurrentSprite() const {
            return currentAnimation ? &currentAnimation->getCurrentSprite() : nullptr;
        }
    };
}

#endif
