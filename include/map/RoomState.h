#ifndef ROOMSTATE_H
#define ROOMSTATE_H

#include <vector>
#include <memory>
#include "../utils/Types.h"

namespace Map {

    struct BaseEntityState {
        Vector2i position;
        int id;
        bool isActive = true;
    };

    struct TileState : public BaseEntityState {
        bool solid;
        double angle;
        SDL_RendererFlip flip;

        TileState(int id, Vector2i pos, bool solid, double angle = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE)
        {
            this->id = id;
            this->position = pos;
            this->solid = solid;
            this->isActive = true;
            this->angle = angle;
            this->flip = flip;
        }
    };

    struct EnemyState : public BaseEntityState {
        int health;
        
        EnemyState(int id, Vector2i pos, int health, bool isDefeated)
        {
            this->id = id;
            this->position = pos;
            this->health = health;
            this->isActive = !isDefeated;
        }
    };

    struct ItemState : public BaseEntityState {
        ItemState(int id, Vector2i pos, bool isCollected)
        {
            this->id = id;
            this->position = pos;
            this->isActive = !isCollected;
        }
    };

    struct RoomState {
        Map::Room* roomRef;
        std::vector<TileState> tiles;
        std::vector<EnemyState> enemies;
        std::vector<ItemState> items;
        bool wasVisited = false;
        bool doorsOpened = false;
    };

}

#endif