#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <vector>
#include "../managers/ItemEffect.h"

enum class EItemType {
    Passive, Active
};

class Item {
private:
    std::string name;
    std::string description;
    int quality;
    EItemType type;
    std::vector<ItemEffect> effects;
	std::string spritePath;

public:
    Item(std::string name = "", std::string description = "", int quality = 0, EItemType type = EItemType::Passive,
         std::vector<ItemEffect> effects = {}, std::string spritePath = "")
        : name(name), description(description), quality(quality), type(type), effects(effects), spritePath(spritePath) {}

    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    int getQuality() const { return quality; }
    EItemType getType() const { return type; }
    const std::vector<ItemEffect>& getEffects() const { return effects; }
	const std::string& getSpritePath() const { return spritePath; }
};

#endif
