#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "Sprite.h"

namespace Renderer {
    class Animation {
    private:
        std::vector<Sprite> frames;
        float frameTime = 0.1f;
        float timer = 0.0f;
        size_t currentFrame = 0;
        bool loop = true;

    public:
        Animation(const std::vector<Sprite>& frames, float frameTime, bool loop = true)
            : frames(frames), frameTime(frameTime), loop(loop) {}
        
        Animation()
        : frames(),
            frameTime(0.0f),
            timer(0.0f),
            currentFrame(0),
            loop(false) {}
           

        void update(float deltaTime) {
            timer += deltaTime;
            if (timer >= frameTime && !frames.empty()) {
                timer -= frameTime;
                currentFrame++;
                if (currentFrame >= frames.size()) {
                    currentFrame = loop ? 0 : frames.size() - 1;
                }
            }
        }

        const Sprite& getCurrentSprite() const {
            return frames[currentFrame];
        }

        void reset() {
            currentFrame = 0;
            timer = 0.0f;
        }
    };
}

#endif
