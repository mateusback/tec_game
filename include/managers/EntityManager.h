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

	public:
		void add(std::unique_ptr<Entities::Entity> entity);
		void updateAll(float deltaTime);
		void renderAll(SDL_Renderer* renderer);
		void removeInactive();

		#pragma region Getters
		std::vector<std::unique_ptr<Entities::Entity>>& getEntities() { return this->entities; }
		#pragma endregion

		#pragma region Templates
		template<typename T>
		std::vector<T*> getEntitiesByType(Entities::EBodyType type) {
			std::vector<T*> filtered;

			for (auto& e : entities) {
				auto* body = dynamic_cast<Entities::Body*>(e.get());
				if (body && body->getBodyType() == type) {
					if (auto* casted = dynamic_cast<T*>(body)) {
						filtered.push_back(casted);
					}
				}
			}

			return filtered;
		}
		#pragma endregion
	};
}

#endif
