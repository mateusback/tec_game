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

		std::vector<Entities::Body*> getEntitiesByType(Entities::EBodyType type);
		void add(std::unique_ptr<Entities::Entity> entity);
		void updateAll(float deltaTime);
		void renderAll(SDL_Renderer* renderer);
		void removeInactive();

		#pragma region Getters
		std::vector<std::unique_ptr<Entities::Entity>>& getEntities() { return this->entities; }
		#pragma endregion
	};
}

#endif
