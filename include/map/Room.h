#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include <nlohmann/json.hpp>
#include "MapTypes.h"

namespace Map {
    struct Room {
        int id;
        int x, y;
        ERoomType type;
        std::vector<nlohmann::json> entities;
        std::vector<std::vector<int>> layout;
        bool doorsOpen = true;
    
        Room(int id = -1, int x = 0, int y = 0, ERoomType type = ERoomType::Normal)
            : id(id), x(x), y(y), type(type) {}
    };
}

#endif
