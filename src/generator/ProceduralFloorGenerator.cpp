#include "../../include/generators/ProceduralFloorGenerator.h"

namespace Generator {

    ProceduralFloorGenerator::ProceduralFloorGenerator(int seed, int roomCount, int floorLevel)
        : rng(seed), roomCount(roomCount), floorLevel(floorLevel) {
        if (roomCount <= 0) {
            throw std::invalid_argument("roomCount deve ser positivo");
        }
    }

    json ProceduralFloorGenerator::generate(const std::vector<json>& availableRooms,
                                            const std::vector<json>& itemPool) {
        std::vector<Position> directions = {
            {0, 1}, {1, 0}, {0, -1}, {-1, 0}
        };

        std::unordered_map<Position, json, pair_hash> layout;
        std::unordered_set<Position, pair_hash> used;
        Position current = {0, 0};
        used.insert(current);

        auto validRooms = this->filterRoomsByFloor(availableRooms);

        auto startRoom = this->chooseRoomByType(validRooms, "Start");
        startRoom["position"] = { {"x", current.first}, {"y", current.second} };
        layout[current] = startRoom;

        int secretRoomsPlaced = 0;
        bool itemRoomPlaced = false;
        bool bossRoomPlaced = false;

        while (layout.size() < static_cast<size_t>(this->roomCount)) {
            auto [dx, dy] = directions[this->rng() % directions.size()];
            Position next = {current.first + dx, current.second + dy};

            if (used.find(next) != used.end()) {
                current = *used.begin();
                continue;
            }

            json chosenRoom;
            if (!itemRoomPlaced) {
                chosenRoom = this->chooseRoomByType(validRooms, "Item");
                itemRoomPlaced = true;
            } else if (!bossRoomPlaced) {
                chosenRoom = this->chooseRoomByType(validRooms, "Boss");
                bossRoomPlaced = true;
            } else if (secretRoomsPlaced < 2) {
                chosenRoom = this->chooseRoomByType(validRooms, "Secret");
                secretRoomsPlaced++;
            } else {
                chosenRoom = this->chooseRoomByType(validRooms, "Normal");
            }

            chosenRoom["position"] = { {"x", next.first}, {"y", next.second} };
            layout[next] = chosenRoom;
            used.insert(next);
            current = next;
        }

        json result;
        result["floor"] = this->floorLevel;
        for (const auto& [pos, room] : layout) {
            json roomInfo = {
                {"id", room["id"]},
                {"x", room["position"]["x"]},
                {"y", room["position"]["y"]}
            };
            result["rooms"].push_back(roomInfo);
        }

        return result;
    }

    std::vector<json> ProceduralFloorGenerator::filterRoomsByFloor(const std::vector<json>& rooms) {
        std::vector<json> filtered;
        for (const auto& room : rooms) {
            int min = room.value("minFloor", 1);
            int max = room.value("maxFloor", 99);
            if (this->floorLevel >= min && this->floorLevel <= max) {
                filtered.push_back(room);
            }
        }
        return filtered;
    }

    json ProceduralFloorGenerator::chooseRoomByType(const std::vector<json>& rooms, const std::string& type) {
        std::vector<json> filtered;
        for (const auto& room : rooms) {
            if (room.value("type", "") == type) {
                filtered.push_back(room);
            }
        }
        if (filtered.empty()) {
            throw std::runtime_error("Nenhuma sala do tipo " + type + " encontrada");
        }
        return filtered[this->rng() % filtered.size()];
    }

    json ProceduralFloorGenerator::chooseRandomItemFromPool(const std::vector<json>& itemPool) {
        std::vector<json> filtered;
        for (const auto& item : itemPool) {
            if (item.value("pool", "") == "Room") {
                filtered.push_back(item);
            }
        }
        if (filtered.empty()) return nullptr;
        return filtered[this->rng() % filtered.size()];
    }

}