#ifndef TILE_H
#define TILE_H

#include <string>
#include <vector>

struct Tile {
    int id;
    std::string name;
    int index = 0;
    bool solid = false;
    bool destructible = false;
    int destroyedId = -1;
    std::vector<int> frames;   // índices da animação no tileset (vazio = estático)
    float frameTime = 0.15f;
};

#endif
