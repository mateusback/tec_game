#ifndef MINIMAP_RENDERER_H
#define MINIMAP_RENDERER_H

#include <SDL2/SDL.h>
#include "../utils/Types.h"
#include "../managers/RoomManager.h"
namespace Renderer {
    class MiniMapRenderer {
    public:
        MiniMapRenderer(SDL_Renderer* renderer, const Manager::RoomManager* roomManager);
        void render(SDL_Renderer* renderer);

    private:
        const Manager::RoomManager* roomManager;
        int roomSizePx = 10;
        int spacing = 2;

        int deltaX(EDirection dir) {
            switch (dir) {
                case EDirection::Right: return 1;
                case EDirection::Left: return -1;
                default: return 0;
            }
        }

        int deltaY(EDirection dir) {
            switch (dir) {
                case EDirection::Down: return 1;
                case EDirection::Up: return -1;
                default: return 0;
            }
        }
    };
}

#endif