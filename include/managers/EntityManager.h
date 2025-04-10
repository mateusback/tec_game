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
		void add(std::unique_ptr<Entity> entity) {
			entities.push_back(std::move(entity));
		}

		void updateAll(float deltaTime) {
			for (auto& e : entities) {
				e->update(deltaTime);
			}
		}

		void renderAll(SDL_Renderer* renderer) {
			for (auto& e : entities) {
				auto* body = dynamic_cast<Entities::Body*>(e.get());
				if (body && body->isVisible()) {
					e->render(renderer);
				}
			}
		}

		void removeInactive() {
			entities.erase(
				std::remove_if(entities.begin(), entities.end(), [](const std::unique_ptr<Entity>& e) {
					return e && !e->isActive();
				}),
				entities.end()
			);
		}

		#pragma region Getters
		std::vector<std::unique_ptr<Entity>>& getEntities() { this->entities; }
		#pragma endregion
	};
}

#endif
