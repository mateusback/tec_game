#ifndef TILE_BODY_H
#define TILE_BODY_H

#include "Body.h"

namespace Entities {
    class TileBody : public Body {
    protected:
        TileBody(EBodyType type, float x = 0, float y = 0, float w = 50, float h = 50, bool solid = false)
        : Body(type, x, y, w, h, solid, true) {}
    public:
        #pragma region Constructors
        TileBody(float x, float y, float w, float h, SDL_Texture* texture, bool solid = false)
        : Body(EBodyType::Tile, x, y, w, h, solid, true) { this->setTexture(texture); }

        TileBody(Vector pos, Vector scl, SDL_Texture* texture, bool solid = false)
        : Body(EBodyType::Tile, pos.x, pos.y, scl.x, scl.y, solid, true) { this->setTexture(texture); }

        TileBody(Vector4 collider, SDL_Texture* texture, bool solid = false)
        : Body(EBodyType::Tile, collider.x, collider.y, collider.z, collider.w, solid, true) { this->setTexture(texture); }
        #pragma endregion

		#pragma region Getters
        SDL_Texture* getTexture() const { return this->texture; }
        EBodyType getBodyType() const override { return EBodyType::Body; }
        #pragma endregion
        
        void update(float deltaTime) override {}
        void onCollision(Body* other) override {}
    };
}

#endif
