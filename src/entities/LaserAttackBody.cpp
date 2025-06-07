#include "../../include/entities/LaserAttackBody.h"


namespace Entities {

LaserAttackBody::LaserAttackBody(Vector2f origin, Vector2f dir, float dmg, float damageTime)
    : Body(origin, Vector2f(0, 0), false, true),
      direction(dir), damage(dmg), damageTime(damageTime), currentLength(0.0f) {
        this->loadAnimations();
      }

void LaserAttackBody::update(float deltaTime)
{
    this->animator.update(deltaTime);
}

void LaserAttackBody::render(SDL_Renderer* renderer)
{
 
}

void LaserAttackBody::loadAnimations() {

}


};