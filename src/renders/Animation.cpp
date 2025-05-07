#include "../../include/renders/Animation.h"
#include <iostream>

namespace Renderer
{
    void Animation::update(float deltaTime) {
        if (this->finished || this->frames.empty()) return;

        this->timer += deltaTime;
        if (this->timer >= this->frameTime) {
            this->timer -= this->frameTime;
            this->currentFrame++;

            if (this->currentFrame >=  this->frames.size()) {
                if (loop) {
                    this->currentFrame = 0;
                } else {
                    this->currentFrame =  this->frames.size() - 1;
                    this->finished = true;

                    auto callback = this->onComplete;
                    this->onComplete = nullptr;
                    if (callback) {
                        std::cout << "Animation completed!" << std::endl;
                        callback();
                    }
                }
            }
        }
    }
}
