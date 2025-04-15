#include "../include/managers/EntityManager.h"

namespace Manager
{
    std::vector<Entities::Body*> EntityManager::getEntitiesByType(Entities::EBodyType type) {
        std::vector<Entities::Body*> filtered;
    
        for (auto& e : entities) {
            auto* body = dynamic_cast<Entities::Body*>(e.get());
            if (body && body->getBodyType() == type) {
                filtered.push_back(body);
            }
        }
        return filtered;
    }

    void EntityManager::add(std::unique_ptr<Entities::Entity> entity) {
        entities.push_back(std::move(entity));
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