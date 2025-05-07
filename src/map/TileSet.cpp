#include "../include/map/TileSet.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

bool TileSet::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    json j;
    file >> j;

    this->tileSize = j["tileSize"];
    this->spriteSheetPath = j["spriteSheet"];

    for (auto& [key, value] : j["tiles"].items()) {
        int id = std::stoi(key);
        Tile tile;
        tile.id = id;
        tile.name = value["name"].get<std::string>();
        tile.index = value["index"].get<int>();
        tile.solid = value.value("solid", false);
        tile.destructible = value.value("destructible", false);
        tile.destroyedId = value.value("destroyedId", -1);

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