#ifndef VIRTUAL_RENDERER_H
#define VIRTUAL_RENDERER_H

#include <SDL2/SDL.h>
#include <algorithm>

namespace Renderer {
    class VirtualRenderer {
    private:
        float tileSize;
        int screenWidth;
        int screenHeight;

    public:
        VirtualRenderer(int screenWidth, int screenHeight, int tileCols, int tileRows)
            : screenWidth(screenWidth), screenHeight(screenHeight) {
            float w = static_cast<float>(this->screenWidth) / tileCols;
            float h = static_cast<float>(this->screenHeight) / tileRows;
            this->tileSize = std::min(w, h);
        }

        SDL_Rect tileToScreenRect(int tileX, int tileY, int tileW = 1, int tileH = 1) const;
        void updateLayout(int tileCols, int tileRows);

        float getTileSize() const { return this->tileSize; }
        int getScreenWidth() const { return this->screenWidth; }
        int getScreenHeight() const { return this->screenHeight; }

    };
}
#endif
