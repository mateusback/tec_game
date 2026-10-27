#include "../../include/entities/TileBody.h"
#include "../../include/managers/AnimationLoader.h"

namespace Entities {
    void TileBody::initTile(SDL_Texture* tileSheet, const Tile* tile, double angle, SDL_RendererFlip flip) {
        this->animationManager.clear();

        std::vector<int> indices = tile->frames.empty() ? std::vector<int>{ tile->index } : tile->frames;
        auto frames = Manager::AnimationLoader::loadFromIndices(tileSheet, indices);

        for (auto& sprite : frames) {
            sprite.setAngle(angle);
            sprite.setFlip(flip);
        }

        Renderer::Animation anim(frames, tile->frameTime, true);
        this->animationManager.addAnimation("default", anim);
        this->animationManager.setAnimation("default");

        this->is_animated = frames.size() > 1;

        // defasa pela posição para tiles iguais não animarem em sincronia
        if (this->is_animated) {
            Vector2f pos = this->getPosition();
            int phase = (static_cast<int>(pos.x) / 7 + static_cast<int>(pos.y) / 11) % static_cast<int>(frames.size());
            for (int i = 0; i < phase; ++i)
                this->animationManager.update(tile->frameTime);
        }
    }
}
