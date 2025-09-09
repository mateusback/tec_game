#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include "../items/Item.h"
#include <vector>
#include <my-lib/utils.h>
#include <string>
#include <random>

using namespace Items;

namespace Manager {
	class ItemManager {
	private:
		Mylib::unordered_map_string_key<Item> itemMap;

	public:
		bool loadFromFile(const std::string& filePath);

		const Item* getItem(const std::string& name) const {
			auto it = itemMap.find(name);
			return it != itemMap.end() ? &it->second : nullptr;
		}

		const Item* getItemById(int id) const;
		const Item* getRandomItemFromPool(EItemPool pool, std::mt19937& rng) const;

		#pragma region Getters
		const Mylib::unordered_map_string_key<Item>& getAllItems() const { return itemMap; }
		#pragma endregion
	};
}


#endif
