#include "../../include/entities/ItemBody.h"
#include "../../include/entities/PlayerBody.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/managers/AnimationLoader.h"
#include <SDL2/SDL.h>

namespace Entities
{
    void ItemBody::onCollision(Body* other)
    {
        if (this->has_collision == false) return;

        if (auto* player = dynamic_cast<PlayerBody*>(other)) {
            player->pickUpItem(this);
            this->setVisible(false);
            this->setCollision(false);
        }
    }

    void ItemBody::update(float deltaTime)
    {
        if (this->active == false) return;
        if (this->is_animated) {
            this->animator.update(deltaTime);
        }
    }

    void ItemBody::loadAnimations() {
        SDL_Texture* texture = textures()->Get(this->item.getSpritePath());

        if (!texture) return;

        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

        if (w == 32 && h == 32) {
            this->is_animated = false;
            return;
        }

        this->is_animated = true;

        Manager::AnimationManager anim;
        Manager::AnimationLoader::loadNamedAnimations(texture, {
            { "idle", 0, 30 }
        }, anim);

        this->animator.add_part(anim, Vector2f(0.f, 0.f));
        this->animator.play("idle");
    }
}
