#include "../../include/scenes/GameplayScene.h"
#include "../../include/physics/CollisionManager.h"
#include "../../include/managers/TextureManager.h"
#include "../../include/renders/TextRenderer.h"
#include "../../include/managers/FontManager.h"
#include "../../include/serializers/FloorSerialization.h"
#include "../../include/utils/DebugUtils.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/entities/EffectBody.h"
#include "../../include/entities/BombBody.h"
#include <SDL2/SDL_image.h>
#include <fstream>


GameplayScene::GameplayScene(SDL_Renderer* renderer, int screenWidth, int screenHeight) {
   textures()->Load(renderer, "player_b", "assets/player/personagem_B.png");
   textures()->Load(renderer, "player_f", "assets/player/personagem_F.png");
   textures()->Load(renderer, "player_l", "assets/player/personagem_L.png");
   textures()->Load(renderer, "player_r", "assets/player/personagem_R.png");
        
   textures()->Load(renderer, "player_with_item", "assets/player_with_item.png");
   textures()->Load(renderer, "attack", "assets/attack.png");
   textures()->Load(renderer, "attack_destroy", "assets/attack_fade.png");


    //todo, depois colocar isso na lista do inimigo
    textures()->Load(renderer, "shell_hidden", "assets/enemies/shell_hidden.png");
    textures()->Load(renderer, "bomb", "assets/bomb.png");
    textures()->Load(renderer, "bomb_explosion", "assets/bomb_explosion.png");
    textures()->Load(renderer, "rock_destroyed", "assets/tiles/rock-destroyed.png");

    Manager::FontManager::load("default", "assets/fonts/Montserrat-Bold.ttf", 16);
    enemyManager.loadFromFile("assets/data/enemies.json");
    tileSet.loadFromFile("assets/data/tileset.json");
    itemManager.loadFromFile("assets/data/items.json");

    for (const auto& [id, tile] : tileSet.getAllTiles()) {
       textures()->Load(renderer, tile.spritePath, tile.spritePath);
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
            entityManager.add(std::move(attack));
        }
    }

    //TODO - Depois passar para o player
    if (input.putBomb && player->getBombCooldown() <= 0.0f) {
        Vector pos = player->getPosition();
        auto bomb = std::make_unique<Entities::BombBody>(
            Vector4{
                pos.x,
                pos.y,
                virtualRenderer()->getTileSize(),
                virtualRenderer()->getTileSize()  
            },
            true,
            true,
            2.0f,
            virtualRenderer()->getTileSize() * 3.0f,
            60.0f,
            entityManager,
            tileSet
        );
        player->setBombCooldown(5.0f);
        bomb->setTexture(Manager::TextureManager::Get("bomb"));
        entityManager.add(std::move(bomb));
    }

    
    this->player->update(deltaTime);
    //this->entityManager.updateAll(deltaTime);

    auto items = entityManager.getEntitiesByType<Entities::ItemBody>();
    auto tiles = entityManager.getEntitiesByType<Entities::TileBody>();
    auto enemies = entityManager.getEntitiesByType<Entities::EnemyBody>();
    auto attacks = entityManager.getEntitiesByType<Entities::AttackBody>();
    auto effects = entityManager.getEntitiesByType<Entities::EffectBody>();
    auto bombs = entityManager.getEntitiesByType<Entities::BombBody>();
    Vector totalDisplacement = {0.f, 0.f};
    Vector4 currentHitbox = player->getHitbox();

    for (auto* bomb : bombs) {
        bomb->update(deltaTime);
    }

    for (auto* tile : tiles) {
        if (tile->hasCollision() &&
            Physics::CollisionManager::checkCollision(player->getHitbox(), tile->getHitbox())) {
            player->onCollision(tile);
            break;
        }
    }

    player->setPosition(player->getPosition().x + totalDisplacement.x, player->getPosition().y + totalDisplacement.y);
    
    for (auto* item : items) {
        if (item->hasCollision() &&
            Physics::CollisionManager::checkCollision(player->getHitbox(), item->getHitbox())) {
            player->onCollision(item);
        }
    }
    
    for (auto* attack : attacks) {
        attack->update(deltaTime);

        if (attack->getOrigin() != this->player && player->hasCollision() && attack->hasCollision() &&
        Physics::CollisionManager::checkCollision(attack->getHitbox(), player->getHitbox())) {
            player->takeDamage(attack->getAttackDamage());
            attack->setActive(false);

            addDestroyEffect(attack->getPosition(), attack->getScale());
            continue;
        }

        if (attack->getOrigin() == this->player) {
            for (auto* enemy : enemies) {
                if (enemy->hasCollision() && attack->hasCollision() &&
                    Physics::CollisionManager::checkCollision(enemy->getHitbox(), attack->getHitbox())) {
    
                    enemy->takeDamage(attack->getAttackDamage());
                    attack->setActive(false);
    
                    addDestroyEffect(attack->getPosition(), attack->getScale());
                    if (enemy->getHealth() <= 0)
                        enemy->setActive(false);
                    break;
                }
            }
        }

        for (auto* tile : tiles) {
            if (tile->hasCollision() &&
                Physics::CollisionManager::checkCollision(attack->getHitbox(), tile->getHitbox())) {
    
                attack->setActive(false);
                addDestroyEffect(attack->getPosition(), attack->getScale());
                break;
            }
        }
    }

    for (auto* enemy : enemies) {
        enemy->update(deltaTime);
    }

    for (auto* effect : effects) {
        effect->update(deltaTime);
    }

    this->entityManager.removeInactive();
    this->entityManager.addAll();
}

void GameplayScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    //TODO - Isso foi um grande erro, lembrar de separar em camadas depois
    this->entityManager.renderAll(renderer);
    this->player->render(renderer);

    auto enemies = entityManager.getEntitiesByType<Entities::EnemyBody>();
    for (auto* enemy : enemies) {
        SDL_FRect barBg, barFill;
    
        float healthPercent = enemy->getHealthPercent();
        Vector pos = enemy->getPosition();
        Vector size = enemy->getScale();
    
        barBg = {
            pos.x,
            pos.y - 6.f,
            size.x,
            5.f
        };
    
        barFill = {
            pos.x,
            pos.y - 6.f,
            size.x * healthPercent,
            5.f
        };
    
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRectF(renderer, &barBg);
    
        SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
        SDL_RenderFillRectF(renderer, &barFill);
    }

    if (true) {
        Utils::DebugUtils::drawCollidersOfType<Entities::ItemBody>(renderer, entityManager, {0, 0, 255, 255});
        Utils::DebugUtils::drawCollidersOfType<Entities::TileBody>(renderer, entityManager, {0, 255, 0, 255});
        Utils::DebugUtils::drawCollidersOfType<Entities::AttackBody>(renderer, entityManager, {255, 0, 255, 255});
        Utils::DebugUtils::drawCollidersOfType<Entities::EnemyBody>(renderer, entityManager, {255, 255, 0, 255});
    
        if (player->hasCollision()) {
            SDL_FRect rect = {
                player->getHitbox().x, player->getHitbox().y,
                player->getHitbox().w, player->getHitbox().z
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

    virtualRenderer()->updateLayout(tileCols, tileRows);

    for (int row = 0; row < tileRows; ++row) {
        for (int col = 0; col < tileCols; ++col) {
            int tileId = currentRoom->layout[row][col];
            const Tile* tile = tileSet.getTile(tileId);
            if (!tile) continue;

            SDL_Texture* texture = Manager::TextureManager::Get(tile->spritePath);
            SDL_Rect screenRect = virtualRenderer()->tileToScreenRect(col, row);

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
            tileBody->setTileId(tileId);
            tileBody->setTileData(tile);

            this->entityManager.add(std::move(tileBody));
        }
    }

    Vector4 playerVect = virtualRenderer()->mapToScreen(4, 4, 1, 1);
    player = new Entities::PlayerBody(
        playerVect,
        true,
        true
    );

    player->setAttackRate(1.0f);
    player->setAttackSpeed(3.5f);
    player->setTexture(Manager::TextureManager::Get("player_f"));
    player->setAcceleration(virtualRenderer()->normalizeValue(3));
    player->setHitboxMargin(0.2f, 0.2f);
    this->player = player;

    for (const auto& e : currentRoom->entities) {
        std::string type = e.at("type");

        if (type == "Item") {
            int itemId = e.at("id");
            int x = e.at("x");
            int y = e.at("y");
        
            const Items::Item* itemData = itemManager.getItemById(itemId);
            if (itemData) {
               textures()->Load(renderer, itemData->getSpritePath(), itemData->getSpritePath());
        
                SDL_Rect screenRect = virtualRenderer()->tileToScreenRect(x, y);
        
                auto item = std::make_unique<Entities::ItemBody>(
                    Vector4{
                        static_cast<float>(screenRect.x),
                        static_cast<float>(screenRect.y),
                        static_cast<float>(screenRect.w),
                        static_cast<float>(screenRect.h)
                    },
                    *itemData
                );
                item->setTexture(textures()->Get(itemData->getSpritePath()));
        
                this->entityManager.add(std::move(item));
            }
        }
        if (type == "Enemy") {
            int enemyId = e.at("id");
            int x = e.at("x");
            int y = e.at("y");
            std::cout << "Enemy ID: " << enemyId << std::endl;
        
            const Enemies::Enemy* enemyData = enemyManager.getEnemyById(enemyId);
            if (enemyData) {
               textures()->Load(renderer, enemyData->getSpritePath(), enemyData->getSpritePath());
                
                SDL_Rect screenRect = virtualRenderer()->tileToScreenRect(x, y);
        
                auto enemy = std::make_unique<Entities::EnemyBody>(
                    Vector4{
                        static_cast<float>(screenRect.x),
                        static_cast<float>(screenRect.y),
                        static_cast<float>(screenRect.w),
                        static_cast<float>(screenRect.h)
                    },
                    *enemyData,
                    this->entityManager
                );
        
                enemy->setTexture(textures()->Get(enemyData->getSpritePath()));
                enemy->setTarget(this->player);
        
                this->entityManager.add(std::move(enemy));
                std::cout << "Enemy added: " << enemyData->getName() << std::endl;
            }
        }
    }
}

void GameplayScene::addDestroyEffect(Vector position, Vector scale) {
    auto effect = std::make_unique<Entities::EffectBody>(
        position,
        scale,
        textures()->Get("attack_destroy"),
        0.2f
    );
    entityManager.add(std::move(effect));
}