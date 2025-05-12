#include "../../include/entities/TileBody.h"
#include "../../include/managers/AnimationLoader.h"

namespace Entities {
    void TileBody::initStaticTile(SDL_Texture* tileSheet, int tileIndex) {
        this->animationManager.clear();

        auto sprite = Manager::AnimationLoader::loadSingleFrame(tileSheet, tileIndex);
        std::vector<Renderer::Sprite> frames = { std::move(sprite) };

        Renderer::Animation anim(frames, 0.0f, false);
        this->animationManager.addAnimation("default", anim);
        this->animationManager.setAnimation("default");

        this->is_animated = false;
    }

    void TileBody::initFlippedStaticTile(SDL_Texture* tileSheet, int tileIndex, double angle, SDL_RendererFlip flip) {
        this->animationManager.clear();

        auto sprite = Manager::AnimationLoader::loadSingleFrame(tileSheet, tileIndex);

        sprite.setAngle(angle);
        sprite.setFlip(flip);

        std::vector<Renderer::Sprite> frames = { std::move(sprite) };

        Renderer::Animation anim(frames, 0.0f, false);
        this->animationManager.addAnimation("default", anim);
        this->animationManager.setAnimation("default");

        this->is_animated = false;
    }
 
} 