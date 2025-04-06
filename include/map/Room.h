#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include <nlohmann/json.hpp>

enum class RoomType {
    Start,
    Normal,
    Treasure,
    Boss
};

struct Room {
    int id;
    int x, y;
    RoomType type;
    std::vector<nlohmann::json> entities;
    std::vector<std::vector<int>> layout;

    Room(int id = -1, int x = 0, int y = 0, RoomType type = RoomType::Normal)
        : id(id), x(x), y(y), type(type) {}
};

#endif
