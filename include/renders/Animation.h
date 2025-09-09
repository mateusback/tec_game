#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "Sprite.h"
#include <functional>

namespace Renderer {

    struct AnimationInformation {
        std::string name;
        int startFrame;
        int endFrame;
        float frameDuration = 0.15f;
    };

    class Animation {
    private:
        std::vector<Sprite> frames;
        float frameTime = 0.1f;
        float timer = 0.0f;
        size_t currentFrame = 0;
        bool loop = true;
        bool finished = false;
        
    public:
        std::function<void()> onComplete = nullptr;
        //functor

        Animation(const std::vector<Sprite>& frames, float frameTime, bool loop = true)
            : frames(frames), frameTime(frameTime), loop(loop), finished(false) {}
        
        Animation()
        : frames(),
            frameTime(0.1f),
            timer(0.0f),
            currentFrame(0),
            loop(false),
            finished(false) {}
           
        void update(float deltaTime);

        const Sprite& getCurrentFrame() const {
            return this->frames[this->currentFrame];
        }

        void reset() {
            currentFrame = 0;
            timer = 0.0f;
            finished = false;
        }

        const Sprite& getFrame(int index) const {
            return frames.at(index);
        }
        
        void setFrameTime(float newFrameTime) {
            this->frameTime = newFrameTime;
        }
    };
}

#endif
