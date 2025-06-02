#ifndef LASER_ATTACK_BODY_H
#define LASER_ATTACK_BODY_H

#include "Body.h"

namespace Entities {
    class LaserAttackBody : public Body {
    private:
        Vector2f direction;
        Entity* origin = nullptr;
        float damage;
        float currentLength;
        float damageTime = 0.0f;

        float timer = 0.0f;

    public:
        LaserAttackBody(Vector2f origin, Vector2f dir, float dmg, float damageTime = 0.5f);

        void update(float deltaTime) override;
        void render(SDL_Renderer* renderer) override;
        void loadAnimations() override;

        void setLength(float len) { this->currentLength = len; }
        float getLength() const { return this->currentLength; }
        void setDirection(const Vector2f& dir) { this->direction = dir; }
        Vector2f getDirection() const { return this->direction; }
        void setOrigin(Entity* entity) { this->origin = entity; }
        Entity* getOrigin() const { return this->origin; }
        void setDamage(float dmg) { this->damage = dmg; }
        float getDamage() const { return this->damage; }
        void setDamageTime(float time) { this->damageTime = time; }
        float getDamageTime() const { return this->damageTime; }
    };
};


#endif