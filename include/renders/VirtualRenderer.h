#ifndef VIRTUAL_RENDERER_H
#define VIRTUAL_RENDERER_H

#include <SDL2/SDL.h>
#include <algorithm>

#include <my-lib/math-vector.h>
#include "../utils/Types.h"

namespace Renderer {
    class VirtualRenderer {
    private:
        float tileSize;
        int screenWidth;
        int screenHeight;
        Vector2f cameraOffset = {0, 0};

    public:
        VirtualRenderer(int screenWidth, int screenHeight, int tileCols, int tileRows)
            : screenWidth(screenWidth), screenHeight(screenHeight) {
            float w = static_cast<float>(this->screenWidth) / tileCols;
            float h = static_cast<float>(this->screenHeight) / tileRows;
            this->tileSize = std::min(w, h);
        }

        Vector4f mapToScreen(float x, float y, float w = 1.0f, float h = 1.0f) const;
        Vector4f mapToScreen(Vector2i position, float w = 1.0f, float h = 1.0f) const;
        void updateLayout(int tileCols, int tileRows);

        float getVirtualWidth() const;
        float getVirtualHeight() const;

        Vector2f tileToScreenPosition(int col, int row) const;
        Vector2i screenToTilePosition(Vector2f position) const;

        float normalizeValue(float value) const { return value * this->tileSize; }
        float denormalizeValue(float value) const { return value / this->tileSize; }
        Vector2f normalizeVector(const Vector2f& vector) const { return Vector2f{ vector.x * this->tileSize, vector.y * this->tileSize }; }
        Vector2f denormalizeVector(const Vector2f& vector) const { return Vector2f{ vector.x / this->tileSize, vector.y / this->tileSize }; }

        float getTileSize() const { return this->tileSize; }
        int getScreenWidth() const { return this->screenWidth; }
        int getScreenHeight() const { return this->screenHeight; }

        float getTileSizeMultipliedBy(float multiplier) const { return this->tileSize * multiplier; }
        float getTileSizeSubtractedBy(float value) const { return this->tileSize - value; }
        float getTileSizeAddedBy(float value) const { return this->tileSize + value; }
        float getTileSizeDividedBy(float value) const { return this->tileSize / value; }
        void setCameraOffset(Vector2f offset) { this->cameraOffset = offset; }
        Vector2f getCameraOffset() const { return this->cameraOffset; }
    };
}
#endif
