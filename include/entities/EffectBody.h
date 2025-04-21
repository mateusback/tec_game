// EffectBody.h
#ifndef EFFECT_BODY_H
#define EFFECT_BODY_H

#include "Body.h"

namespace Entities {
    class EffectBody : public Body {
    private:
        float duration;

    public:
        EffectBody(Vector position, Vector scale, SDL_Texture* texture, float durationSeconds = 0.3f)
            : Body(position.x, position.y, scale.x, scale.y, false, true),
              duration(durationSeconds) {
            this->setTexture(texture);
        }

        void update(float deltaTime) override {
            duration -= deltaTime;
            if (duration <= 0.f)
                this->setActive(false);
        }
    };
}

#endif
