#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#include <SDL2/SDL.h>
#include "../managers/EntityManager.h"

namespace Utils {

    class DebugUtils {
    public:
        template<typename T>
        static void drawCollidersOfType(SDL_Renderer* renderer, Manager::EntityManager& manager, SDL_Color color) {
            for (auto& e : manager.getEntities()) {
                if (auto* body = dynamic_cast<T*>(e.get())) {
                    if (body->hasCollision()) {
                        Vector4 hitbox = body->getHitbox();
                        SDL_FRect rect = {
                            hitbox.x, hitbox.y,
                            hitbox.z, hitbox.w
                        };
                        drawCollider(renderer, rect, color);
                    }
                }
            }
        }

        static void drawCollider(SDL_Renderer* renderer, const SDL_FRect& rect, SDL_Color color) {
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderDrawRectF(renderer, &rect);
        }
    };

}
 

#endif