#ifndef ROOMSTATE_H
#define ROOMSTATE_H

#include <vector>
#include <memory>
#include "../entities/TileBody.h"
#include "../entities/EnemyBody.h"
#include "../entities/ItemBody.h"

namespace Map {
    struct RoomState {
        Map::Room* roomRef;
        std::vector<std::unique_ptr<Entities::TileBody>> tiles;
        std::vector<std::unique_ptr<Entities::EnemyBody>> enemies;
        std::vector<std::unique_ptr<Entities::ItemBody>> items;
        bool wasVisited = false;
    };
}

#endif
