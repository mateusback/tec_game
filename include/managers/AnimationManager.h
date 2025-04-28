#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include <string>
#include <my-lib/utils.h>
#include "renders/Animation.h"

namespace Manager {
    class AnimationManager {
    private:
        Mylib::unordered_map_string_key<Animation> animations;
        Animation* currentAnimation = nullptr;

    public:
        void addAnimation(const std::string_view name, const Animation& animation) {
            animations[name] = animation;
            if (!currentAnimation) {
                currentAnimation = &animations[name];
            }
        }

        void setAnimation(const std::string_view name) {
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
