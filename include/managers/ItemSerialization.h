#ifndef ITEM_SERIALIZATION_H
#define ITEM_SERIALIZATION_H

#include "../entities/Item.h"
#include <nlohmann/json.hpp>
#include <stdexcept>

using json = nlohmann::json;

inline void from_json(const json& j, ItemEffect& e) {
    std::string targetStr = j.at("target").get<std::string>();

    EEffectTarget target;
    if (targetStr == "AttackDamage") target = EEffectTarget::AttackDamage;
    else if (targetStr == "AttackSpeed") target = EEffectTarget::AttackSpeed;
    else if (targetStr == "AttackRange") target = EEffectTarget::AttackRange;
    else if (targetStr == "AttackDuration") target = EEffectTarget::AttackDuration;
    else if (targetStr == "FireRate") target = EEffectTarget::FireRate;
    else if (targetStr == "Defense") target = EEffectTarget::Defense;
    else if (targetStr == "Health") target = EEffectTarget::Health;
    else if (targetStr == "MaxHealth") target = EEffectTarget::MaxHealth;
    else throw std::invalid_argument("Unknown target in effect: " + targetStr);

    e = {target, j.at("value").get<float>()};
}

inline void from_json(const json& j, Item& item) {
    std::string typeStr = j.at("type").get<std::string>();
    std::string poolStr = j.at("pool").get<std::string>();

    EItemType type = (typeStr == "Active") ? EItemType::Active : EItemType::Passive;

    EItemPool pool;
    if (poolStr == "Boss") pool = EItemPool::Boss;
    else if (poolStr == "Chest") pool = EItemPool::Chest;
    else pool = EItemPool::Room;

    std::string spritePath = j.contains("sprite") ? j.at("sprite").get<std::string>() : "";

    item = Item{
        j.at("id").get<int>(),
        pool,
        j.at("name").get<std::string>(),
        j.at("description").get<std::string>(),
        j.at("quality").get<int>(),
        type,
        j.at("effects").get<std::vector<ItemEffect>>(),
        spritePath
    };
}

#endif
