#include "../../include/entities/TileBody.h"
#include "../../include/managers/AnimationLoader.h"

namespace Entities {
    void TileBody::initStaticTile(SDL_Texture* tileSheet, int sheetWidthPixels, int tileIndex, int tileSize) {
        auto sprite = Manager::AnimationLoader::loadSingleFrame(tileSheet, sheetWidthPixels, tileIndex, tileSize, tileSize);
        this->animationManager.addAnimation("default", Renderer::Animation({ sprite }, 0.0f, false));
        this->animationManager.setAnimation("default");
        this->is_animated = false;
    }
} 