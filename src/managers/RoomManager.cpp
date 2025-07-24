#include "../../include/managers/RoomManager.h"
#include "../../include/serializers/FloorSerialization.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/entities/TileBody.h"
#include "../../include/entities/ItemBody.h"
#include "../../include/entities/EnemyBody.h"
#include "../../include/generators/ProceduralFloorGenerator.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "entities/BossBody.h"

namespace Manager {
    RoomManager::RoomManager(SDL_Renderer* renderer,
        EntityManager* entityManager,
        TileSet* tileSet,
        ItemManager* itemManager,
        EnemyManager* enemyManager,
        BossManager* bossManager)
        : renderer(renderer),
          entityManager(entityManager),
          tileSet(tileSet),
          itemManager(itemManager),
          enemyManager(enemyManager),
          bossManager(bossManager){}

void RoomManager::connectFloorRooms() {
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
        std::cerr << "Sala inicial não encontrada no andar " << this->floor.index << std::endl;
    }
}

const Map::Room* RoomManager::getRoomByPosition(int x, int y) const {
    for (const auto& room : this->floor.rooms) {
        if (room.x == x && room.y == y)
            return &room;
    }
    return nullptr;
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
                case Map::EEntityType::Boss: {
                    const auto* boss = bossManager->getBossById(e.id);
                    if (boss) {
                        textures()->Load(renderer, boss->getSpritePath(), boss->getSpritePath());

                        for (const auto& [_, path] : boss->getSpecialTexturesMap()) {
                            textures()->Load(renderer, path, path);
                        }
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
    std::cout << "Carregando do tipo: " << Map::roomTypeToString(room->type) << std::endl;
    if(!this->roomStates.contains(room->id)) {
        std::cout << "Criando estado da sala: " << room->id << std::endl;
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
    if (room->type == Map::ERoomType::Start){
        Vector4f playerVect = virtualRenderer()->mapToScreen(4, 4, 1, 1.1f);
        this->player = new Entities::PlayerBody(playerVect, this->entityManager, true, true);
    }
    std::cout << "Carregando Tiles da sala: " << room->id << std::endl;
    this->loadTiles(room); 
    std::cout << "Carregando Entidades da sala: " << room->id << std::endl;

    this->loadEntities(room);

    state.wasVisited = true;

}

void RoomManager::loadTiles(Map::Room* room) {
    if (!room || room->layout.empty() || room->layout[0].empty()) return;

    SDL_Texture* tileSheet = textures()->Get("tileset");
    int rows = static_cast<int>(room->layout.size());
    int cols = static_cast<int>(room->layout[0].size());

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int tileId = room->layout[row][col];
            const Tile* tile = this->tileSet->getTile(tileId);
            if (!tile) continue;

            Vector4f screenVect = virtualRenderer()->mapToScreen(col, row);
            auto tileBody = std::make_unique<Entities::TileBody>(
                screenVect,
                tileSheet,
                tile->solid
            );

            if (tileId == 7) {
                double angle = 0.0;
                SDL_RendererFlip flip = SDL_FLIP_NONE;

                if (row == 0) angle = 0.0;
                else if (row == rows - 1) { angle = 0.0; flip = SDL_FLIP_VERTICAL; }
                else if (col == 0) angle = 270.0;
                else if (col == cols - 1) angle = 90.0;

                tileBody->initFlippedStaticTile(tileSheet, tile->index, angle, flip);
            } else {
                tileBody->initStaticTile(tileSheet, tile->index);
            }

            tileBody->setTileId(tileId);
            tileBody->setTileData(tile);
            this->entityManager->add(std::move(tileBody));
        }
    }
}

void RoomManager::loadEntities(Map::Room* room) {
    bool hasEnemies = false;

    for (const auto& e : room->entities) {
            std::cout << "Carregando entidade: " << e.id << " do tipo: " << Map::entityTypeToString(e.type) << std::endl;

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

            case Map::EEntityType::Boss: {
                if (!this->bossManager) break;
                std::cout << "Carregando Boss: " << e.id << std::endl;
                const Enemies::BossData* bossData = this->bossManager->getBossById(e.id);
                if (bossData) {
                    Vector4f screenVect = virtualRenderer()->mapToScreen(e.x, e.y, 3.0f, 3.0f); // 96x96

                    auto boss = std::make_unique<Entities::BossBody>(
                        screenVect, *bossData, *this->entityManager
                    );
                    boss->setTexture(textures()->Get(bossData->getSpritePath()));
                    boss->setTarget(this->player);

                    this->entityManager->add(std::move(boss));
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
    if (!this->currentRoom) return;

    if (this->areAllEnemiesDefeated()) {
        this->openDoorsOfCurrentRoom();
    }
}

void RoomManager::updateVirutalRenderer(Map::Room* room) {
    if (!room || room->layout.empty() || room->layout[0].empty()) {
        std::cerr << "Layout da sala " << room->id << " está vazio.\n";
        return;
    }
    std::cout << "Tamanho do layout: " << room->layout.size() << "x" << room->layout[0].size() << std::endl;
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

void RoomManager::generateFloor(int index, int seed) {
    std::ifstream file("assets/data/rooms.json");
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir rooms.json\n";
        return;
    }

    json data;
    file >> data;

    std::vector<json> templates = data.at("rooms").get<std::vector<json>>();

    Generator::ProceduralFloorGenerator generator;
    this->floor = generator.generate(index, seed, templates, this->itemManager);

    std::cout << "Andar " << index << " gerado com sucesso!" << std::endl;
}

Manager::RoomManager::~RoomManager()
{
    delete this->player;
    this->player = nullptr;
}

}
