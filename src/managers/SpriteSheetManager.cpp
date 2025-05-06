#include "../../include/managers/SpriteSheetManager.h"
#include "../../include/managers/AnimationLoader.h"

using namespace Manager;

void SpriteSheetManager::load(SDL_Texture* texture, const std::vector<int>& indices) {
    this->sprites = AnimationLoader::loadFromIndices(texture, indices);
}


const Renderer::Sprite& SpriteSheetManager::get(int index) const {
    return this->sprites.at(index);
}
