#ifndef TYPES_H
#define TYPES_H

#include <my-lib/math-vector.h>
#include <utility>
#include <functional>

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

inline EDirection oppositeDirection(EDirection dir){
    switch (dir) {
        case EDirection::Up: return EDirection::Down;
        case EDirection::Down: return EDirection::Up;
        case EDirection::Left: return EDirection::Right;
        case EDirection::Right: return EDirection::Left;
        default: throw std::invalid_argument("Direção inválida");
    }
}

inline EDirection directionFromDelta(int dx, int dy) {
    if (dx == 1 && dy == 0)  return EDirection::Right;
    if (dx == -1 && dy == 0) return EDirection::Left;
    if (dx == 0 && dy == 1)  return EDirection::Down;
    if (dx == 0 && dy == -1) return EDirection::Up;
    throw std::invalid_argument("Delta inválido para direção");
}


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

namespace Utils {
    using Position = std::pair<int, int>;
}

namespace std {
    template<>
    struct hash<std::pair<int, int>> {
        std::size_t operator()(const std::pair<int, int>& p) const noexcept {
            std::size_t h1 = std::hash<int>{}(p.first);
            std::size_t h2 = std::hash<int>{}(p.second);
            return h1 ^ (h2 << 1);
        }
    };
}


#endif
