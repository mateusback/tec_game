#ifndef TILE_BODY_H
#define TILE_BODY_H

#include "Body.h"
#include "../map/Tile.h"

namespace Entities {
    class TileBody : public Body {
    protected:
        int tileId = -1;
        const Tile* tileData = nullptr;
        TileBody(float x = 0, float y = 0, float w = 50, float h = 50, bool solid = false)
        : Body(x, y, w, h, solid, true) {}
    public:
        #pragma region Constructors
        TileBody(float x, float y, float w, float h, SDL_Texture* texture, bool solid = false)
        : Body(x, y, w, h, solid, true) { this->setTexture(texture); }

        TileBody(Vector pos, Vector scl, SDL_Texture* texture, bool solid = false)
        : Body(pos.x, pos.y, scl.x, scl.y, solid, true) { this->setTexture(texture); }

        TileBody(Vector4 collider, SDL_Texture* texture, bool solid = false)
        : Body(collider.x, collider.y, collider.z, collider.w, solid, true) { this->setTexture(texture); }
        #pragma endregion


        void update(float deltaTime) override {}
        void onCollision(Body* other) override {}

        void initStaticTile(SDL_Texture* tileSheet, int sheetWidthPixels, int tileIndex, int tileSize = 32);
        
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
