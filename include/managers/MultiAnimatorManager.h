#ifndef MULTI_ANIMATOR_H
#define MULTI_ANIMATOR_H

#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include "AnimationManager.h"
#include "utils/Types.h"

class MultiAnimator {
private:
    struct Part {
        Manager::AnimationManager manager;
        Vector2f offset;

        Part(const Manager::AnimationManager& m, const Vector2f& o)
            : manager(m), offset(o) {}
    };

    std::vector<Part> parts;

public:
    struct SpriteInstance {
        const Renderer::Sprite* sprite;
        Vector2f offset;
    };

    void add_part(const Manager::AnimationManager& manager, const Vector2f& offset = Vector2f(0.f, 0.f)) {
        parts.emplace_back(manager, offset);
    }

    void play(const std::string& animationName, std::function<void()> onComplete = nullptr, bool force = false) {
        for (auto& part : parts) {
            part.manager.setAnimation(animationName, onComplete, force);
        }
    }

    void update(float dt) {
        for (auto& part : parts) {
            part.manager.update(dt);
        }
    }

    std::vector<SpriteInstance> getCurrentSprites() const {
        std::vector<SpriteInstance> result;

        for (const auto& part : parts) {
            const Renderer::Sprite* s = part.manager.getCurrentSprite();
            if (s) {
                result.push_back(SpriteInstance{ s, part.offset });
            }
        }

        return result;
    }

    void clear() {
        parts.clear();
    }
};

#endif
