#include "../../include/entities/AttackBody.h"
#include "../../include/managers/AnimationLoader.h"
#include <SDL2/SDL.h>

namespace Entities
{
void AttackBody::update(float deltaTime)
{
    this->animator.update(deltaTime);

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

    Manager::AnimationManager swingAnim;
    Manager::AnimationLoader::loadNamedAnimations(
        this->texture,
        {
            {"swing", 0, 30, false}
        },
        swingAnim,
        0.015f
    );

    this->animator.add_part(swingAnim, Vector2f(0.f, 0.f));
    this->animator.play("swing");
}
}