#include "../../include/scenes/GameplayScene.h"
#include "../../include/physics/CollisionManager.h"
#include "../../include/managers/TextureManager.h"
#include "../../include/renders/TextRenderer.h"
#include "../../include/managers/FontManager.h"
#include "../../include/serializers/FloorSerialization.h"
#include <SDL2/SDL_image.h>
#include <fstream>
#include <iostream>

GameplayScene::GameplayScene(SDL_Renderer* renderer, int screenWidth, int screenHeight)
    : virtualRenderer(screenWidth, screenHeight, 1, 1) {
    Manager::TextureManager::Load(renderer, "player", "assets/player.png");
    Manager::TextureManager::Load(renderer, "player_with_item", "assets/player_with_item.png");
    Manager::TextureManager::Load(renderer, "attack", "assets/attack.png");

    Manager::FontManager::load("default", "assets/fonts/Montserrat-Bold.ttf", 16);

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
        std::printf("TESTEEEEEEEE");
    }
}

void GameplayScene::update(float deltaTime, const Manager::PlayerInput& input) {
    this->player->handleInput(input);

    if (input.shootDirection.x != 0 || input.shootDirection.y != 0)
    {
        auto attack = player->attack(player->getCenterPoint(), input.shootDirection);
        attack->setTexture(Manager::TextureManager::Get("attack"));
        attack->setScale(virtualRenderer.getTileSize() /3 , virtualRenderer.getTileSize() /3);
        if (attack)
            entityManager.add(std::move(attack));
    
        player->setFireTimer(player->getFireRate());
    }
    
    this->player->update(deltaTime);
    this->entityManager.updateAll(deltaTime);

    //TODO - melhorar esse templete, se eu estou passando o T com o tipo, não preciso do parametro de tipo
    auto tiles = entityManager.getEntitiesByType<Entities::TileBody>(Entities::EBodyType::Tile);
    for (auto* tile : tiles) {
        if (tile->hasCollision() &&
            Physics::CollisionManager::checkCollision(player->getCollider(), tile->getCollider())) {
            player->onCollision(tile);
            break;
        }
    }
    
    auto items = entityManager.getEntitiesByType<Entities::ItemBody>(Entities::EBodyType::Item);
    for (auto* item : items) {
        if (item->hasCollision() &&
            Physics::CollisionManager::checkCollision(player->getCollider(), item->getCollider())) {
            player->onCollision(item);
        }
    }
    
    auto attacks = entityManager.getEntitiesByType<Entities::AttackBody>(Entities::EBodyType::Attack);
    for (auto* attack : attacks) {
        attack->update(deltaTime);
    }

    this->entityManager.removeInactive();
}

void GameplayScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    //TODO - Isso foi um grande erro, lembrar de separar em camadas depois
    this->entityManager.renderAll(renderer);
    this->player->render(renderer);

    if (debugMode) {
        for (auto& e : entityManager.getEntities()) {
            auto* body = dynamic_cast<Entities::Body*>(e.get());
            if (body->isActive()) {
                SDL_Color color = {255, 255, 0, 255};
                SDL_FRect rect = { body->getCollider().x, body->getCollider().y, body->getCollider().w, body->getCollider().z };
                drawCollider(renderer, rect, color);
            }
        }
        if (player->hasCollision()) {
            SDL_FRect rectp = { player->getCollider().x, player->getCollider().y, player->getCollider().w, player->getCollider().z };
            SDL_Color colorp = {255, 0, 0, 255};
            drawCollider(renderer, rectp, colorp);
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
            int itemId = e.at("item_id");
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

                std::unique_ptr<Entities::Entity> casted(static_cast<Entities::Entity*>(item.release()));
                this->entityManager.add(std::move(casted));
            }
        }
    }
    
    SDL_Rect playerRect = virtualRenderer.tileToScreenRect(4, 4);
    player = new Entities::PlayerBody(
        static_cast<float>(playerRect.x),
        static_cast<float>(playerRect.y),
        static_cast<float>(playerRect.w),
        static_cast<float>(playerRect.h),
        true,
        true
    );
    player->setTexture(Manager::TextureManager::Get("player"));
    player->setAcceleration(100.0f + virtualRenderer.getTileSize());
    this->player = player;
}

void GameplayScene::drawCollider(SDL_Renderer* renderer, const SDL_FRect& rect, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRectF(renderer, &rect);
}

