#ifndef PROCEDURAL_FLOOR_GENERATOR_H
#define PROCEDURAL_FLOOR_GENERATOR_H

#include <nlohmann/json.hpp>
#include <vector>
#include <utility>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <string>

#include "../map/Room.h"
#include "../map/Floor.h"
#include "../utils/Types.h"

using json = nlohmann::json;
using Position = std::pair<int, int>;

namespace Generator {

    class ProceduralFloorGenerator {
    private:
        int floorIndex;
        int nextRoomId;
        std::mt19937 rng;
        std::unordered_map<Position, Map::Room> layout;
        std::unordered_set<int> usedRoomIds;

        void reset(int floorIndex, int seed);
        std::vector<json> loadAvailableRoomTemplates(const std::string& path);
        Map::Room chooseRoomByType(const std::vector<json>& rooms, Map::ERoomType type);
        void expandRooms(const std::vector<json>& templates, int targetCount);
        void connectRooms();
        void assignSpecialRooms(const std::vector<json>& templates);
        Map::Floor buildFloor();
        int calculateTargetRoomCount(int floorIndex);

    public:
        ProceduralFloorGenerator() = default;
        Map::Floor generate(int floorIndex, int seed, const std::vector<json>& roomTemplates);
    };

}

#endif
