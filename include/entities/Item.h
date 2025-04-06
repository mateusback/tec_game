#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <vector>
#include "../managers/ItemEffect.h"

enum class EItemType {
    Passive, Active
};

enum class EItemPool {
    Boss, Room, Chest
};

class Item {
private:
    int id;
    EItemPool pool;
    std::string name;
    std::string description;
    int quality;
    EItemType type;
    std::vector<ItemEffect> effects;
	std::string spritePath;

public:
    Item(int id = -1, EItemPool pool = EItemPool::Room, std::string name = "", std::string description = "",
        int quality = 0, EItemType type = EItemType::Passive, std::vector<ItemEffect> effects = {}, std::string spritePath = "")
    : id(id), pool(pool), name(name), description(description), quality(quality),
        type(type), effects(effects), spritePath(spritePath) {}

    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    int getQuality() const { return quality; }
    EItemType getType() const { return type; }
    const std::vector<ItemEffect>& getEffects() const { return effects; }
	const std::string& getSpritePath() const { return spritePath; }
    int getId() const { return id; }
    EItemPool getPool() const { return pool; }
};

#endif
