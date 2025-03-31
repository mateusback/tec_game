#include "../../include/entities/AttackBody.h"
#include <SDL2/SDL.h>

namespace Entites
{
    AttackBody::AttackBody(float x, float y, float w, float h, bool collision, bool visible, float dmg, float range, float duration, float lifesteal, float crit_chance, float crit_dmg)
        : MovingBody(x, y, w, h, collision, visible), 
          attack_damage(dmg), attack_range(range), attack_duration(duration),
          life_steal(lifesteal), critical_chance(crit_chance), critical_damage(crit_dmg) {}

    void AttackBody::Update(float deltaTime)
    {
        this->move(deltaTime);
        attack_duration -= deltaTime;
    }

    void AttackBody::Render(SDL_Renderer* renderer)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        SDL_Rect intRect = this->GetIntRect();
        SDL_RenderFillRect(renderer, &intRect);
    }

    bool AttackBody::IsExpired() const
    {
        return attack_duration <= 0.0f;
    }

    float AttackBody::getAttackDamage() const { return attack_damage; }
    float AttackBody::getAttackRange() const { return attack_range; }
    float AttackBody::getAttackDuration() const { return attack_duration; }
    float AttackBody::getLifeSteal() const { return life_steal; }
    float AttackBody::getCriticalChance() const { return critical_chance; }
    float AttackBody::getCriticalDamage() const { return critical_damage; }
}
