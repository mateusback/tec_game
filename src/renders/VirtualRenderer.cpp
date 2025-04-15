#include "../include/renders/VirtualRenderer.h"

namespace Renderer {

    SDL_Rect VirtualRenderer::tileToScreenRect(int tileX, int tileY, int tileW, int tileH) const {
        return SDL_Rect{
            static_cast<int>(tileX * this->tileSize),
            static_cast<int>(tileY * this->tileSize),
            static_cast<int>(tileW * this->tileSize),
            static_cast<int>(tileH * this->tileSize)
        };       
    }

    void VirtualRenderer::updateLayout(int tileCols, int tileRows) {
        float sizeByWidth = static_cast<float>(this->screenWidth) / tileCols;
        float sizeByHeight = static_cast<float>(this->screenHeight) / tileRows;
        this->tileSize = std::min(sizeByWidth, sizeByHeight);
    }  

}
