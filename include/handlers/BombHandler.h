#ifndef BOMB_EXPLOSION_HANDLER_H
#define BOMB_EXPLOSION_HANDLER_H

#include "../core/Events.h"
#include "../managers/EntityManager.h"
#include "../map/TileSet.h"
#include "../utils/types.h"

class BombHandler {
public:
BombHandler(Manager::EntityManager* entityManager, TileSet* tileSet);

private:
    Manager::EntityManager* entityManager;
    TileSet* tileSet;

    void handleExplosion(const Event::BombExploded& event);
    void putBombAt(const Vector2f& position);
};

#endif
