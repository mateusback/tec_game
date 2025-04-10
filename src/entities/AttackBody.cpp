#include "../../include/entities/AttackBody.h"
#include <SDL2/SDL.h>

namespace Entities
{
    AttackBody::AttackBody(float x, float y, float w, float h, bool collision, bool visible, float dmg, float range, float duration, float lifesteal, float crit_chance, float crit_dmg)
        : MovingBody(x, y, w, h, collision, visible), 
          attack_damage(dmg), attack_range(range), attack_duration(duration),
          life_steal(lifesteal), critical_chance(crit_chance), critical_damage(crit_dmg) {}

    void AttackBody::update(float deltaTime)
    {
        this->move(deltaTime);
        attack_duration -= deltaTime;
    }

    void AttackBody::render(SDL_Renderer* renderer)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        SDL_Rect intRect = this->getIntRect();
        SDL_RenderFillRect(renderer, &intRect);
    }

    bool AttackBody::isExpired() const
    {
        return attack_duration <= 0.0f;
    }
}
