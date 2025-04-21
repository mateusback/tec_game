#include "../include/managers/EntityManager.h"

namespace Manager
{
    void EntityManager::add(std::unique_ptr<Entities::Entity> entity) {
        this->toAdd.push_back(std::move(entity));
    }

    void EntityManager::addAll() {
        for (auto& e : this->toAdd) {
            if (e) {
                this->entities.push_back(std::move(e));
            }
        }
        this->toAdd.clear();
    }

    void EntityManager::updateAll(float deltaTime) {
        for (auto& e : entities) {
            auto* body = dynamic_cast<Entities::Body*>(e.get());
            if (body) {
                body->update(deltaTime);
            }
        }
    }

    void EntityManager::renderAll(SDL_Renderer* renderer) {
        for (auto& e : entities) {
            auto* body = dynamic_cast<Entities::Body*>(e.get());
            if (body && body->isVisible()) {
                body->render(renderer);
            }
        }
    }

    void EntityManager::removeInactive() {
        entities.erase(
            std::remove_if(entities.begin(), entities.end(), [](const std::unique_ptr<Entities::Entity>& e) {
                return e && !e->isActive();
            }),
            entities.end()
        );
    }
}