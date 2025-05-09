#ifndef EVENTS_H
#define EVENTS_H

#include "../utils/Types.h"

namespace Event {
    struct PlayerPlacedBomb {
        Vector2f position;
    };

    struct BombExploded {
        Vector2f position;
        float radius;
        float damage;
    };
}

#endif