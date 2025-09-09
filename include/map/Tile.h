#ifndef TILE_H
#define TILE_H

#include <string>

struct Tile {
    int id;
    std::string name;
    int index = 0;
    bool solid = false;
    bool destructible = false;
    int destroyedId = -1;
};

#endif
