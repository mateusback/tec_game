#ifndef ITEM_TYPES_H
#define ITEM_TYPES_H

#include <stdexcept>
#include <unordered_map>

namespace Items {
	enum class EItemType {
		Passive, Active, Weapon
	};

	enum class EItemPool {
		Boss, Room, Chest
	};

	enum class EEffectTarget {
		AttackDamage,
		AttackSpeed,
		AttackRange,
		AttackDuration,
		FireRate,
		Defense,
		Health,
		MaxHealth
	};


	inline EItemType stringToItemType(const std::string& str) {
		if (str == "Passive") return EItemType::Passive;
		if (str == "Active") return EItemType::Active;
		if (str == "Weapon") return EItemType::Weapon;
		throw std::invalid_argument("Unknown item type: " + str);
	}

	inline EItemPool stringToItemPool(const std::string& str) {
		if (str == "Boss") return EItemPool::Boss;
		if (str == "Chest") return EItemPool::Chest;
		return EItemPool::Room;
	}

	inline EEffectTarget stringToEffectTarget(const std::string& str) {
		static const std::unordered_map<std::string, EEffectTarget> map = {
			{"AttackDamage", EEffectTarget::AttackDamage},
			{"AttackSpeed", EEffectTarget::AttackSpeed},
			{"AttackRange", EEffectTarget::AttackRange},
			{"AttackDuration", EEffectTarget::AttackDuration},
			{"FireRate", EEffectTarget::FireRate},
			{"Defense", EEffectTarget::Defense},
			{"Health", EEffectTarget::Health},
			{"MaxHealth", EEffectTarget::MaxHealth},
		};
	
		auto it = map.find(str);
		if (it == map.end())
			throw std::invalid_argument("Unknown target in effect: " + str);
		return it->second;
	}
}
#endif
