#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#include <cstdint>

namespace GameConstants {
    namespace Character {
        constexpr float BASE_HEALTH = 100.0f;
        constexpr float BASE_ATTACK_DAMAGE = 10.0f;
        constexpr float BASE_ATTACK_SPEED = 200.0f;
        constexpr float BASE_ATTACK_RANGE = 1.0f;
        constexpr float BASE_ATTACK_DURATION = 3.0f;
        constexpr float BASE_ATTACK_RATE = 0.3f;
        constexpr float BASE_DEFENSE = 0.0f;
        constexpr float BASE_ACCELERATION = 100.0f;
        constexpr uint8_t BASE_LEVEL = 1;
    }
    
    namespace Moving {
        constexpr float DEFAULT_ACCELERATION = 100.0f;
    }

    namespace Body {
        constexpr float DEFAULT_VISIBLE = true;
        constexpr float DEFAULT_COLLISION = false;
    }
}

#endif // GAME_CONSTANTS_H