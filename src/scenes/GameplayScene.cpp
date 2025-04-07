#include "../../include/scenes/GameplayScene.h"
#include "../../include/physics/CollisionManager.h"
#include "../../include/core/TextureManager.h"
#include "../../include/core/TextRenderer.h"
#include "../../include/core/FontManager.h"
#include "../../include/map/FloorSerialization.h"
#include <SDL2/SDL_image.h>
#include <fstream>

GameplayScene::GameplayScene(SDL_Renderer* renderer) {
    Core::TextureManager::Load(renderer, "player", "assets/player.png");
    Core::TextureManager::Load(renderer, "player_with_item", "assets/player_with_item.png");
    Core::TextureManager::Load(renderer, "item", "assets/item.png");

    tileSet.loadFromFile("assets/data/tileset.json");
    itemManager.loadFromFile("assets/data/items.json");

    for (const auto& [id, tile] : tileSet.getAllTiles()) {
        Core::TextureManager::Load(renderer, tile.spritePath, tile.spritePath);
    }

    loadFloor(1);
    loadCurrentRoom(renderer);

    player = new Entities::PlayerBody(400, 400, 32, 32, true, true);
    player->setTexture(Core::TextureManager::Get("player"));  
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
    //TODO - Isso foi um grande erro, lembrar de separar em camadas depois
    entityManager.renderAll(renderer);
    player->render(renderer);

    //Isso é para #DEBUG, pode tirar
    if(false)
    {
        TTF_Font* font = Core::FontManager::get("default");
        
        int x = 10;
        int y = 100;
        int lineHeight = 18;
        
        auto drawStat = [&](const std::string& label, float value) {
            std::string text = label + ": " + std::to_string(value);
            Core::TextRenderer::render(renderer, font, text, x, y);
            y += lineHeight;
        };
        
        drawStat("HP", player->getHealth());
        drawStat("Max HP", player->getMaxHealth());
        drawStat("Atk Dmg", player->getAttackDamage());
        drawStat("Atk Spd", player->getAttackSpeed());
        drawStat("Atk Range", player->getAttackRange());
        drawStat("Atk Duration", player->getAttackDuration());
        drawStat("Fire Rate", player->getFireRate());
        drawStat("Defense", player->getDefense());
        drawStat("Level", static_cast<float>(player->getLevel()));
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
        if (room.type == RoomType::Start) {
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

            SDL_Texture* texture = Core::TextureManager::Get(tile->spritePath);
            SDL_FRect rect = { col * tileSize, row * tileSize, tileSize, tileSize };

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

            const Item* itemData = itemManager.getItemById(itemId);
            if (itemData) {
                Core::TextureManager::Load(renderer, itemData->getSpritePath(), itemData->getSpritePath());

                auto item = std::make_unique<Entities::ItemBody>(
                    SDL_FRect{x, y, 32, 32}, *itemData
                );
                item->setTexture(Core::TextureManager::Get(itemData->getSpritePath()));

                std::unique_ptr<Entity> casted(static_cast<Entity*>(item.release()));
                entityManager.add(std::move(casted));
            }
        }
    }
}