#ifndef FLOOR_SERIALIZATION_H
#define FLOOR_SERIALIZATION_H

#include "Floor.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

inline void from_json(const json& j, Room& room) {
    std::string typeStr = j.at("type").get<std::string>();
    RoomType type;
    if (typeStr == "Start") type = RoomType::Start;
    else if (typeStr == "Treasure") type = RoomType::Treasure;
    else if (typeStr == "Boss") type = RoomType::Boss;
    else type = RoomType::Normal;

    room = Room{
        j.at("id").get<int>(),
        j.at("x").get<int>(),
        j.at("y").get<int>(),
        type
    };

    if (j.contains("entities")) {
        room.entities = j.at("entities").get<std::vector<json>>();
    }

    if (j.contains("layout")) {
        room.layout = j.at("layout").get<std::vector<std::vector<int>>>();
    }
}

inline void from_json(const nlohmann::json& j, Map::Floor& floor) {
    floor.index = j.at("floor").get<int>();
    floor.rooms = j.at("rooms").get<std::vector<Room>>();
}

#endif
