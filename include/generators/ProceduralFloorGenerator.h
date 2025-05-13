#ifndef PROCEDURAL_FLOOR_GENERATOR_H
#define PROCEDURAL_FLOOR_GENERATOR_H

#include <nlohmann/json.hpp>
#include <vector>
#include <utility>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <string>

using json = nlohmann::json;
using Position = std::pair<int, int>;

namespace Generator {

    class ProceduralFloorGenerator {
    public:
        ProceduralFloorGenerator(int seed, int roomCount = 8, int floorLevel = 1);

        json generate(const std::vector<json>& availableRooms,
                      const std::vector<json>& itemPool);

    private:
        std::mt19937 rng;
        int roomCount;
        int floorLevel;

        std::vector<json> filterRoomsByFloor(const std::vector<json>& rooms);
        json chooseRoomByType(const std::vector<json>& rooms, const std::string& type);
        json chooseRandomItemFromPool(const std::vector<json>& itemPool);

        struct pair_hash {
            std::size_t operator()(const std::pair<int, int>& p) const {
                return std::hash<int>()(p.first) ^ std::hash<int>()(p.second << 1);
            }
        };
    };

}

#endif
