#include "../include/renders/VirtualRenderer.h"

namespace Renderer {
    Vector4 VirtualRenderer::mapToScreen(float x, float y, float w, float h) const {
        return Vector4{
            x * this->tileSize,
            y * this->tileSize,
            w * this->tileSize,
            h * this->tileSize
        };
    }

    void VirtualRenderer::updateLayout(int tileCols, int tileRows) {
        float sizeByWidth = static_cast<float>(this->screenWidth) / tileCols;
        float sizeByHeight = static_cast<float>(this->screenHeight) / tileRows;
        this->tileSize = std::min(sizeByWidth, sizeByHeight);
    }  

    Vector VirtualRenderer::tileToScreenPosition(int col, int row) const {
        float tileSize = this->getTileSize();
    
        return Vector{
            col * tileSize,
            row * tileSize
        };
    }

}
