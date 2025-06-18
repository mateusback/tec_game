#include "../../include/managers/AnimationLoader.h"
#include "../../include/utils/GlobalAccess.h"
#include <iostream>

namespace Manager
{

    SDL_Rect AnimationLoader::computeSourceRect(SDL_Texture *texture, int frameIndex)
    {
        int width = 0, height = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

        int tilesPerRow = width / SPRITE_HEIGHT;
        int row = frameIndex / tilesPerRow;
        int col = frameIndex % tilesPerRow;

        return SDL_Rect{
            col * SPRITE_WIDTH,
            row * SPRITE_HEIGHT,
            SPRITE_WIDTH,
            SPRITE_HEIGHT};
    }

    SDL_Rect AnimationLoader::computeSourceRect(SDL_Texture *texture, int frameIndex, int spriteWidth, int spriteHeight)
    {
        int width = 0, height = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

        int tilesPerRow = width / spriteWidth;
        int row = frameIndex / tilesPerRow;
        int col = frameIndex % tilesPerRow;

        return SDL_Rect{
            col * spriteWidth,
            row * spriteHeight,
            spriteWidth,
            spriteHeight};
    }

    std::vector<Renderer::Sprite> AnimationLoader::loadFromIndices(SDL_Texture *texture, const std::vector<int> &indices)
    {
        std::vector<Renderer::Sprite> frames;
        frames.reserve(indices.size());

        for (int idx : indices)
        {
            frames.emplace_back(texture, computeSourceRect(texture, idx));
        }

        return frames;
    }

    std::vector<Renderer::Sprite> AnimationLoader::loadFromIndices(SDL_Texture *texture, const std::vector<int> &indices, int spriteWidth, int spriteHeight)
    {
        std::vector<Renderer::Sprite> frames;
        frames.reserve(indices.size());

        for (int idx : indices)
        {
            frames.emplace_back(texture, computeSourceRect(texture, idx, spriteWidth, spriteHeight));
        }

        return frames;
    }

    std::vector<Renderer::Sprite> AnimationLoader::loadRange(SDL_Texture *texture, int startFrame, int frameCount)
    {
        std::vector<int> indices;
        indices.reserve(frameCount);

        for (int i = 0; i < frameCount; ++i)
        {
            indices.push_back(startFrame + i);
        }

        return loadFromIndices(texture, indices);
    }

    std::vector<Renderer::Sprite> AnimationLoader::loadRange(SDL_Texture *texture, int startFrame, int frameCount, int spriteWidth, int spriteHeight)
    {
        std::vector<int> indices;
        indices.reserve(frameCount);

        for (int i = 0; i < frameCount; ++i)
        {
            indices.push_back(startFrame + i);
        }

        return loadFromIndices(texture, indices, spriteWidth, spriteHeight);
    }

    Renderer::Sprite AnimationLoader::loadSingleFrame(SDL_Texture *texture, int frameIndex)
    {
        if (!texture)
        {
            std::cerr << "[Erro] Textura é nula em loadSingleFrame!" << std::endl;
            return Renderer::Sprite(nullptr, {0, 0, 0, 0});
        }

        return Renderer::Sprite(texture, computeSourceRect(texture, frameIndex));
    }

    Renderer::Sprite AnimationLoader::loadSingleFrame(SDL_Texture *texture, int frameIndex, int spriteWidth, int spriteHeight)
    {
        if (!texture)
        {
            std::cerr << "[Erro] Textura é nula em loadSingleFrame!" << std::endl;
            return Renderer::Sprite(nullptr, {0, 0, 0, 0});
        }

        return Renderer::Sprite(texture, computeSourceRect(texture, frameIndex, spriteWidth, spriteHeight));
    }

    void AnimationLoader::loadNamedAnimations(SDL_Texture *texture, const std::vector<AnimationInfo> &animations, AnimationManager &manager, float frameDuration, int spriteWidth = SPRITE_WIDTH, int spriteHeight = SPRITE_HEIGHT)
    {
        int textureWidth = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, nullptr);
        int tilesPerRow = textureWidth / spriteWidth;

        for (const auto &anim : animations)
        {
            int startIndex = anim.row * tilesPerRow;
            auto frames = loadRange(texture, startIndex, anim.frameCount, spriteWidth, spriteHeight);
            Renderer::Animation animation(frames, frameDuration, anim.loop);
            manager.addAnimation(anim.name, animation);
        }
    }

    void AnimationLoader::loadStaticAnimations(SDL_Texture *texture, const std::vector<StaticAnimationInfo> &animations, AnimationManager &manager)
    {
        for (const auto &anim : animations)
        {
            SDL_Rect rect = {
                anim.column * SPRITE_WIDTH,
                anim.row * SPRITE_HEIGHT,
                SPRITE_WIDTH,
                SPRITE_HEIGHT};

            std::vector<Renderer::Sprite> frames;
            frames.emplace_back(texture, rect);

            Renderer::Animation animation(frames, 1.0f, false);
            manager.addAnimation(anim.name, animation);
        }
    }

    void AnimationLoader::loadStaticAnimations(SDL_Texture *texture, const std::vector<StaticAnimationInfo> &animations, AnimationManager &manager, int spriteWidth, int spriteHeight)
    {
        for (const auto &anim : animations)
        {
            SDL_Rect rect = {
                anim.column * spriteWidth,
                anim.row * spriteHeight,
                spriteWidth,
                spriteHeight};

            std::vector<Renderer::Sprite> frames;
            frames.emplace_back(texture, rect);

            Renderer::Animation animation(frames, 1.0f, false);
            manager.addAnimation(anim.name, animation);
        }
    }

    int AnimationLoader::calculateStartIndex(SDL_Texture *texture, int row)
    {
        int textureWidth = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, nullptr);

        int tilesPerRow = textureWidth / SPRITE_WIDTH;
        return row * tilesPerRow;
    }
}