#include "../../include/scenes/GameplayScene.h"
#include "../../include/physics/CollisionManager.h"
#include "../../include/core/TextureManager.h"
#include "../../include/core/TextRenderer.h"
#include "../../include/core/FontManager.h"
#include <SDL2/SDL_image.h>

GameplayScene::GameplayScene(SDL_Renderer* renderer) {
    Core::TextureManager::Load(renderer, "player", "assets/player.png");
    Core::TextureManager::Load(renderer, "player_with_item", "assets/player_with_item.png");
    Core::TextureManager::Load(renderer, "item", "assets/item.png");

    player = new Entities::PlayerBody(100, 100, 32, 32, true, true);
    player->setTexture(Core::TextureManager::Get("player"));

    Entities::ItemBody* item = new Entities::ItemBody({200, 200, 32, 32}, true, true, EItemPool::Floor, EItemType::Passive);
    item->setTexture(Core::TextureManager::Get("item"));
    items.push_back(item);
}

void GameplayScene::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        std::printf("TESTEEEEEEEE");
    }
}

void GameplayScene::update(float deltaTime) {

    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    player->handleInput(keys);
    player->update(deltaTime);

    for (auto it = items.begin(); it != items.end(); ) {
        if (Physics::CollisionManager::checkCollision(player->getCollider(), (*it)->getCollider())) {
            player->onCollision(*it);
            if (!(*it)->hasCollision()) {
                it = items.erase(it);
                continue;
            }
        }
        ++it;
    }
}

void GameplayScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    player->render(renderer);
    
    for (auto& item : items) {
        item->render(renderer);
    }

    SDL_RenderPresent(renderer);
}
