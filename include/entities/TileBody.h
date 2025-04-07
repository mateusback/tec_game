#ifndef TILE_BODY_H
#define TILE_BODY_H

#include "Body.h"

namespace Entities {
    class TileBody : public Body {
    public:
        TileBody(SDL_FRect rect, SDL_Texture* texture, bool solid = false)
            : Body(rect, solid, true) {
            this->setTexture(texture);
        }

        void update(float deltaTime) override {}
        void onCollision(Body* other) override {}
        BodyType getBodyType() const override { return BodyType::Body; }
    };
}

#endif
