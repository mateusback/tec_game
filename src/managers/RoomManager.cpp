#include "../../include/managers/RoomManager.h"
#include "../../include/serializers/FloorSerialization.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/entities/TileBody.h"
#include "../../include/entities/ItemBody.h"
#include "../../include/entities/EnemyBody.h"
#include <fstream>
#include <iostream>

namespace Manager {
    RoomManager::RoomManager(SDL_Renderer* renderer,
        EntityManager* entityManager,
        TileSet* tileSet,
        ItemManager* itemManager,
        EnemyManager* enemyManager)
        : renderer(renderer),
          entityManager(entityManager),
          tileSet(tileSet),
          itemManager(itemManager),
          enemyManager(enemyManager) {}

void RoomManager::loadFloor(int index) {
    std::string path = "assets/data/floor" + std::to_string(index) + ".json";
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir floor: " << path << std::endl;
        return;
    }

    nlohmann::json j;
    file >> j;
    from_json(j, this->floor);

    this->currentRoom = nullptr;
    for (auto& room : this->floor.rooms) {
        if (room.type == Map::ERoomType::Start) {
            this->currentRoom = &room;
            break;
        }
    }

    if (!this->currentRoom) {
        std::cerr << "Sala inicial não encontrada no andar " << index << std::endl;
    }
}

void RoomManager::loadRoom(Map::Room* room) {
    if (!room) return;

    this->currentRoom = room;
    this->updateLayout(room);
    this->entityManager->clearAll();
    if (room->type == Map::ERoomType::Start) {
        this->createPlayerInStartRoom();
    }
    
    this->player->setAcceleration(virtualRenderer()->normalizeValue(3));
    this->player->setScale(virtualRenderer()->normalizeVector({1.0f, 1.0f}));
    this->loadTiles(room);
    this->loadEntities(room);
}

void RoomManager::loadTiles(Map::Room* room) {
    SDL_Texture* tileSheet = textures()->Get("tileset");
    int tileSize = this->tileSet->getTileSize();
    int sheetWidthPixels = textures()->getTextureScale(tileSheet)[0];

    for (size_t row = 0; row < room->layout.size(); ++row) {
        for (size_t col = 0; col < room->layout[row].size(); ++col) {
            int tileId = room->layout[row][col];
            const Tile* tile = this->tileSet->getTile(tileId);
            if (!tile) continue;

            SDL_Rect screenRect = virtualRenderer()->tileToScreenRect(col, row);

            auto tileBody = std::make_unique<Entities::TileBody>(
                Vector4{
                    static_cast<float>(screenRect.x),
                    static_cast<float>(screenRect.y),
                    static_cast<float>(screenRect.w),
                    static_cast<float>(screenRect.h)
                },
                tileSheet,
                tile->solid
            );

            tileBody->initStaticTile(tileSheet, sheetWidthPixels, tile->index, tileSize);
            tileBody->setAnimated(false);
            tileBody->setTileId(tileId);
            tileBody->setTileData(tile);

            this->entityManager->add(std::move(tileBody));
        }
    }
}

void RoomManager::loadEntities(Map::Room* room) {
    for (const auto& e : room->entities) {
        std::string type = e.at("type");

        if (type == "Item" && this->itemManager) {
            int itemId = e.at("id");
            int x = e.at("x");
            int y = e.at("y");

            const Items::Item* itemData = this->itemManager->getItemById(itemId);
            if (itemData) {
                textures()->Load(this->renderer, itemData->getSpritePath(), itemData->getSpritePath());
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
                this->entityManager->add(std::move(item));
            }
        }

        if (type == "Enemy" && this->enemyManager) {
            int enemyId = e.at("id");
            int x = e.at("x");
            int y = e.at("y");
            std::cout << "Enemy ID: " << enemyId << std::endl;

            const Enemies::Enemy* enemyData = this->enemyManager->getEnemyById(enemyId);
            if (enemyData) {
                textures()->Load(this->renderer, enemyData->getSpritePath(), enemyData->getSpritePath());

                SDL_Rect screenRect = virtualRenderer()->tileToScreenRect(x, y);

                auto enemy = std::make_unique<Entities::EnemyBody>(
                    Vector4{
                        static_cast<float>(screenRect.x),
                        static_cast<float>(screenRect.y),
                        static_cast<float>(screenRect.w),
                        static_cast<float>(screenRect.h)
                    },
                    *enemyData,
                    * this->entityManager
                );

                enemy->setTexture(textures()->Get(enemyData->getSpritePath()));
                enemy->setTarget(this->player);

                this->entityManager->add(std::move(enemy));
                std::cout << "Enemy added: " << enemyData->getName() << std::endl;
            }
        }
    }
}

void RoomManager::createPlayerInStartRoom() {
    Vector4 playerVect = virtualRenderer()->mapToScreen(4, 4, 1, 1);
    this->player = new Entities::PlayerBody(playerVect, true, true);

    this->player->setAttackRate(1.0f);
    this->player->setAttackSpeed(3.5f);
    this->player->setTexture(Manager::TextureManager::Get("player_f"));
    this->player->loadAnimations();
    this->player->setAcceleration(virtualRenderer()->normalizeValue(3));
    this->player->setHitboxMargin(0.2f, 0.2f);
}

void RoomManager::updateLayout(Map::Room* room) {
    int tileCols = room->layout[0].size();
    int tileRows = room->layout.size();
    virtualRenderer()->updateLayout(tileCols, tileRows);
}

void RoomManager::loadRoomByType(Map::ERoomType type) {
    for (auto& room : this->floor.rooms) {
        if (room.type == type) {
            this->loadRoom(&room);
            break;
        }
    }
}

Map::Room* RoomManager::getCurrentRoom() {
    return this->currentRoom;
}

Map::Room* RoomManager::getRoomByPosition(int x, int y) {
    for (auto& room : this->floor.rooms) {
        if (room.x == x && room.y == y)
            return &room;
    }
    return nullptr;
}

const std::vector<Map::Room>& RoomManager::getRooms() {
    return this->floor.rooms;
}


void RoomManager::moveToRoomInDirection(Utils::EDirection direction) {
    int newX = this->currentRoom->x;
    int newY = this->currentRoom->y;

    switch (direction) {
        case Utils::EDirection::Right: newX += 1; break;
        case Utils::EDirection::Left:  newX -= 1; break;
        case Utils::EDirection::Up:    newY -= 1; break;
        case Utils::EDirection::Down:  newY += 1; break;
    }

    Map::Room* nextRoom = getRoomByPosition(newX, newY);
    if (nextRoom) {
        this->loadRoom(nextRoom);
    } else {
        std::cout << "Nenhuma sala conectada nessa direção.\n";
    }
}

void RoomManager::checkAndMovePlayerBetweenRooms() {
    if (!this->player || !this->currentRoom) return;

    Vector playerPos = this->player->getPosition();
    Vector playerSize = this->player->getScale();

    int tileSize = virtualRenderer()->getTileSize();
    int cols = this->currentRoom->layout[0].size();
    int rows = this->currentRoom->layout.size();

    float roomRight = tileSize * cols;
    float roomBottom = tileSize * rows;

    // DIREITA
    if (playerPos.x + playerSize.x >= roomRight - 1) {
        if (hasDoorOnBorder(Utils::EDirection::Right)) {
            this->moveToRoomInDirection(Utils::EDirection::Right);
            this->player->setPosition(virtualRenderer()->tileToScreenPosition(1, rows / 2));
        }
    }
    // ESQUERDA
    else if (playerPos.x <= 1) {
        if (hasDoorOnBorder(Utils::EDirection::Left)) {
            this->moveToRoomInDirection(Utils::EDirection::Left);
            this->player->setPosition(virtualRenderer()->tileToScreenPosition(cols - 2, rows / 2));
        }
    }
    // CIMA
    else if (playerPos.y <= 1) {
        if (hasDoorOnBorder(Utils::EDirection::Up)) {
            this->moveToRoomInDirection(Utils::EDirection::Up);
            this->player->setPosition(virtualRenderer()->tileToScreenPosition(cols / 2, rows - 2));
        }
    }
    // BAIXO
    else if (playerPos.y + playerSize.y >= roomBottom - 1) {
        if (hasDoorOnBorder(Utils::EDirection::Down)) {
            this->moveToRoomInDirection(Utils::EDirection::Down);
            this->player->setPosition(virtualRenderer()->tileToScreenPosition(cols / 2, 1));
        }
    }
}

bool RoomManager::hasDoorOnBorder(Utils::EDirection direction) const {
    const auto& layout = this->currentRoom->layout;

    switch (direction) {
        case Utils::EDirection::Right:
            for (const auto& row : layout)
                if (row.back() == 7) return true;
            break;
        case Utils::EDirection::Left:
            for (const auto& row : layout)
                if (row.front() == 7) return true;
            break;
        case Utils::EDirection::Up:
            for (std::size_t col = 0; col < layout[0].size(); ++col)
                if (layout[0][col] == 7) return true;
            break;
        case Utils::EDirection::Down:
            for (std::size_t col = 0; col < layout[0].size(); ++col)
                if (layout.back()[col] == 7) return true;
            break;
    }

    return false;
}

Manager::RoomManager::~RoomManager()
{
    delete this->player;
    this->player = nullptr;
}

}
