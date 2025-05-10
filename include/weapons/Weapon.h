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

    virtual void attack(const Vector2f& origin, const Vector2f& direction) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;


    Entities::CharacterBody* getOwner() { return this->owner; }
    void setOwner(Entities::CharacterBody* owner) { 
        std::cout << "Weapon::setOwner" << std::endl;
        this->owner = owner; 
    }

protected:
    Entities::CharacterBody* owner = nullptr;
    Manager::EntityManager* entityManager = nullptr;
};

#endif
