#ifndef ITEM_TYPES_H
#define ITEM_TYPES_H

namespace Items {
	enum class EItemType {
		Passive, Active
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
}
#endif
