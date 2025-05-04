#include "../../include/renders/HudRenderer.h"
#include "../../include/managers/AnimationLoader.h"
#include "../../include/utils/GlobalAccess.h"

using namespace Renderer;

HudRenderer::HudRenderer(SDL_Renderer* renderer) {
    SDL_Texture* texture = textures()->Get("hudsheet");
    std::vector<int> indices = { 0, 1, 2, 3, 4 };
    this->hudIcons.load(texture, 160, indices);
}

void HudRenderer::render(SDL_Renderer* renderer, const Entities::PlayerBody* player) {
    drawHearts(renderer, player->getHealth(), player->getMaxHealth());
    drawBombs(renderer, player->getBombs());
    drawCoins(renderer, player->getCoins());
}

void HudRenderer::drawHearts(SDL_Renderer* renderer, int hp, int maxHp) {
    float spacing = virtualRenderer()->normalizeValue(0.01f);
    float size = virtualRenderer()->normalizeValue(0.2f);

    for (int i = 0; i < maxHp; ++i) {
        int iconIndex = 2;
        if (i < hp) iconIndex = 0;
        if (i == hp - 1 && hp % 1 != 0) iconIndex = 1;

        const Renderer::Sprite& icon = this->hudIcons.get(iconIndex);
        SDL_Texture* texture = icon.getTexture();
        const SDL_Rect& src = icon.getSourceRect();

        SDL_Rect dst = {
            static_cast<int>(virtualRenderer()->normalizeValue(0.1f + i) + spacing * i),
            static_cast<int>(virtualRenderer()->normalizeValue(0.4f)),
            static_cast<int>(size),
            static_cast<int>(size)
        };

        SDL_RenderCopy(renderer, texture, &src, &dst);
    }
}

void HudRenderer::drawBombs(SDL_Renderer* renderer, int bombs) {
    const Renderer::Sprite& icon = this->hudIcons.get(3);
    SDL_Texture* texture = icon.getTexture();
    const SDL_Rect& src = icon.getSourceRect();
    float size = virtualRenderer()->normalizeValue(0.2f);

    SDL_Rect dst = {
        static_cast<int>(virtualRenderer()->normalizeValue(0.5f)),
        static_cast<int>(virtualRenderer()->normalizeValue(1.8f)),
        static_cast<int>(size),
        static_cast<int>(size)
    };

    SDL_RenderCopy(renderer, texture, &src, &dst);
}

void HudRenderer::drawCoins(SDL_Renderer* renderer, int coins) {
    const Renderer::Sprite& icon = this->hudIcons.get(4);
    SDL_Texture* texture = icon.getTexture();
    const SDL_Rect& src = icon.getSourceRect();
    float size = virtualRenderer()->normalizeValue(0.2f);

    SDL_Rect dst = {
        static_cast<int>(virtualRenderer()->normalizeValue(2.0f)),
        static_cast<int>(virtualRenderer()->normalizeValue(1.8f)),
        static_cast<int>(size),
        static_cast<int>(size)
    };

    SDL_RenderCopy(renderer, texture, &src, &dst);
}
