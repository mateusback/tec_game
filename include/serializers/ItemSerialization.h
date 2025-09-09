#ifndef ITEM_SERIALIZATION_H
#define ITEM_SERIALIZATION_H

#include "../items/Item.h"
#include <nlohmann/json.hpp>
#include <stdexcept>

using json = nlohmann::json;

namespace Items {

    inline void from_json(const json& j, ItemEffect& e) {
        const std::string targetStr = j.at("target").get<std::string>();
        e = { stringToEffectTarget(targetStr), j.at("value").get<float>() };
    }
    
    inline void from_json(const json& j, Item& item) {
        const std::string typeStr = j.at("type").get<std::string>();
        const std::string poolStr = j.at("pool").get<std::string>();
    
        EItemType type = stringToItemType(typeStr);
        EItemPool pool = stringToItemPool(poolStr);

        std::string spritePath = j.contains("sprite") ? j.at("sprite").get<std::string>() : "";
        std::string weaponId = j.contains("weapon") ? j.at("weapon").get<std::string>() : "";
    
        item = Item{
            j.at("id").get<int>(),
            pool,
            j.at("name").get<std::string>(),
            j.at("description").get<std::string>(),
            j.at("quality").get<int>(),
            type,
            j.at("effects").get<std::vector<ItemEffect>>(),
            spritePath,
            weaponId
        };
    }
}
#endif
