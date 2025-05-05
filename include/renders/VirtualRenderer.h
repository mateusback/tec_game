#ifndef VIRTUAL_RENDERER_H
#define VIRTUAL_RENDERER_H

#include <SDL2/SDL.h>
#include <algorithm>

#include <my-lib/math-vector.h>
using Vector = Mylib::Math::Vector<float, 2>;
using Vector4 = Mylib::Math::Vector<float, 4>;
using Point = Vector;

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
        Vector4 mapToScreen(float x, float y, float w = 1.0f, float h = 1.0f) const;
        void updateLayout(int tileCols, int tileRows);

        Vector tileToScreenPosition(int col, int row) const;

        float normalizeValue(float size) const { return size * this->tileSize; }
        float denormalizeValue(float size) const { return size / this->tileSize; }
        Vector normalizeVector(const Vector& vector) const { return Vector{ vector.x * this->tileSize, vector.y * this->tileSize }; }
        Vector denormalizeVector(const Vector& vector) const { return Vector{ vector.x / this->tileSize, vector.y / this->tileSize }; }

        float getTileSize() const { return this->tileSize; }
        int getScreenWidth() const { return this->screenWidth; }
        int getScreenHeight() const { return this->screenHeight; }

        float getTileSizeMultipliedBy(float multiplier) const { return this->tileSize * multiplier; }
        float getTileSizeSubtractedBy(float value) const { return this->tileSize - value; }
        float getTileSizeAddedBy(float value) const { return this->tileSize + value; }
        float getTileSizeDividedBy(float value) const { return this->tileSize / value; }
    };
}
#endif
