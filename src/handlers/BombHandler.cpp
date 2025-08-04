#include "../../include/handlers/BombHandler.h"
#include "../../include/managers/EventManager.h"
#include "../../include/entities/TileBody.h"
#include "../../include/entities/EnemyBody.h"
#include "../../include/entities/BombBody.h"
#include "../../include/entities/EffectBody.h"
#include "../../include/utils/GlobalAccess.h"

#include <SDL2/SDL.h>

BombHandler::BombHandler(Manager::EntityManager* entityManager, TileSet* tileSet)
    : entityManager(entityManager), tileSet(tileSet)
{
    EventManager::On<Event::BombExploded>([this](const Event::BombExploded& event) {
        handleExplosion(event);
    });

    EventManager::On<Event::PlayerPlacedBomb>([this](const Event::PlayerPlacedBomb& e) {
        this->putBombAt(e.position);
    });
}

void BombHandler::handleExplosion(const Event::BombExploded& event) {
    SDL_Texture* tileSheet = Manager::TextureManager::Get("tileset");

    auto tiles = this->entityManager->getEntitiesByType<Entities::TileBody>();
    for (auto* tile : tiles) {
        float dist = (tile->getPosition() - event.position).length();
        const Tile* tileData = tile->getTileData();
        if (!tileData) continue;

        if (dist <= event.radius && tileData->destructible && tileData->destroyedId != -1) {
            const Tile* destroyedTileData = this->tileSet->getTile(tileData->destroyedId);
            if (destroyedTileData) {
                tile->setCollision(destroyedTileData->solid);
                tile->setTileId(tileData->destroyedId);
                tile->setTileData(destroyedTileData);
                tile->initFlippedStaticTile(tileSheet, destroyedTileData->index, tile->getAngle(), tile->getFlip());
            }
        }
    }

    auto enemies = this->entityManager->getEntitiesByType<Entities::EnemyBody>();
    for (auto* enemy : enemies) {
        float dist = (enemy->getPosition() - event.position).length();
        if (dist <= event.radius) {
            enemy->takeDamage(event.damage);
            if (enemy->getHealth() <= 0) {
                enemy->setActive(false);
            }
        }
    }

    auto effect = std::make_unique<Entities::EffectBody>(
        event.position - event.radius / 2,
        Vector2f{event.radius, event.radius},
        Manager::TextureManager::Get("bomb_explosion"),
        0.5f
    );
    effect->setAnimationInfo({"explosion", 0, 9, 0.1f});
    effect->loadAnimations();
    this->entityManager->add(std::move(effect));
}

void BombHandler::putBombAt(const Vector2f& position) {
    float tileSize = virtualRenderer()->getTileSize();
    auto bomb = std::make_unique<Entities::BombBody>(
        Vector4f({position.x, position.y, tileSize, tileSize}),
        false,
        true,
        2.0f,
        virtualRenderer()->normalizeValue(3.0f),
        60.0f
    );
    bomb->setTexture(Manager::TextureManager::Get("bomb"));
    this->entityManager->add(std::move(bomb));
}
