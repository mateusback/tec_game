#include "../../include/entities/ItemBody.h"
#include "../../include/entities/PlayerBody.h"
#include <SDL2/SDL.h>

namespace Entities
{
    void ItemBody::onCollision(Body* other)
    {
        if (this->has_collision == false) return;

        if (other->getBodyType() == BodyType::Player) {
            auto* player = static_cast<PlayerBody*>(other);
            player->pickUpItem(this);
            this->setVisible(false);
            this->setCollision(false);
        }
    }
}
