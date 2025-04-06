#include "../include/managers/ItemManager.h"
#include "../include/managers/ItemSerialization.h"

#include <fstream>
#include <iostream>

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
