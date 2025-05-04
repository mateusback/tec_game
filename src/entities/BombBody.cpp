#include "../../include/entities/BombBody.h"
#include "../../include/entities/TileBody.h"
#include "../../include/entities/EnemyBody.h"
#include "../../include/entities/EffectBody.h"
#include "../../include/Utils/GlobalAccess.h"

namespace Entities {
    void BombBody::update(float deltaTime) {
        this->timeToExplode -= deltaTime;
        if (this->timeToExplode <= 0.0f) {
            this->explode();
            this->setActive(false);
        }
    }

    void BombBody::explode() {
        audio()->playSoundEffect("bomb_explosion", 0);
        std::cout << "Bomba explodiu!" << std::endl;
        Vector explosionScale = {this->explosionRadius, this->explosionRadius};
        Vector explosionCenter = this->position + (this->scale / 2.0f);
        Vector explosionPos = explosionCenter - (explosionScale / 2.0f);
        this->addDestroyEffect(explosionPos, explosionScale);
            
        auto tiles = entityManager.getEntitiesByType<TileBody>();
        auto enemies = entityManager.getEntitiesByType<EnemyBody>();
    
        for (auto* tile : tiles) {
            float dist = (tile->getPosition() - this->getPosition()).length();
            const Tile* tileData = tile->getTileData();
            if (!tileData) continue;
        
            if (dist <= this->explosionRadius &&
                tileData->destructible &&
                tileData->destroyedId != -1)
            {
                const Tile* destroyedTileData = tileSet.getTile(tileData->destroyedId);
                if (destroyedTileData) {
                    tile->setCollision(destroyedTileData->solid);
                    tile->setTileId(tileData->destroyedId);
                    tile->setTileData(destroyedTileData);
                    tile->initStaticTile(
                        textures()->Get(tileSet.getSpriteSheetPath()), 
                        textures()->getTextureScale(textures()->Get(tileSet.getSpriteSheetPath())).x, 
                        destroyedTileData->index, 
                        tileSet.getTileSize()
                    );
                }
            }
        }
    
        for (auto* enemy : enemies) {
            float dist = (enemy->getPosition() - this->getPosition()).length();
            if (dist <= this->explosionRadius) {
                enemy->takeDamage(this->explosionDamage);
                if (enemy->getHealth() <= 0) {
                    enemy->setActive(false);
                }
            }
        }
    }

    void BombBody::addDestroyEffect(Vector position, Vector scale) {
        auto effect = std::make_unique<Entities::EffectBody>(
            position,
            scale,
            textures()->Get("bomb_explosion"),
            0.5f
        );
        entityManager.add(std::move(effect));
    }
}
