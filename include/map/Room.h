#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include <nlohmann/json.hpp>
#include <my-lib/utils.h>

#include "MapTypes.h"

namespace Map {
    struct EntityInstance {
        EEntityType type;
        int id;
        int x, y;
    };

    struct Room {
        int id;
        int x, y;
        ERoomType type;
        std::vector<EntityInstance> entities;
        std::vector<std::vector<int>> layout;
        Mylib::unordered_map_string_key<bool> connections;
        bool doorsOpen = true;
    
        Room(int id = -1, int x = 0, int y = 0, ERoomType type = ERoomType::Normal)
            : id(id), x(x), y(y), type(type) {}
    };
}

#endif
