#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "../entities/Entity.h"
#include "../entities/Body.h"
#include <vector>
#include <memory>
#include <algorithm>

namespace Manager {

	class EntityManager {
	private:
		std::vector<std::unique_ptr<Entities::Entity>> entities;
		std::vector<std::unique_ptr<Entities::Entity>> toAdd;

	public:
		void add(std::unique_ptr<Entities::Entity> entity);
		void updateAll(float deltaTime);
		void renderAll(SDL_Renderer* renderer);

		void addAll();
		void removeInactive();
		void clear() { this->entities.clear(); }
		void clearAll() { this->entities.clear(); this->toAdd.clear(); }

		#pragma region Getters
		std::vector<std::unique_ptr<Entities::Entity>>& getEntities() { return this->entities; }
		#pragma endregion

		#pragma region Templates
		template<typename T>
		std::vector<T*> getEntitiesByType() {
			std::vector<T*> filtered;
		
			for (auto& e : entities) {
				if (auto* casted = dynamic_cast<T*>(e.get())) {
					filtered.push_back(casted);
				}
			}
		
			return filtered;
		}
		#pragma endregion
	};
}

#endif
