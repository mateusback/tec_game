#include "../include/map/TileSet.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

bool TileSet::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    json j;
    file >> j;

    for (auto& [key, value] : j["tiles"].items()) {
        int id = std::stoi(key);
        Tile tile;
        tile.id = id;
        tile.name = value["name"];
        tile.spritePath = value["sprite"];
        tile.solid = value.contains("solid") && value["solid"].get<bool>();
        tileMap[id] = tile;
    }

    return true;
}

const Tile* TileSet::getTile(int id) const {
    auto it = tileMap.find(id);
    return it != tileMap.end() ? &it->second : nullptr;
}

const std::unordered_map<int, Tile>& TileSet::getAllTiles() const {
    return tileMap;
}