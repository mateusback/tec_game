#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

namespace Managers {
    class AnimationManager {
        private:
        
        std::unordered_map<std::string, Animation*> animations;
    public:
        AnimationManager() = default;
        ~AnimationManager() = default;

        void addAnimation(const std::string& name, Animation* animation);
        void removeAnimation(const std::string& name);
        Animation* getAnimation(const std::string& name);
        std::vector<Sprite*> getSprites();

    }
}


std::vector<Sprite*> AnimationManager::getSprites() {
    std::vector<Sprite*> sprites;
    for (auto& animation : animations) {
        for (auto& sprite : animation.second->getSprites()) {
            sprites.push_back(sprite);
        }
    }
    return sprites;
}

#pragma endregion