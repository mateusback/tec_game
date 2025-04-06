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

    std::filesystem::path base = std::filesystem::current_path();
    std::cout << "Diretório atual: " << std::filesystem::current_path() << std::endl;
    std::filesystem::path filePath = base / "assets" / "data" / "items.json";
    std::cout << "Diretório: " << filePath << std::endl;
    itemManager.loadFromFile(filePath.string());    

    const Item* espada = itemManager.getItem("Espada Sagrada");
    if (espada) {
        Core::TextureManager::Load(renderer, espada->getSpritePath(), espada->getSpritePath());
    
        auto item = std::make_unique<Entities::ItemBody>(
            SDL_FRect{200, 200, 32, 32}, *espada
        );
        std::printf(">rect.x: %c", espada->getSpritePath());
    
        item->setTexture(Core::TextureManager::Get(espada->getSpritePath()));
    
        std::unique_ptr<Entity> casted(static_cast<Entity*>(item.release()));
        entityManager.add(std::move(casted));
    }
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

    entityManager.updateAll(deltaTime);

    for (auto& e : entityManager.getEntities()) {
        auto* item = dynamic_cast<Entities::ItemBody*>(e.get());
        if (item && item->hasCollision() &&
            Physics::CollisionManager::checkCollision(player->getCollider(), item->getCollider())) {
            player->onCollision(item);
        }
    }

    entityManager.removeInactive();
}

void GameplayScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    player->render(renderer);
    
    entityManager.renderAll(renderer);

    SDL_RenderPresent(renderer);
}
