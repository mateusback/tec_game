#ifndef EFFECT_BODY_H
#define EFFECT_BODY_H

#include "Body.h"
#include "../managers/AnimationLoader.h"

namespace Entities {
    class EffectBody : public Body {
    private:
        float duration;
        double angle = 0.0;
        SDL_RendererFlip flip = SDL_FLIP_NONE;

    public:
        EffectBody(Vector2f position, Vector2f scale, SDL_Texture* texture, float durationSeconds = 0.3f)
            : Body(position, scale, false, true), duration(durationSeconds) {
            this->setTexture(texture);
        }

        void loadAnimations() override;
        void render(SDL_Renderer* renderer) override;

        void update(float deltaTime) override {
            Body::update(deltaTime); 
            duration -= deltaTime;
            if (duration <= 0.f)
                this->setActive(false);
        }
        void setAngle(double a) { this->angle = a; }
        void setFlip(SDL_RendererFlip f) { this->flip = f; }
    };
}

#endif
