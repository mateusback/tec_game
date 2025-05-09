#ifndef TYPES_H
#define TYPES_H

#include <my-lib/math-vector.h>

using Vector2f = Mylib::Math::Vector<float, 2>;
using Vector4f = Mylib::Math::Vector<float, 4>;
using Pointf = Vector2f;

using Vector2i = Mylib::Math::Vector<int, 2>;
using Vector4i = Mylib::Math::Vector<int, 4>;
using Pointi = Vector2i;

enum class EDirection {
    Up,
    Down,
    Left,
    Right
};

inline std::string directionToString(EDirection dir) {
    switch (dir) {
        case EDirection::Up: return "up";
        case EDirection::Down: return "down";
        case EDirection::Left: return "left";
        case EDirection::Right: return "right";
        default: return "unknown";
    }
}

inline EDirection stringToDirection(const std::string& str) {
    if (str == "up") return EDirection::Up;
    if (str == "down") return EDirection::Down;
    if (str == "left") return EDirection::Left;
    if (str == "right") return EDirection::Right;
    throw std::invalid_argument("Direção inválida: " + str);
}

#endif
