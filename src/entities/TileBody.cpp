#include "../../include/entities/TileBody.h"
#include "../../include/managers/AnimationLoader.h"

namespace Entities {
    void TileBody::initStaticTile(SDL_Texture* texture, int tileIndex) {
        Renderer::Sprite sprite = Manager::AnimationLoader::loadSingleFrame(texture, tileIndex);
        Renderer::Animation anim({ sprite }, 0.15f, false);

        Manager::AnimationManager manager;
        manager.addAnimation("default", anim);
        
        this->animator.add_part(manager);
        this->animator.play("default");
    }

    void TileBody::initFlippedStaticTile(SDL_Texture* tileSheet, int tileIndex, double angle, SDL_RendererFlip flip) {
        this->animator.clear();

        Renderer::Sprite sprite = Manager::AnimationLoader::loadSingleFrame(tileSheet, tileIndex);
        sprite.setAngle(angle);
        sprite.setFlip(flip);

        std::vector<Renderer::Sprite> frames = { std::move(sprite) };
        Renderer::Animation anim(frames, 0.0f, false);

        Manager::AnimationManager manager;
        manager.addAnimation("default", anim);

        this->animator.add_part(manager);
        this->animator.play("default");

        this->is_animated = false;
    }
} 