#ifndef WEAPON_H
#define WEAPON_H

#include <SDL2/SDL.h>
#include <memory>

#include "../managers/EntityManager.h"
#include "../entities/CharacterBody.h"

class Weapon {
public:
    virtual ~Weapon() = default;

    void setEntityManager(Manager::EntityManager* em) { this->entityManager = em; }

    virtual void attack(const Vector2f& direction) = 0;

    Entities::CharacterBody* getOwner() { return this->owner; }
    void setOwner(Entities::CharacterBody* owner) { 
        std::cout << "Weapon::setOwner" << std::endl;
        this->owner = owner; 
    }
    Manager::EntityManager* getEntityManager() { return this->entityManager; }

protected:
    Entities::CharacterBody* owner = nullptr;
    Manager::EntityManager* entityManager = nullptr;

    constexpr static float DEFAULT_BULLET_SIZE = 0.33f;
    constexpr static float DEFAULT_SWING_SIZE = 1.5f;
};

#endif
