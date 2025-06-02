// EffectBody.h
#ifndef EFFECT_BODY_H
#define EFFECT_BODY_H

#include "Body.h"

namespace Entities {
    class EffectBody : public Body {
    private:
        float duration;

    public:
        EffectBody(Vector2f position, Vector2f scale, SDL_Texture* texture, float durationSeconds = 0.3f)
            : Body(position, scale, false, true),
              duration(durationSeconds) {
            this->setTexture(texture);
        }

        void loadAnimations() override {
        }

        void update(float deltaTime) override {
            duration -= deltaTime;
            if (duration <= 0.f)
                this->setActive(false);
        }
    };
}

#endif
