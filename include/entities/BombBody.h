#ifndef BOMB_BODY_H
#define BOMB_BODY_H


#include "Body.h"
#include "../managers/EntityManager.h"
#include "../map/TileSet.h"

namespace Entities
{
    class BombBody : public Body
    {
    protected:
        float timeToExplode = 0.0f;
        float explosionRadius = 0.0f;
        float explosionDamage = 0.0f;
        Manager::EntityManager& entityManager;
        const TileSet& tileSet;
        
        public:
        BombBody(Vector4f rect,
            bool collision,
            bool visible,
            float time,
            float radius,
            float damage,
            Manager::EntityManager& entityManager,
            const TileSet& tileSet)
       : Body(rect, collision, visible),
         timeToExplode(time),
         explosionRadius(radius),
         explosionDamage(damage),
         entityManager(entityManager),
         tileSet(tileSet) {}
   
        virtual void update(float deltaTime) override;
        void explode();
        void addDestroyEffect(Vector2f position, Vector2f scale);
        
		#pragma region Getters
        float getTimeToExplode() const { return this->timeToExplode; }
        float getExplosionRadius() const { return this->explosionRadius; }
        float getExplosionDamage() const { return this->explosionDamage; }
		#pragma endregion

		#pragma region Setters
        void setTimeToExplode(float time) { this->timeToExplode = time; }
        void setExplosionRadius(float radius) { this->explosionRadius = radius; }
        void setExplosionDamage(float damage) { this->explosionDamage = damage; }
		#pragma endregion
    };
}

#endif
