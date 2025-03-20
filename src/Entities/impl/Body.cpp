#include "Body.h"

Body::Body(float x, float y, float w, float h, bool collision, bool visible)
    : rect{x, y, w, h}, has_collision(collision), is_visible(visible) {}