#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <vector>
#include "ItemEffect.h"

namespace Items {
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

		#pragma region Getters
		int getId() const { return this->id; }
		EItemPool getPool() const { return this->pool; }
		const std::string& getName() const { return this->name; }
		const std::string& getDescription() const { return this->description; }
		int getQuality() const { return this->quality; }
		EItemType getType() const { return this->type; }
		const std::vector<ItemEffect>& getEffects() const { return this->effects; }
		const std::string& getSpritePath() const { return this->spritePath; }
		#pragma endregion

		#pragma region Setters
		void setId(int id){ this->id = id }
		void setPool(EItemPool pool) { this->pool = pool }
		void setName(std::string name) { this->name = name }
		void setDescription(std::string description) { this->description = description}
		void setQuality(int quality) { this->quality = quality }
		void setType(EItemType type) {this->type = type }
		void setEffects(std::vector<ItemEffect> effects) { this->effects = effects}
		void setSpritePath(std::string spritePath) { this->spritePath = spritePath }
		#pragma endregion
	};
}
#endif
