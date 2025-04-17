#include "../../include/scenes/GameplayScene.h"
#include "../../include/physics/CollisionManager.h"
#include "../../include/managers/TextureManager.h"
#include "../../include/renders/TextRenderer.h"
#include "../../include/managers/FontManager.h"
#include "../../include/serializers/FloorSerialization.h"
#include "../../include/utils/DebugUtils.h"
#include <SDL2/SDL_image.h>
#include <fstream>


GameplayScene::GameplayScene(SDL_Renderer* renderer, int screenWidth, int screenHeight)
    : virtualRenderer(screenWidth, screenHeight, 1, 1) {
    Manager::TextureManager::Load(renderer, "player", "assets/player/personagem_S4.png");
    Manager::TextureManager::Load(renderer, "player_back", "assets/player/personagem_S2.png");
    Manager::TextureManager::Load(renderer, "player_left", "assets/player/personagem_S3.png");
    Manager::TextureManager::Load(renderer, "player_right", "assets/player/personagem_S1.png");
        
    Manager::TextureManager::Load(renderer, "player_with_item", "assets/player_with_item.png");
    Manager::TextureManager::Load(renderer, "attack", "assets/attack.png");
    Manager::TextureManager::Load(renderer, "enemy", "assets/enemies/shell.png");

    Manager::FontManager::load("default", "assets/fonts/Montserrat-Bold.ttf", 16);
    enemyManager.loadFromFile("assets/data/enemies.json");
    tileSet.loadFromFile("assets/data/tileset.json");
    itemManager.loadFromFile("assets/data/items.json");

    for (const auto& [id, tile] : tileSet.getAllTiles()) {
        Manager::TextureManager::Load(renderer, tile.spritePath, tile.spritePath);
    }

    loadFloor(1);
    loadCurrentRoom(renderer);
}

void GameplayScene::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        //TODO - Implement pause menu
    }
}

void GameplayScene::update(float deltaTime, const Manager::PlayerInput& input) {
    this->player->handleInput(input);

    if (input.shootDirection.x != 0 || input.shootDirection.y != 0)
    {
        auto attack = player->attack(player->getCenterPoint(), input.shootDirection);
        if(attack){
            attack->setTexture(Manager::TextureManager::Get("attack"));
            attack->setScale(virtualRenderer.getTileSize() /3 , virtualRenderer.getTileSize() /3);
            //TODO - guardar o objeto em uma lista e adicionar nos objetos ativos só no final do frame. 
            entityManager.add(std::move(attack));
        }
    }
    
    this->player->update(deltaTime);
    this->entityManager.updateAll(deltaTime);

    auto tiles = entityManager.getEntitiesByType<Entities::TileBody>();
    for (auto* tile : tiles) {
        if (tile->hasCollision() &&
            Physics::CollisionManager::checkCollision(player->getCollider(), tile->getCollider())) {
            player->onCollision(tile);
            break;
        }
    }
    
    auto items = entityManager.getEntitiesByType<Entities::ItemBody>();
    for (auto* item : items) {
        if (item->hasCollision() &&
            Physics::CollisionManager::checkCollision(player->getCollider(), item->getCollider())) {
                std::cout << "ItemBody count: " << items.size() << std::endl;
            player->onCollision(item);
        }
    }
    
    auto attacks = entityManager.getEntitiesByType<Entities::AttackBody>();
    for (auto* attack : attacks) {
        attack->update(deltaTime);
    }

    auto enemies = entityManager.getEntitiesByType<Entities::EnemyBody>();
    for (auto* enemy : enemies) {
        if (enemy->hasCollision() &&
            Physics::CollisionManager::checkCollision(player->getCollider(), enemy->getCollider())) {
            player->onCollision(enemy);
        }
    }

    this->entityManager.removeInactive();
}

void GameplayScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    //TODO - Isso foi um grande erro, lembrar de separar em camadas depois
    this->entityManager.renderAll(renderer);
    this->player->render(renderer);

    if (true) {
        Utils::DebugUtils::drawCollidersOfType<Entities::ItemBody>(renderer, entityManager, {0, 0, 255, 255});
        Utils::DebugUtils::drawCollidersOfType<Entities::TileBody>(renderer, entityManager, {0, 255, 0, 255});
        Utils::DebugUtils::drawCollidersOfType<Entities::AttackBody>(renderer, entityManager, {255, 0, 255, 255});
        Utils::DebugUtils::drawCollidersOfType<Entities::EnemyBody>(renderer, entityManager, {255, 255, 0, 255});
    
        if (player->hasCollision()) {
            SDL_FRect rect = {
                player->getCollider().x, player->getCollider().y,
                player->getCollider().w, player->getCollider().z
            };
            Utils::DebugUtils::drawCollider(renderer, rect, {255, 0, 0, 255});
        }
    }

    SDL_RenderPresent(renderer);
}

void GameplayScene::loadFloor(int index) {
    std::string path = "assets/data/floor" + std::to_string(index) + ".json";
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir floor: " << path << std::endl;
        return;
    }

    nlohmann::json j;
    file >> j;
    from_json(j, floor);

    for (auto& room : floor.rooms) {
        if (room.type == Map::ERoomType::Start) {
            currentRoom = &room;
            break;
        }
    }

    if (!currentRoom) {
        std::cerr << "Sala inicial não encontrada no andar " << index << std::endl;
    }
}

void GameplayScene::loadCurrentRoom(SDL_Renderer* renderer) {
    if (!currentRoom) return;

    int tileCols = currentRoom->layout[0].size();
    int tileRows = currentRoom->layout.size();

    this->virtualRenderer.updateLayout(tileCols, tileRows);

    for (int row = 0; row < tileRows; ++row) {
        for (int col = 0; col < tileCols; ++col) {
            int tileId = currentRoom->layout[row][col];
            const Tile* tile = tileSet.getTile(tileId);
            if (!tile) continue;

            SDL_Texture* texture = Manager::TextureManager::Get(tile->spritePath);
            SDL_Rect screenRect = virtualRenderer.tileToScreenRect(col, row);

            auto tileBody = std::make_unique<Entities::TileBody>(
                Vector4{
                    static_cast<float>(screenRect.x),
                    static_cast<float>(screenRect.y),
                    static_cast<float>(screenRect.w),
                    static_cast<float>(screenRect.h)
                },
                texture,
                tile->solid
            );

            this->entityManager.add(std::move(tileBody));
        }
    }

    for (const auto& e : currentRoom->entities) {
        std::string type = e.at("type");

        if (type == "Item") {
            int itemId = e.at("id");
            int x = e.at("x");
            int y = e.at("y");
        
            const Items::Item* itemData = itemManager.getItemById(itemId);
            if (itemData) {
                Manager::TextureManager::Load(renderer, itemData->getSpritePath(), itemData->getSpritePath());
        
                SDL_Rect screenRect = virtualRenderer.tileToScreenRect(x, y);
        
                auto item = std::make_unique<Entities::ItemBody>(
                    Vector4{
                        static_cast<float>(screenRect.x),
                        static_cast<float>(screenRect.y),
                        static_cast<float>(screenRect.w),
                        static_cast<float>(screenRect.h)
                    },
                    *itemData
                );
                item->setTexture(Manager::TextureManager::Get(itemData->getSpritePath()));
        
                this->entityManager.add(std::move(item));
            }
        }
        if (type == "Enemy") {
            std::cout << "Enemy" << std::endl;
            int enemyId = e.at("id");
            int x = e.at("x");
            int y = e.at("y");
            std::cout << "Enemy ID: " << enemyId << std::endl;
        
            const Enemies::Enemy* enemyData = enemyManager.getEnemyById(enemyId);
            std::cout << "Enemy Data: " << enemyData << std::endl;
            if (enemyData) {
                Manager::TextureManager::Load(renderer, enemyData->getSpritePath(), enemyData->getSpritePath());
                
                SDL_Rect screenRect = virtualRenderer.tileToScreenRect(x, y);
        
                auto enemy = std::make_unique<Entities::EnemyBody>(
                    Vector4{
                        static_cast<float>(screenRect.x),
                        static_cast<float>(screenRect.y),
                        static_cast<float>(screenRect.w),
                        static_cast<float>(screenRect.h)
                    },
                    *enemyData,
                    enemyData->getAcceleration()
                );
        
                enemy->setTexture(Manager::TextureManager::Get(enemyData->getSpritePath()));
                enemy->setTarget(this->player);
        
                this->entityManager.add(std::move(enemy));
                std::cout << "Enemy added: " << enemyData->getName() << std::endl;
            }
        }
    }
    
    SDL_Rect playerRect = virtualRenderer.tileToScreenRect(4, 4);
    player = new Entities::PlayerBody(
        static_cast<float>(playerRect.x),
        static_cast<float>(playerRect.y),
        static_cast<float>(playerRect.w * 1.5),
        static_cast<float>(playerRect.h * 1.5),
        true,
        true
    );
    player->setAttackRate(2.0f);
    player->setTexture(Manager::TextureManager::Get("player"));
    player->setAcceleration(100.0f + virtualRenderer.getTileSize());
    this->player = player;
}