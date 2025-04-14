#include "../../include/entities/AttackBody.h"
#include <SDL2/SDL.h>

namespace Entities
{
    AttackBody::AttackBody(float x, float y, float w, float h, bool collision, bool visible, float dmg, float range, float duration, float lifesteal, float crit_chance, float crit_dmg)
        : MovingBody(x, y, w, h, collision, visible), 
          attack_damage(dmg), attack_range(range), attack_duration(duration),
          life_steal(lifesteal), critical_chance(crit_chance), critical_damage(crit_dmg) {
            this->bodyType = EBodyType::Attack;
          }

    void AttackBody::update(float deltaTime)
    {
        //TODO - AJUSTAR A FORMA QUE O JOGO ESTÁ LIDANDO COM OS TIROS 
        std::cout << "Attack Duration: " << this->attack_duration << std::endl;
        this->attack_duration -= deltaTime;
        if (this->attack_duration <= 0.f) {
            this->setActive(false);
            std::cout << "Retornando" << std::endl;
            return;
        }
        std::cout << "Movendo " << this->attack_duration << std::endl;
        this->move(deltaTime);
        this->attack_duration -= deltaTime;
    }
}
