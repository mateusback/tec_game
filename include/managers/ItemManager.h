#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include "../items/Item.h"
#include <vector>
#include <unordered_map>
#include <string>

namespace Manager {
	class ItemManager {
	private:
		//https://github.com/ehmcruz/my-game-lib/blob/main/include/my-game-lib/audio.h
		std::unordered_map<std::string, Item> itemMap;

	public:
		bool loadFromFile(const std::string& filePath);

		const Item* getItem(const std::string& name) const {
			auto it = itemMap.find(name);
			return it != itemMap.end() ? &it->second : nullptr;
		}

		const Item* getItemById(int id) const;
		const Item* getRandomItemFromPool(EItemPool pool) const;

		#pragma region Getters
		const std::unordered_map<std::string, Item>& getAllItems() const { return itemMap; }
		#pragma endregion
	};
}


#endif
