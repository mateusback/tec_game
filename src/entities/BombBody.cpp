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
        std::cout << "Bomba explodiu!" << std::endl;
        Vector explosionScale = {this->explosionRadius, this->explosionRadius};
        this->addDestroyEffect(this->position, explosionScale);
    
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
                    tile->setTexture(textures()->Get(destroyedTileData->spritePath));
                    tile->setCollision(destroyedTileData->solid);
                    tile->setTileId(tileData->destroyedId);
                    tile->setTileData(destroyedTileData);
        
                    std::cout << "Tile destruído: " << tileData->name
                              << " -> " << destroyedTileData->name << std::endl;
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
            0.2f
        );
        entityManager.add(std::move(effect));
    }
}
