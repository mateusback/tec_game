#include "../../include/scenes/GameplayScene.h"
#include "../../include/physics/CollisionManager.h"
#include "../../include/managers/TextureManager.h"
#include "../../include/renders/TextRenderer.h"
#include "../../include/managers/FontManager.h"
#include "../../include/serializers/FloorSerialization.h"
#include <SDL2/SDL_image.h>
#include <fstream>
#include <iostream>

GameplayScene::GameplayScene(SDL_Renderer* renderer) {
    Manager::TextureManager::Load(renderer, "player", "assets/player.png");
    Manager::TextureManager::Load(renderer, "player_with_item", "assets/player_with_item.png");

    Manager::FontManager::load("default", "assets/fonts/Montserrat-Bold.ttf", 16);

    tileSet.loadFromFile("assets/data/tileset.json");
    itemManager.loadFromFile("assets/data/items.json");

    for (const auto& [id, tile] : tileSet.getAllTiles()) {
        Manager::TextureManager::Load(renderer, tile.spritePath, tile.spritePath);
    }

    loadFloor(1);
    loadCurrentRoom(renderer);

    player = new Entities::PlayerBody(400, 400, 32, 32, true, true);
    player->setTexture(Manager::TextureManager::Get("player"));
}

void GameplayScene::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        std::printf("TESTEEEEEEEE");
    }
}

void GameplayScene::update(float deltaTime, const Manager::PlayerInput& input) {
    player->handleInput(input);
    player->update(deltaTime);

    entityManager.updateAll(deltaTime);

    std::cout << "Iniciando busca de entidades" << std::endl;
    for (auto& e : entityManager.getEntities()) {
        auto* tile = dynamic_cast<Entities::TileBody*>(e.get());
        std::cout << "Tile: " << tile << std::endl;
        if (tile && tile->hasCollision() &&
            Physics::CollisionManager::checkCollision(player->getCollider(), tile->getCollider())) {
            std::cout << "Colidiu com o tile: " << tile->getCenterPoint() << std::endl;
            player->onCollision(tile);
        }
    }

    //TODO - ISSO AQUI TÁ QUEBRANDO TOTALMENTE O JOGO, AJUSTAR
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
    //TODO - Isso foi um grande erro, lembrar de separar em camadas depois
    entityManager.renderAll(renderer);
    player->render(renderer);

    if (debugMode) {
        for (auto& e : entityManager.getEntities()) {
            auto* body = dynamic_cast<Entities::Body*>(e.get());
            if (body->isActive()) {
                SDL_FRect rect = { body->getCollider().x, body->getCollider().y, body->getCollider().w, body->getCollider().z };
                drawCollider(renderer, rect);
            }
        }
        if (player->hasCollision()) {
            SDL_FRect rectp = { player->getCollider().x, player->getCollider().y, player->getCollider().w, player->getCollider().z };
            drawCollider(renderer, rectp);
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

    const int tileSize = 32;

    // TODO - 1. Quero uma sala com 14x9, 12x7 jogavel e o resto paredes e portas.
    for (int row = 0; row < currentRoom->layout.size(); ++row) {
        for (int col = 0; col < currentRoom->layout[row].size(); ++col) {
            int tileId = currentRoom->layout[row][col];
            const Tile* tile = tileSet.getTile(tileId);

            if (!tile) continue;

            SDL_Texture* texture = Manager::TextureManager::Get(tile->spritePath);
            Vector4 rect = { col * tileSize, row * tileSize, tileSize, tileSize };

            auto tileBody = std::make_unique<Entities::TileBody>(
                rect,
                texture,
                tile->solid
            );

            entityManager.add(std::move(tileBody));
        }
    }


    // TODO - 2. Separar em métodos depois, deixar mais organizado e legivel. 
    for (const auto& e : currentRoom->entities) {
        std::string type = e.at("type");

        if (type == "Item") {
            int itemId = e.at("item_id");
            float x = e.at("x");
            float y = e.at("y");

            const Items::Item* itemData = itemManager.getItemById(itemId);
            if (itemData) {
                Manager::TextureManager::Load(renderer, itemData->getSpritePath(), itemData->getSpritePath());

                auto item = std::make_unique<Entities::ItemBody>(
                    Vector4{x, y, 32, 32}, *itemData
                );
                item->setTexture(Manager::TextureManager::Get(itemData->getSpritePath()));

                std::unique_ptr<Entities::Entity> casted(static_cast<Entities::Entity*>(item.release()));
                entityManager.add(std::move(casted));
            }
        }
    }
}

void GameplayScene::drawCollider(SDL_Renderer* renderer, const SDL_FRect& rect) {
    SDL_Color color = {255, 0, 0, 255};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRectF(renderer, &rect);
}

