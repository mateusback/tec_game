#include "../../include/managers/AnimationLoader.h"
#include <iostream>

namespace Manager
{
    std::vector<Renderer::Sprite> AnimationLoader::loadFromIndices(SDL_Texture* texture, int sheetWidthPixels, const std::vector<int>& indices, int tileWidth, int tileHeight) {
        std::vector<Renderer::Sprite> frames;
        frames.reserve(indices.size());
        
        int tilesPerRow = sheetWidthPixels / tileWidth;
        std::cout << "Tiles por linha: " << tilesPerRow << std::endl;
        
        for (int idx : indices) {
            int row = idx / tilesPerRow;
            int col = idx % tilesPerRow;
        
            SDL_Rect srcRect = {
                col * tileWidth,
                row * tileHeight,
                tileWidth,
                tileHeight
            };
        
            frames.emplace_back(texture, srcRect);
        }
        
        return frames;
    }

    std::vector<Renderer::Sprite> AnimationLoader::loadRange(SDL_Texture* texture, int sheetWidthPixels, int startFrame, int frameCount, int tileWidth, int tileHeight) {
        std::vector<int> indices;
        indices.reserve(frameCount);

        for (int i = 0; i < frameCount; ++i) {
            indices.push_back(startFrame + i);
        }

        return loadFromIndices(texture, sheetWidthPixels, indices, tileWidth, tileHeight);
    }

    Renderer::Sprite AnimationLoader::loadSingleFrame(SDL_Texture* texture, int sheetWidthPixels, int frameIndex, int tileWidth, int tileHeight)
    {
        int tilesPerRow = sheetWidthPixels / tileWidth;
    
        int row = frameIndex / tilesPerRow;
        int col = frameIndex % tilesPerRow;
    
        SDL_Rect srcRect = {
            col * tileWidth,
            row * tileHeight,
            tileWidth,
            tileHeight
        };
    
        return Renderer::Sprite(texture, srcRect);
    }

} 