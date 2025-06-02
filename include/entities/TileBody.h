#ifndef TILE_BODY_H
#define TILE_BODY_H

#include "Body.h"
#include "../map/Tile.h"

namespace Entities {
    class TileBody : public Body {
    protected:
        int tileId = -1;
        const Tile* tileData = nullptr;
    public:
        #pragma region Constructors
        TileBody(Vector4f collider, SDL_Texture* texture, bool solid = false)
        : Body(collider, solid, true) { this->setTexture(texture); }

        TileBody(const TileBody& other)
        : Body(other), tileId(other.tileId), tileData(other.tileData) {
            this->setTexture(other.getTexture());
            this->setPosition(other.getPosition());
        }
        #pragma endregion


        void update(float deltaTime) override {}
        void onCollision(Body* other) override {}

        void initStaticTile(SDL_Texture* tileSheet, int tileIndex);
        void initFlippedStaticTile(SDL_Texture* texture, int tileIndex, double angle = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE);
        
		#pragma region Getters
        int getTileId() const { return tileId; }
        const Tile* getTileData() const { return tileData; }
        #pragma endregion
        
        #pragma region Setters
        void setTileId(int id) { tileId = id; }
        void setTileData(const Tile* tile) { tileData = tile; }
        #pragma endregion
    };
}

#endif
