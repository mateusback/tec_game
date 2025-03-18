#ifndef BODY_H
#define BODY_H

#include <SDL2/SDL.h>

class Body {
    SDL_FRect rect;
    bool has_collision;
    bool is_visible;
};

#endif
