#ifndef FLOOR_SERIALIZATION_H
#define FLOOR_SERIALIZATION_H

#include "../map/Floor.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Map {

    inline void from_json(const json& j, EntityInstance& e) {
        e.type = entityTypeFromString(j.at("type").get<std::string>());
        e.id   = j.at("id").get<int>();
        e.x    = j.at("x").get<int>();
        e.y    = j.at("y").get<int>();
    }

    inline void from_json(const json& j, Room& room) {
        room.id   = j.at("id").get<int>();
        room.type = roomTypeFromString(j.at("type").get<std::string>());

        if (j.contains("layout"))
            room.layout = j.at("layout").get<std::vector<std::vector<int>>>();

        if (j.contains("entities"))
            room.entities = j.at("entities").get<std::vector<EntityInstance>>();

        if (j.contains("connections")) 
            room.connections = j.at("connections").get<Mylib::unordered_map_string_key<bool>>();
    }

    inline void from_json(const json& j, RoomInfo& info) {
        info.id = j.at("id").get<int>();
        info.x  = j.at("x").get<int>();
        info.y  = j.at("y").get<int>();
    }

    inline void from_json(const json& j, Floor& floor) {
        floor.index     = j.at("floor").get<int>();
        floor.roomInfos = j.at("rooms").get<std::vector<RoomInfo>>();
    }

}

#endif
