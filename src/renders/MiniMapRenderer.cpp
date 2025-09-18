#include "../../include/renders/MiniMapRenderer.h"
#include "../../include/utils/GlobalAccess.h"

namespace Renderer {
MiniMapRenderer::MiniMapRenderer(SDL_Renderer* renderer, const Manager::RoomManager* roomManager)
    : roomManager(roomManager) {
}

void MiniMapRenderer::render(SDL_Renderer* renderer) {
    if (!roomManager) return;

    const auto& rooms = roomManager->getRooms();
    const auto* current = roomManager->getCurrentRoom();
    if (!current) return;

    int minimapWidthPx = 10 * (roomSizePx + spacing);
    int minimapHeightPx = 10 * (roomSizePx + spacing);

    Vector2i offset = {
        virtualRenderer()->getScreenWidth() - minimapWidthPx - 10,
        virtualRenderer()->getScreenHeight() - minimapHeightPx - 10
    };

    for (const auto& room : rooms) {
        bool visited = roomManager->wasRoomVisited(room.id);
        bool isNeighborOfVisited = false;

        for (auto dir : {EDirection::Right, EDirection::Left, EDirection::Up, EDirection::Down}) {
            const Map::Room* neighbor = roomManager->getRoomByPosition(room.x + deltaX(dir), room.y + deltaY(dir));
            if (neighbor && roomManager->wasRoomVisited(neighbor->id)) {
                isNeighborOfVisited = true;
                break;
            }
        }

        if (!visited && !isNeighborOfVisited && &room != current)
            continue;

        SDL_Rect rect;
        rect.w = rect.h = roomSizePx;
        rect.x = offset.x + (room.x * (roomSizePx + spacing));
        rect.y = offset.y + (room.y * (roomSizePx + spacing));

        // Sala do boss em vermelho
        bool isBossRoom = (room.type == Map::ERoomType::Boss);

        if (isBossRoom) {
            SDL_SetRenderDrawColor(renderer, 220, 30, 30, 255);
        } else if (&room == current) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        } else if (visited) {
            SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        } else if (isNeighborOfVisited) {
            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        }

        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &rect);
    }
}

}
