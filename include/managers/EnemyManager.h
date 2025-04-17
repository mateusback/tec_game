#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include <vector>
#include <my-lib/utils.h>
#include <string>
#include "../enemies/Enemy.h"

using namespace Enemies;

namespace Manager {
	class EnemyManager {
	private:
		Mylib::unordered_map_string_key<Enemy> enemyMap;

	public:
	bool loadFromFile(const std::string& filePath);

	const Enemy* getItem(const std::string& name) const {
		auto it = enemyMap.find(name);
		return it != enemyMap.end() ? &it->second : nullptr;
	}

	const Enemy* getEnemyById(int id) const;

	#pragma region Getters
	const Mylib::unordered_map_string_key<Enemy>& getAllEnemies() const { return enemyMap; }
	#pragma endregion
	};
}


#endif
