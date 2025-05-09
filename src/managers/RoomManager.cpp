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
    std::string floorPath = "assets/data/floor" + std::to_string(index) + ".json";
    std::ifstream floorFile(floorPath);
    if (!floorFile.is_open()) {
        std::cerr << "Erro ao abrir floor: " << floorPath << std::endl;
        return;
    }

    nlohmann::json floorJson;
    floorFile >> floorJson;
    from_json(floorJson, this->floor);

    std::ifstream roomsFile("assets/data/rooms.json");
    if (!roomsFile.is_open()) {
        std::cerr << "Erro ao abrir rooms.json" << std::endl;
        return;
    }

    nlohmann::json roomsJson;
    roomsFile >> roomsJson;

    std::vector<Map::Room> allRooms = roomsJson.at("rooms").get<std::vector<Map::Room>>();

    this->floor.rooms.clear();
    for (const auto& info : this->floor.roomInfos) {
        auto it = std::find_if(allRooms.begin(), allRooms.end(), [&](const Map::Room& r) {
            return r.id == info.id;
        });

        if (it != allRooms.end()) {
            Map::Room room = *it;
            room.x = info.x;
            room.y = info.y;
            this->floor.rooms.push_back(std::move(room));
        } else {
            std::cerr << "Sala com ID " << info.id << " não encontrada no rooms.json\n";
        }
    }

    for (auto& room : this->floor.rooms) {
        if (room.type == Map::ERoomType::Secret) continue;

        auto setConn = [&](EDirection dir, int dx, int dy) {
            Map::Room* neighbor = getRoomByPosition(room.x + dx, room.y + dy);
            room.connections[directionToString(dir)] = neighbor && neighbor->type != Map::ERoomType::Secret;
        };

        setConn(EDirection::Right, 1, 0);
        setConn(EDirection::Left, -1, 0);
        setConn(EDirection::Up, 0, -1);
        setConn(EDirection::Down, 0, 1);
    }


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

void RoomManager::loadRequiredAssets(SDL_Renderer* renderer) {
    for (const auto& room : this->floor.rooms) {
        for (const auto& e : room.entities) {
            switch (e.type) {
                case Map::EEntityType::Enemy: {
                    const auto* enemy = enemyManager->getEnemyById(e.id);
                    if (enemy) {
                        const std::string& path = enemy->getSpritePath();
                        textures()->Load(renderer, path, path);
                    }
                    break;
                }
                case Map::EEntityType::Item: {
                    const auto* item = itemManager->getItemById(e.id);
                    if (item) {
                        const std::string& path = item->getSpritePath();
                        textures()->Load(renderer, path, path);
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void RoomManager::loadRoom(Map::Room* room) {
    if (!room) [[unlikely]] return;

    this->currentRoom = room;
    this->entityManager->clearAll();
    this->updateVirutalRenderer(room);

    if(!this->roomStates.contains(room->id)) {
        Map::RoomState newState;
        newState.roomRef = room;
        newState.wasVisited = false;
        roomStates[room->id] = std::move(newState);
    }

    Map::RoomState& state = roomStates[room->id];

    if (state.wasVisited) {
        for (auto& t : state.tiles) {
            const Tile* tileData = this->tileSet->getTile(t.id);
            Vector4f screenVect = virtualRenderer()->mapToScreen(t.position);
            auto tile = std::make_unique<Entities::TileBody>(screenVect, textures()->Get("tileset"), tileData->solid);
            tile->initStaticTile(textures()->Get("tileset"), tileData->index);
            tile->setTileId(t.id);
            tile->setTileData(tileData);
            this->entityManager->add(std::move(tile));
        }
    
        for (auto& e : state.enemies) {
            const Enemies::Enemy* enemyData = this->enemyManager->getEnemyById(e.id);
            Vector4f screenVect = virtualRenderer()->mapToScreen(e.position);
            auto enemy = std::make_unique<Entities::EnemyBody>(screenVect, *enemyData, *this->entityManager);
            enemy->setTexture(textures()->Get(enemyData->getSpritePath()));
            enemy->setHealth(e.health);
            enemy->setActive(e.isActive);
            enemy->setTarget(this->player);
            this->entityManager->add(std::move(enemy));
        }
    
        for (auto& i : state.items) {
            const Items::Item* itemData = this->itemManager->getItemById(i.id);
            Vector4f screenVect = virtualRenderer()->mapToScreen(i.position);
            auto item = std::make_unique<Entities::ItemBody>(screenVect, *itemData);
            item->setTexture(textures()->Get(itemData->getSpritePath()));
            item->setActive(i.isActive);
            this->entityManager->add(std::move(item));
        }
    
        this->currentRoom->doorsOpen = state.doorsOpened;
        return;
    }
    if (room->type == Map::ERoomType::Start)
        this->createPlayerInStartRoom();
    
    this->loadTiles(room); 
    this->loadEntities(room);

    state.wasVisited = true;

}

void RoomManager::loadTiles(Map::Room* room) {
    if (!room || room->layout.empty() || room->layout[0].empty()) return;

    SDL_Texture* tileSheet = textures()->Get("tileset");

    for (const auto& [dirStr, connected] : room->connections) {
        if (!connected) continue;

        EDirection dir = stringToDirection(dirStr);
        int rows = static_cast<int>(room->layout.size());
        int cols = static_cast<int>(room->layout[0].size());

        int dx = 0, dy = 0;
        switch (dir) {
            case EDirection::Up: dy = -1; break;
            case EDirection::Down: dy = 1; break;
            case EDirection::Left: dx = -1; break;
            case EDirection::Right: dx = 1; break;
            default: break;
        }

        Map::Room* neighbor = getRoomByPosition(room->x + dx, room->y + dy);
        if (!neighbor || neighbor->type == Map::ERoomType::Secret) continue;

        std::vector<Vector2i> doorPositions;

        switch (dir) {
            case EDirection::Up:
                if (cols % 2 == 0) {
                    doorPositions.push_back({cols / 2, 0});
                    doorPositions.push_back({(cols / 2) - 1, 0});
                } else {
                    doorPositions.push_back({cols / 2, 0});
                }
                break;
            case EDirection::Down:
                if (cols % 2 == 0) {
                    doorPositions.push_back({cols / 2, rows - 1});
                    doorPositions.push_back({(cols / 2) - 1, rows - 1});
                } else {
                    doorPositions.push_back({cols / 2, rows - 1});
                }
                break;
            case EDirection::Left:
                if (rows % 2 == 0) {
                    doorPositions.push_back({0, rows / 2});
                    doorPositions.push_back({0, (rows / 2) - 1});
                } else {
                    doorPositions.push_back({0, rows / 2});
                }
                break;
            case EDirection::Right:
                if (rows % 2 == 0) {
                    doorPositions.push_back({cols - 1, rows / 2});
                    doorPositions.push_back({cols - 1, (rows / 2) - 1});
                } else {
                    doorPositions.push_back({cols - 1, rows / 2});
                }
                break;
            default:
                break;
        }

        for (const auto& pos : doorPositions) {
            if (pos.y >= 0 && pos.y < rows && pos.x >= 0 && pos.x < cols) {
                int currentTileId = room->layout[pos.y][pos.x];
                const Tile* currentTile = this->tileSet->getTile(currentTileId);
                if (!currentTile || !currentTile->solid) continue; 
                room->layout[pos.y][pos.x] = 7;
            }
        }
    }

    for (size_t row = 0; row < room->layout.size(); ++row) {
        for (size_t col = 0; col < room->layout[row].size(); ++col) {
            int tileId = room->layout[row][col];
            const Tile* tile = this->tileSet->getTile(tileId);
            if (!tile) continue;

            Vector4f screenVect = virtualRenderer()->mapToScreen(col, row);

            auto tileBody = std::make_unique<Entities::TileBody>(
                screenVect,
                tileSheet,
                tile->solid
            );

            tileBody->initStaticTile(tileSheet, tile->index);
            tileBody->setTileId(tileId);
            tileBody->setTileData(tile);

            this->entityManager->add(std::move(tileBody));
        }
    }
}

void RoomManager::loadEntities(Map::Room* room) {
    bool hasEnemies = false;

    for (const auto& e : room->entities) {
        switch (e.type) {
            case Map::EEntityType::Item: {
                if (!this->itemManager) break;

                const Items::Item* itemData = this->itemManager->getItemById(e.id);
                if (itemData) {
                    Vector4f screenVect = virtualRenderer()->mapToScreen(e.x, e.y, 1.0f, 1.0f);
                    auto item = std::make_unique<Entities::ItemBody>(screenVect, *itemData);
                    item->setTexture(textures()->Get(itemData->getSpritePath()));
                    item->loadAnimations();
                    this->entityManager->add(std::move(item));
                }
                break;
            }

            case Map::EEntityType::Enemy: {
                if (!this->enemyManager) break;

                const Enemies::Enemy* enemyData = this->enemyManager->getEnemyById(e.id);
                if (enemyData) {
                    Vector4f screenVect = virtualRenderer()->mapToScreen(e.x, e.y);
                    auto enemy = std::make_unique<Entities::EnemyBody>(
                        screenVect, *enemyData, *this->entityManager
                    );
                    enemy->setTexture(textures()->Get(enemyData->getSpritePath()));
                    enemy->setTarget(this->player);
                    this->entityManager->add(std::move(enemy));
                    hasEnemies = true;
                }
                break;
            }

            default:
                break;
        }
    }

    room->doorsOpen = !hasEnemies;
}

void RoomManager::update(float deltaTime) {
    if (!this->currentRoom || this->currentRoom->doorsOpen) return;

    if (this->areAllEnemiesDefeated()) {
        std::cout << "Todas as ameaças eliminadas. Abrindo portas..." << std::endl;
        this->openDoorsOfCurrentRoom();
    }
}

void RoomManager::createPlayerInStartRoom() {
    Vector4f playerVect = virtualRenderer()->mapToScreen(4, 4, 1, 1);
    this->player = new Entities::PlayerBody(playerVect, true, true);

    this->player->setAttackRate(1.0f);
    this->player->setAttackSpeed(3.5f);
    this->player->setTexture(Manager::TextureManager::Get("player_f"));
    this->player->loadAnimations();
    this->player->setAcceleration(virtualRenderer()->normalizeValue(3));
    this->player->setHitboxMargin(0.2f, 0.2f);
    this->player->setBombs(3);
}

void RoomManager::updateVirutalRenderer(Map::Room* room) {
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

const Map::Room* RoomManager::getCurrentRoom() const{
    return this->currentRoom;
}

Map::Room* RoomManager::getRoomByPosition(int x, int y) {
    for (auto& room : this->floor.rooms) {
        if (room.x == x && room.y == y)
            return &room;
    }
    return nullptr;
}

const std::vector<Map::Room>& RoomManager::getRooms() const{
    return this->floor.rooms;
}

const bool RoomManager::wasRoomVisited(int roomId) const {
    auto it = roomStates.find(roomId);
    return it != roomStates.end() && it->second.wasVisited;
}


void RoomManager::moveToRoomInDirection(EDirection direction) {
    int newX = this->currentRoom->x;
    int newY = this->currentRoom->y;

    switch (direction) {
        case EDirection::Right: newX += 1; break;
        case EDirection::Left:  newX -= 1; break;
        case EDirection::Up:    newY -= 1; break;
        case EDirection::Down:  newY += 1; break;
    }

    Map::Room* nextRoom = getRoomByPosition(newX, newY);
    if (nextRoom) {
        this->saveCurrentRoomState();
        this->loadRoom(nextRoom);

        int cols = static_cast<int>(nextRoom->layout[0].size());
        int rows = static_cast<int>(nextRoom->layout.size());

        this->player->setScale(virtualRenderer()->normalizeVector({1, 1}));
        this->player->setAcceleration(virtualRenderer()->normalizeValue(3));

        switch (direction) {
            case EDirection::Right:
                this->player->setPosition(virtualRenderer()->tileToScreenPosition(1, rows / 2));
                break;
        
            case EDirection::Left:
                this->player->setPosition(virtualRenderer()->tileToScreenPosition(cols - 2, rows / 2));
                break;
        
            case EDirection::Up:
                this->player->setPosition(virtualRenderer()->tileToScreenPosition(cols / 2, rows - 2));
                break;
        
            case EDirection::Down:
                this->player->setPosition(virtualRenderer()->tileToScreenPosition(cols / 2, 1));
                break;
        }
        
    } else {
        std::cout << "Nenhuma sala conectada nessa direção.\n";
    }
}



void RoomManager::checkAndMovePlayerBetweenRooms() {
    if (!this->player || !this->currentRoom) return;

    Vector2f center = this->player->getCenterPoint();
    int tileSize = virtualRenderer()->getTileSize();
    
    int tileX = static_cast<int>(center.x / tileSize);
    int tileY = static_cast<int>(center.y / tileSize);

    int cols = static_cast<int>(this->currentRoom->layout[0].size());
    int rows = static_cast<int>(this->currentRoom->layout.size());

    if (tileX >= cols) {
        moveToRoomInDirection(EDirection::Right);
    }
    else if (tileX < 0) {
        moveToRoomInDirection(EDirection::Left);
    }
    else if (tileY < 0) {
        moveToRoomInDirection(EDirection::Up);
    }
    else if (tileY >= rows) {
        moveToRoomInDirection(EDirection::Down);
    }
}

bool RoomManager::areAllEnemiesDefeated() const {
    return !this->entityManager->hasAnyAliveEnemy(); 
}

void RoomManager::openDoorsOfCurrentRoom() {
    auto& layout = this->currentRoom->layout;

    // 1. Coletar posições das portas
    std::vector<Vector2i> portasFechadas;

    for (auto* tile : entityManager->getEntitiesByType<Entities::TileBody>()) {
        if (tile->getTileId() == 7) {
            portasFechadas.push_back(virtualRenderer()->screenToTilePosition(tile->getPosition()));
            tile->setActive(false);
        }
    }

    for (const auto& pos : portasFechadas) {
        layout[pos.y][pos.x] = 0;

        const Tile* tileData = this->tileSet->getTile(0);
        if (!tileData) continue;

        Vector4f screenPos = virtualRenderer()->mapToScreen(pos);
        auto tile = std::make_unique<Entities::TileBody>(screenPos, textures()->Get("tileset"), tileData->solid);
        tile->initStaticTile(textures()->Get("tileset"), tileData->index);
        tile->setTileId(0);
        tile->setTileData(tileData);

        this->entityManager->add(std::move(tile));
    }

    this->currentRoom->doorsOpen = true;
}

void RoomManager::saveCurrentRoomState() {
    if (!this->currentRoom) return;

    Map::RoomState& state = roomStates[this->currentRoom->id];
    state.wasVisited = true;
    state.tiles.clear();
    state.enemies.clear();
    state.items.clear();

    for (auto* tile : entityManager->getEntitiesByType<Entities::TileBody>()) {
        if (!tile->isActive()) continue;
        state.tiles.push_back({
            tile->getTileId(),
            virtualRenderer()->screenToTilePosition(tile->getPosition()),
            tile->getTileData()->solid
        });
    }

    for (auto* enemy : entityManager->getEntitiesByType<Entities::EnemyBody>()) {
        if (!enemy->isActive()) continue;
        state.enemies.push_back({
            enemy->getEnemyData().getId(),
            virtualRenderer()->screenToTilePosition(enemy->getPosition()),
            static_cast<int>(enemy->getHealth()),
            enemy->isDead()
        });
    }

    for (auto* item : entityManager->getEntitiesByType<Entities::ItemBody>()) {
        if (!item->isActive()) continue;
        state.items.push_back({
            item->getItem().getId(),
            virtualRenderer()->screenToTilePosition(item->getPosition()),
            item->isActive()
        });
    }

    state.doorsOpened = this->currentRoom->doorsOpen;
}

#pragma endregion

void RoomManager::setEntityPositionByPixels(Entities::Body* entity, Vector2f position) {
    entity->setPosition(virtualRenderer()->normalizeVector(position));
}

void RoomManager::setEntityPositionByTiles(Entities::Body* entity, Vector2f position) {
    entity->setPosition(virtualRenderer()->denormalizeVector(position));
}

Manager::RoomManager::~RoomManager()
{
    delete this->player;
    this->player = nullptr;
}

}
