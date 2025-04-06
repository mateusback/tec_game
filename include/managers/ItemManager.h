#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include "../entities/Item.h"
#include <vector>
#include <unordered_map>
#include <string>

class ItemManager {
private:
    std::unordered_map<std::string, Item> itemMap;

public:
    bool loadFromFile(const std::string& filePath);

    const Item* getItem(const std::string& name) const {
        auto it = itemMap.find(name);
        return it != itemMap.end() ? &it->second : nullptr;
    }

    const std::unordered_map<std::string, Item>& getAllItems() const {
        return itemMap;
    }
};

#endif
