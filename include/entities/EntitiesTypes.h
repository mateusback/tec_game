#ifndef ENTITIES_TYPES_H
#define ENTITIES_TYPES_H

namespace Entities {
    enum class EntityState {
        Idle,
        Moving,
        Attacking,
        Hidden,
        CoolingDown,
        Dead
        };
}

#endif