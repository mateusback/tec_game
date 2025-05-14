#include "../../include/generators/ProceduralFloorGenerator.h"
#include "../../include/map/MapTypes.h"
#include "../../include/serializers/FloorSerialization.h"

#include <fstream>
#include <queue>
#include <algorithm>
#include <nlohmann/json.hpp>

namespace Generator {

    using json = nlohmann::json;

    void ProceduralFloorGenerator::reset(int floorIndex, int seed) {
        this->floorIndex = floorIndex;
        this->nextRoomId = 1;
        this->layout.clear();
        this->usedRoomIds.clear();
        this->rng.seed(seed);
    }

    std::vector<json> ProceduralFloorGenerator::loadAvailableRoomTemplates(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Erro ao abrir " + path);
        }
        json roomTemplatesJson;
        file >> roomTemplatesJson;
        return roomTemplatesJson.at("rooms").get<std::vector<json>>();
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

    void ProceduralFloorGenerator::expandRooms(const std::vector<json>& templates, int targetCount) {
        std::vector<Position> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}};
        std::queue<Position> frontier;
        frontier.push({0, 0});

        while (!frontier.empty() && static_cast<int>(layout.size()) < targetCount) {
            Position current = frontier.front(); frontier.pop();
            std::shuffle(directions.begin(), directions.end(), rng);

            for (const auto& [dx, dy] : directions) {
                Position neighbor{current.first + dx, current.second + dy};
                if (layout.find(neighbor) != layout.end()) continue;

                auto dir = directionFromDelta(dx, dy);
                std::string dirStr = directionToString(dir);
                std::string oppStr = directionToString(oppositeDirection(dir));

                Map::Room& currentRoom = layout[current];
                bool currentAllows = currentRoom.connections.contains(dirStr) && currentRoom.connections[dirStr];
                if (!currentAllows) continue;

                if (usedRoomIds.size() >= templates.size()) {
                    std::cout << "[WARN] Templates esgotados, permitindo repetição de salas.\n";
                    usedRoomIds.clear();
                }

                for (int attempts = 0; attempts < 10; ++attempts) {
                    Map::Room candidate = chooseRoomByType(templates, Map::ERoomType::Normal);

                    if (!candidate.connections.contains(oppStr) || !candidate.connections[oppStr])
                        continue;

                    if (usedRoomIds.contains(candidate.id))
                        continue;

                    candidate.x = neighbor.first;
                    candidate.y = neighbor.second;
                    layout[neighbor] = candidate;
                    usedRoomIds.insert(candidate.id);
                    frontier.push(neighbor);
                    break;
                }

                if (layout.size() >= targetCount) break;
            }
        }
    }

    void ProceduralFloorGenerator::connectRooms() {
        for (auto& [pos, room] : layout) {
            int rows = static_cast<int>(room.layout.size());
            int cols = static_cast<int>(room.layout[0].size());
            int midCol = cols / 2;
            int midRow = rows / 2;
            bool evenCols = (cols % 2 == 0);
            bool evenRows = (rows % 2 == 0);

            for (const auto& [dx, dy] : std::vector<Position>{{0,1},{1,0},{0,-1},{-1,0}}) {
                Position neighbor{pos.first + dx, pos.second + dy};
                if (!layout.contains(neighbor)) continue;

                auto& neighborRoom = layout[neighbor];
                auto dir = directionFromDelta(dx, dy);
                std::string dirStr = directionToString(dir);
                std::string oppStr = directionToString(oppositeDirection(dir));

                room.connections[dirStr] = true;
                neighborRoom.connections[oppStr] = true;

                if (neighborRoom.type != Map::ERoomType::Secret) {
                    switch (dir) {
                        case EDirection::Up:
                            room.layout[0][midCol] = 7;
                            if (evenCols) room.layout[0][midCol - 1] = 7;
                            break;
                        case EDirection::Down:
                            room.layout[rows - 1][midCol] = 7;
                            if (evenCols) room.layout[rows - 1][midCol - 1] = 7;
                            break;
                        case EDirection::Left:
                            room.layout[midRow][0] = 7;
                            if (evenRows) room.layout[midRow - 1][0] = 7;
                            break;
                        case EDirection::Right:
                            room.layout[midRow][cols - 1] = 7;
                            if (evenRows) room.layout[midRow - 1][cols - 1] = 7;
                            break;
                    }
                } else if (neighborRoom.type == Map::ERoomType::Secret) {
                    switch (dir) {
                        case EDirection::Up:
                            room.layout[0][midCol] = 1;
                            if (evenCols) room.layout[0][midCol - 1] = 1;
                            break;
                        case EDirection::Down:
                            room.layout[rows - 1][midCol] = 1;
                            if (evenCols) room.layout[rows - 1][midCol - 1] = 1;
                            break;
                        case EDirection::Left:
                            room.layout[midRow][0] = 1;
                            if (evenRows) room.layout[midRow - 1][0] = 1;
                            break;
                        case EDirection::Right:
                            room.layout[midRow][cols - 1] = 1;
                            if (evenRows) room.layout[midRow - 1][cols - 1] = 1;
                            break;
                    }
                }
            }
        }
    }

    void ProceduralFloorGenerator::assignSpecialRooms(const std::vector<json>& templates) {
        std::vector<std::pair<Position, Map::Room*>> candidates;
        for (auto& [pos, room] : layout) {
            if (room.type == Map::ERoomType::Normal)
                candidates.push_back({pos, &room});
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
            floor.roomInfos.push_back(Map::RoomInfo{room.id, room.x, room.y});
        }
        return floor;
    }

    int ProceduralFloorGenerator::calculateTargetRoomCount(int floorIndex) {
        int baseRooms = static_cast<int>(3.33f * floorIndex);
        int extraRooms = (this->rng() % 2) + 5;
        return std::min(baseRooms + extraRooms, 20);
    }

    Map::Floor ProceduralFloorGenerator::generate(int floorIndex, int seed, const std::vector<json>& roomTemplates) {
        reset(floorIndex, seed);

        Map::Room start = chooseRoomByType(roomTemplates, Map::ERoomType::Start);
        start.id = nextRoomId++;
        start.x = 0;
        start.y = 0;
        layout[{0, 0}] = start;

        int targetRooms = calculateTargetRoomCount(floorIndex);
        expandRooms(roomTemplates, targetRooms);
        assignSpecialRooms(roomTemplates);
        connectRooms();
        return buildFloor();
    }
} 