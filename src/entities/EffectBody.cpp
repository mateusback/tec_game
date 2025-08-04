#include "../../include/entities/EffectBody.h"

namespace Entities
{

    void EffectBody::loadAnimations() {
        this->setAnimated(true);
        this->animationManager.clear();

        Manager::AnimationLoader::loadNamedAnimations(this->getTexture(), {
            { animInfo.name, animInfo.startFrame, animInfo.endFrame }
        }, this->animationManager, this->animInfo.frameDuration);

        this->animationManager.setAnimation(animInfo.name);
    }

    void EffectBody::render(SDL_Renderer* renderer) {
        if (!this->is_visible || !this->active) return;
        Vector4f fullSize = this->getFullSize();

        const Renderer::Sprite* sprite = this->animationManager.getCurrentSprite();
        if (sprite && sprite->getTexture()) {
            SDL_FRect dst = {
                position.x + sprite->getOffset().x,
                position.y + sprite->getOffset().y,
                scale.x,
                scale.y
            };

            SDL_RenderCopyExF(
            renderer,
            sprite->getTexture(),
            &sprite->getSourceRect(),
            &dst,
            this->angle,
            nullptr,
            this->flip
            );
            return;
        }

        if (this->texture) {
            SDL_FRect rect = { fullSize.x, fullSize.y, fullSize.z, fullSize.w };
            SDL_RenderCopyF(renderer, this->texture, nullptr, &rect);
        }
    }
}