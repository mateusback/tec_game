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

#endif
