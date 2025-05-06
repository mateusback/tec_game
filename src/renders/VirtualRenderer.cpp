#include "../include/renders/VirtualRenderer.h"

namespace Renderer {
    Vector4f VirtualRenderer::mapToScreen(float x, float y, float w, float h) const {
        return Vector4f{
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

    Vector2f VirtualRenderer::tileToScreenPosition(int col, int row) const {
        float tileSize = this->getTileSize();
    
        return Vector2f{
            col * tileSize,
            row * tileSize
        };
    }

}
