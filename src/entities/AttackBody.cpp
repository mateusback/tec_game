#include "../../include/entities/AttackBody.h"
#include "../../include/managers/AnimationLoader.h"
#include <SDL2/SDL.h>

namespace Entities
{
void AttackBody::update(float deltaTime)
{
    this->animationManager.update(deltaTime);

    this->attackDuration -= deltaTime;

    if (this->attackDuration <= 0.f)
    {
        this->setActive(false);
        return;
    }

    this->move(deltaTime);
}

void AttackBody::loadAnimations()
{
    if (!this->is_animated) return;
    Manager::AnimationLoader::loadNamedAnimations(this->texture, {
        {"swing", 0, 30, false}
    }, this->animationManager, 0.015f);
    this->animationManager.setAnimation("swing");
}
}