#ifndef TILESET_H
#define TILESET_H

#include "Tile.h"
#include <unordered_map>
#include <string>

class TileSet {
private:
    std::unordered_map<int, Tile> tileMap;

public:
    bool loadFromFile(const std::string& path);
    const Tile* getTile(int id) const;
    const std::unordered_map<int, Tile>& getAllTiles() const;
};

#endif
