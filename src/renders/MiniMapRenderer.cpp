#include "../../include/renders/MiniMapRenderer.h"
#include "../../include/utils/GlobalAccess.h"

namespace Renderer {
MiniMapRenderer::MiniMapRenderer(SDL_Renderer* renderer, const Manager::RoomManager* roomManager)
    : roomManager(roomManager) {
}

void MiniMapRenderer::render(SDL_Renderer* renderer) {
    if (!roomManager) return;

    const auto& rooms = roomManager->getRooms();
    auto current = roomManager->getCurrentRoom();
    if (!current) return;

    int minimapWidthPx = 10 * (roomSizePx + spacing);
    int minimapHeightPx = 10 * (roomSizePx + spacing);

    Vector2i offset = {virtualRenderer()->getScreenWidth() - minimapWidthPx - 10,
        virtualRenderer()->getScreenHeight() - minimapHeightPx - 10};

    for (const auto& room : rooms) {
        bool visited = roomManager->wasRoomVisited(room.id);
        if (!visited && &room != current) continue;

        SDL_Rect rect;
        rect.w = rect.h = roomSizePx;
        rect.x = offset.x + (room.x * (roomSizePx + spacing));
        rect.y = offset.y + (room.y * (roomSizePx + spacing));

        if (&room == current) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        }

        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &rect);
    }
}

}
