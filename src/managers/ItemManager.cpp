#include "../include/managers/ItemManager.h"
#include "../include/serializers/ItemSerialization.h"

#include <fstream>
#include <iostream>

namespace Manager {
    bool ItemManager::loadFromFile(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Erro ao abrir arquivo de itens: " << filePath << std::endl;
            std::cerr << "Erro do sistema: " << std::strerror(errno) << std::endl;
            return false;
        }
    
        nlohmann::json j;
        file >> j;
    
        for (const auto& itemJson : j) {
            Item item = itemJson.get<Item>();
            itemMap[item.getName()] = item;
        }
    
        return true;
    }
    
    const Item* ItemManager::getRandomItemFromPool(EItemPool pool) const {
        std::vector<const Item*> poolItems;
    
        for (const auto& [_, item] : itemMap) {
            if (item.getPool() == pool) {
                poolItems.push_back(&item);
            }
        }
    
        if (poolItems.empty()) return nullptr;
    
        int index = rand() % poolItems.size();
        return poolItems[index];
    }
    
    const Item* ItemManager::getItemById(int id) const {
        for (const auto& [_, item] : itemMap) {
            if (item.getId() == id) {
                return &item;
            }
        }
        return nullptr;
    }    
}
