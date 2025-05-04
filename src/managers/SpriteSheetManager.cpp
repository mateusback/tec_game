#include "../../include/managers/SpriteSheetManager.h"
#include "../../include/managers/AnimationLoader.h"

using namespace Manager;

void SpriteSheetManager::load(SDL_Texture* texture, int sheetWidthPixels, const std::vector<int>& indices, int tileWidth, int tileHeight) {
    this->icons = AnimationLoader::loadFromIndices(texture, sheetWidthPixels, indices, tileWidth, tileHeight);
}

const Renderer::Sprite& SpriteSheetManager::get(int index) const {
    return this->icons.at(index);
}
