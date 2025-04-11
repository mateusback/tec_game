#ifndef TILE_BODY_H
#define TILE_BODY_H

#include "Body.h"

namespace Entities {
    class TileBody : public Body {
    public:
        TileBody(Vector4 rect, SDL_Texture* texture, bool solid = false)
            : Body(rect, solid, true) {
            this->setTexture(texture);
        }
		
        EBodyType getBodyType() const override { return EBodyType::Body; }

        void update(float deltaTime) override {}
        void onCollision(Body* other) override {}
    };
}

#endif
