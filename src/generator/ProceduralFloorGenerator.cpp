#include "../../include/generators/ProceduralFloorGenerator.h"
#include "../../include/map/MapTypes.h"
#include "../../include/serializers/FloorSerialization.h"
#include "../../include/items/ItemTypes.h"
#include "../../include/utils/Types.h"
#include "../../include/items/Item.h"

#include <fstream>
#include <queue>
#include <algorithm>
#include <nlohmann/json.hpp>


namespace Generator {
    const std::array<int, 5> floorVariants = {0, 36, 37, 38, 39};
    const std::array<int, 8> rockVariants = {1, 40, 41, 42, 43, 44, 45, 46};

    void applyTileVariants(Map::Room& room, std::mt19937& rng) {
        std::uniform_int_distribution<int> dist(0, 100);
        for (auto& row : room.layout) {
            for (auto& tile : row) {
                if (tile == 0 && floorVariants.size() > 1) {
                    if (dist(rng) < 7) {
                        tile = floorVariants[1 + (dist(rng) % (floorVariants.size() - 1))];
                    } else {
                        tile = floorVariants[0];
                    }
                }

                else if (tile == 1 && rockVariants.size() > 1) {
                    if (dist(rng) < 20) {
                        tile = rockVariants[1 + (dist(rng) % (rockVariants.size() - 1))];
                    } else {
                        tile = rockVariants[0]; 
                    }
                }
            }
        }
    }

    using json = nlohmann::json;

    void ProceduralFloorGenerator::reset(int floorIndex, int seed) {
        this->floorIndex = floorIndex;
        this->nextRoomId = 1;
        this->layout.clear();
        this->usedRoomIds.clear();
        this->rng.seed(seed);
    }

    Map::Room ProceduralFloorGenerator::chooseRoomByType(const std::vector<json>& rooms, Map::ERoomType type) {
        std::vector<json> filtered;
        for (const auto& room : rooms) {
            if (Map::roomTypeFromString(room.at("type").get<std::string>()) == type) {
                filtered.push_back(room);
            }
        }
        if (filtered.empty()) throw std::runtime_error("Nenhuma sala do tipo requisitado encontrada");
        const json& selected = filtered[this->rng() % filtered.size()];

        Map::Room room = selected.get<Map::Room>();
        return room;
    }

    void ProceduralFloorGenerator::expandRooms(const std::vector<json>& templates, std::size_t targetCount) {
        std::vector<Position> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}};
        std::queue<Position> frontier;
        frontier.emplace(0, 0);

        while (!frontier.empty() && layout.size() < targetCount) {
            Position current = frontier.front(); frontier.pop();
            std::shuffle(directions.begin(), directions.end(), rng);

            for (const auto& [dx, dy] : directions) {
                Position neighbor{current.first + dx, current.second + dy};
                if (this->layout.find(neighbor) != this->layout.end()) continue;

                auto dir = directionFromDelta(dx, dy);
                std::string dirStr = directionToString(dir);
                std::string oppStr = directionToString(oppositeDirection(dir));

                Map::Room& currentRoom = this->layout[current];
                bool currentAllows = currentRoom.connections.contains(dirStr) && currentRoom.connections[dirStr];
                if (!currentAllows) continue;

                if (this->usedRoomIds.size() >= templates.size()) this->usedRoomIds.clear();

                for (int attempts = 0; attempts < 10; ++attempts) {
                    Map::Room candidate = chooseRoomByType(templates, Map::ERoomType::Normal);

                    if (!candidate.connections.contains(oppStr) || !candidate.connections[oppStr]) continue;

                    if (this->usedRoomIds.contains(candidate.id)) continue;

                    candidate.x = neighbor.first;
                    candidate.y = neighbor.second;
                    layout[neighbor] = candidate;
                    this->usedRoomIds.insert(candidate.id);
                    frontier.push(neighbor);
                    break;
                }

                if (this->layout.size() >= targetCount) break;
            }
        }
    }

    void ProceduralFloorGenerator::connectRooms() {
        const int normalDoorId = 7;
        const int fakeWallId   = 9;
        const int bossDoorId   = 24;

        auto placeEdge = [&](auto& grid, EDirection dir, int tileId) {
            const int rows = static_cast<int>(grid.size());
            const int cols = static_cast<int>(grid[0].size());
            const int midCol = cols / 2;
            const int midRow = rows / 2;
            const bool evenCols = (cols % 2 == 0);
            const bool evenRows = (rows % 2 == 0);

            switch (dir) {
                case EDirection::Up:
                    grid[0][midCol] = tileId;
                    if (evenCols) grid[0][midCol - 1] = tileId;
                    break;
                case EDirection::Down:
                    grid[rows - 1][midCol] = tileId;
                    if (evenCols) grid[rows - 1][midCol - 1] = tileId;
                    break;
                case EDirection::Left:
                    grid[midRow][0] = tileId;
                    if (evenRows) grid[midRow - 1][0] = tileId;
                    break;
                case EDirection::Right:
                    grid[midRow][cols - 1] = tileId;
                    if (evenRows) grid[midRow - 1][cols - 1] = tileId;
                    break;
            }
        };

        for (auto& [pos, room] : this->layout) {
            for (const auto& [dx, dy] : std::vector<Position>{{0,1},{1,0},{0,-1},{-1,0}}) {
                Position neighbor{pos.first + dx, pos.second + dy};
                if (!this->layout.contains(neighbor)) continue;

                auto& neighborRoom = this->layout[neighbor];
                auto dir = directionFromDelta(dx, dy);
                std::string dirStr = directionToString(dir);
                std::string oppStr = directionToString(oppositeDirection(dir));

                room.connections[dirStr] = true;
                neighborRoom.connections[oppStr] = true;

                switch (neighborRoom.type) {
                    case Map::ERoomType::Secret:
                        placeEdge(room.layout, dir, fakeWallId);
                        break;

                    case Map::ERoomType::Boss:
                        placeEdge(room.layout, dir, bossDoorId);
                        break;

                    default:
                        placeEdge(room.layout, dir, normalDoorId);
                        break;
                }
            }
        }
    }

    void ProceduralFloorGenerator::assignSpecialRooms(const std::vector<json>& templates) {
        std::vector<std::pair<Position, Map::Room*>> candidates;
        for (auto& [pos, room] : this->layout) {
            if (room.type == Map::ERoomType::Normal)
                candidates.emplace_back(pos, &room);
        }

        std::sort(candidates.begin(), candidates.end(), [](auto& a, auto& b) {
            auto da = std::abs(a.first.first) + std::abs(a.first.second);
            auto db = std::abs(b.first.first) + std::abs(b.first.second);
            return da > db;
        });

        if (candidates.size() >= 3) {
            auto replace = [&](Map::ERoomType type, std::pair<Position, Map::Room*>& target) {
                Map::Room newRoom = chooseRoomByType(templates, type);
                newRoom.id = target.second->id;
                newRoom.x = target.first.first;
                newRoom.y = target.first.second;
                *target.second = newRoom;

                if (type == Map::ERoomType::Item || type == Map::ERoomType::Secret) {
                    int rows = static_cast<int>(newRoom.layout.size());
                    int cols = static_cast<int>(newRoom.layout[0].size());
                    int centerX = cols / 2;
                    int centerY = rows / 2;


                    const Items::Item* randomItem = this->itemManager->getRandomItemFromPool(
                        type == Map::ERoomType::Item ? Items::EItemPool::Room : Items::EItemPool::Boss,
                        this->rng
                    );

                    std::cout << "Atribuindo item " << (randomItem ? randomItem->getName() : "nenhum")
                              << " na sala " << newRoom.id << " (" << centerX << ", " << centerY << ")\n";

                    if (randomItem != nullptr) {
                        json itemEntity;
                        itemEntity["type"] = "Item";
                        itemEntity["id"] = randomItem->getId();
                        itemEntity["x"] = centerX;
                        itemEntity["y"] = centerY;

                        newRoom.entities.push_back(itemEntity);
                    }
                }

                *target.second = newRoom;
            };

            replace(Map::ERoomType::Boss,   candidates[0]);
            replace(Map::ERoomType::Item,   candidates[1]);
            replace(Map::ERoomType::Secret, candidates[2]);
        }
    }

    Map::Floor ProceduralFloorGenerator::buildFloor() {
        Map::Floor floor;
        floor.index = this->floorIndex;
        for (const auto& [_, room] : layout) {
            floor.rooms.push_back(room);
            std::cout << "Sala " << room.id << " (" << room.x << ", " << room.y << ") adicionada ao andar " << floor.index << "\n";
            std::cout << "Tipo: " << Map::roomTypeToString(room.type) << "\n";
            floor.roomInfos.push_back(Map::RoomInfo{room.id, room.x, room.y});
        }
        return floor;
    }

    int ProceduralFloorGenerator::calculateTargetRoomCount(int floorIndex) {
        int baseRooms = static_cast<int>(3.33f * floorIndex) + 3;
        int extraRooms = (this->rng() % 2) + 5;
        return std::min(baseRooms + extraRooms, 20);
    }

    Map::Floor ProceduralFloorGenerator::generate(int floorIndex, int seed, const std::vector<json>& roomTemplates, const Manager::ItemManager* itemManager) {
        reset(floorIndex, seed);
        this->itemManager = itemManager;

        Map::Room start = chooseRoomByType(roomTemplates, Map::ERoomType::Start);
        start.id = nextRoomId++;
        start.x = 0;
        start.y = 0;
        this->layout[{0, 0}] = start;

        std::size_t targetRooms = calculateTargetRoomCount(floorIndex);
        expandRooms(roomTemplates, targetRooms);

        for (auto& [_, room] : this->layout) {
            applyTileVariants(room, this->rng);
        }

        assignSpecialRooms(roomTemplates);
        connectRooms();
        return buildFloor();
    }
} 